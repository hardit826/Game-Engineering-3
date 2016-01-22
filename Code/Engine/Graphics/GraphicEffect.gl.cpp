#include "../Graphics/Graphics.h"
#include "../Graphics/GraphicEffect.h"
#include <cassert>
#include <gl/GLU.h>
#include "../Math/Functions.h"
#include <sstream>
#include "../Math/cMatrix_transformation.h"
#include "../UserOutput/UserOutput.h"
#include "../Windows/WindowsFunctions.h"

#define Alpha_Transparency 0
#define Depth_Testing 1
#define Depth_Writing 2
#define Face_Culling 3

eae6320::Graphics::GraphicEffect::GraphicEffect(char* const i_path_effect)
{
	o_path_effect = i_path_effect;
	o_programID = 0;
	o_vertexShaderPath = NULL;
	o_fragmentShaderPath = NULL;
}
bool eae6320::Graphics::GraphicEffect::LoadShaders()
{
	//o_vertexShaderPath = i_vertexShaderPath;
	//o_fragmentShaderPath = i_fragmentShaderPath;

	if (!CreateProgram())
	{
		return false;
	}
	if (!ReadFromBinEffectFile())
	{
		return false;
	}
	if (!LoadVertexShader())
	{
		return false;
	}
	if (!LoadFragmentShader())
	{
		return false;
	}
	if (!LinkProgram())
	{
		return false;
	}

	o_uniformLocation = glGetUniformLocation(o_programID, "g_position_offset");
	g_transform_localToWorld = glGetUniformLocation(o_programID, "g_transform_localToWorld");
	g_transform_worldToView = glGetUniformLocation(o_programID, "g_transform_worldToView");
	g_transform_viewToScreen = glGetUniformLocation(o_programID, "g_transform_viewToScreen");
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

void eae6320::Graphics::GraphicEffect::SetPath()
{
	glUseProgram(o_programID);
	assert(glGetError() == GL_NO_ERROR);

	SetRenderState();
}

bool eae6320::Graphics::GraphicEffect::CreateProgram()
{
	// Create a program
	{
		o_programID = glCreateProgram();
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to create a program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
		else if (o_programID == 0)
		{
			eae6320::UserOutput::Print("OpenGL failed to create a program");
			return false;
		}
	}

	return true;
}

bool eae6320::Graphics::GraphicEffect::LinkProgram()
{
	// Link the program
	{
		glLinkProgram(o_programID);
		GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get link info
			// (this won't be used unless linking fails
			// but it can be useful to look at when debugging)
			std::string linkInfo;
			{
				GLint infoSize;
				glGetProgramiv(o_programID, GL_INFO_LOG_LENGTH, &infoSize);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					sLogInfo info(static_cast<size_t>(infoSize));
					GLsizei* dontReturnLength = NULL;
					glGetProgramInfoLog(o_programID, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						linkInfo = info.memory;
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get link info of the program: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						return false;
					}
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get the length of the program link info: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
			}
			// Check to see if there were link errors
			GLint didLinkingSucceed;
			{
				glGetProgramiv(o_programID, GL_LINK_STATUS, &didLinkingSucceed);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didLinkingSucceed == GL_FALSE)
					{
						std::stringstream errorMessage;
						errorMessage << "The program failed to link:\n" << linkInfo;
						eae6320::UserOutput::Print(errorMessage.str());
						return false;
					}
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to find out if linking of the program succeeded: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to link the program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	return true;
}

void eae6320::Graphics::GraphicEffect::SetDrawCallUniforms(eae6320::Math::cMatrix_transformation i_mvpMatrixTransformation,Camera i_camera)
{

	const float aspectRatio = (float)4 / 3;
	Math::cVector cameraPosition = Math::cVector(0, 0, 10);
	Math::cQuaternion cameraRotation = Math::cQuaternion();

	const float z_nearPlane = 0.1f;
	const float z_farPlane = 4000.0f;
	const float fieldOfView = Math::ConvertDegreesToRadians(60); //60 degree field of view

	Math::cMatrix_transformation g_matrix_worldToView = Math::cMatrix_transformation::cMatrix_transformation::
		CreateWorldToViewTransform(i_camera.camRotation, i_camera.camPosition);

	Math::cMatrix_transformation g_matrix_viewToScreen = Math::cMatrix_transformation::cMatrix_transformation::
		CreateViewToScreenTransform(fieldOfView, aspectRatio, z_nearPlane, z_farPlane);

	const GLboolean dontTranspose = false; // Matrices are already in the correct format
	const GLsizei uniformCountToSet = 1;

	glUniformMatrix4fv(g_transform_localToWorld, uniformCountToSet, dontTranspose,
		reinterpret_cast<const GLfloat*>(&i_mvpMatrixTransformation));

	glUniformMatrix4fv(g_transform_worldToView, uniformCountToSet, dontTranspose,
		reinterpret_cast<const GLfloat*>(&g_matrix_worldToView));

	glUniformMatrix4fv(g_transform_viewToScreen, uniformCountToSet, dontTranspose,
		reinterpret_cast<const GLfloat*>(&g_matrix_viewToScreen));

	assert(glGetError() == GL_NO_ERROR);
}

bool eae6320::Graphics::GraphicEffect::LoadVertexShader()
{
	// Verify that compiling shaders at run-time is supported
	{
		GLboolean isShaderCompilingSupported;
		glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
		if (!isShaderCompilingSupported)
		{
			eae6320::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
			return false;
		}
	}

	bool wereThereErrors = false;

	// Load the source code from file and set it into a shader
	GLuint vertexShaderId = 0;
	void* shaderSource = NULL;
	{
		// Load the shader source code
		size_t fileSize;
		{
			//const char* sourceCodeFileName = "data/vertex.shader";
			std::string errorMessage;
			if (!LoadAndAllocateShaderProgram(o_vertexShaderPath, shaderSource, fileSize, &errorMessage))
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print(errorMessage);
				goto OnExit;
			}
		}
		// Generate a shader
		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to get an unused vertex shader ID: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
			else if (vertexShaderId == 0)
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print("OpenGL failed to get an unused vertex shader ID");
				goto OnExit;
			}
		}
		// Set the source code into the shader
		{
			const GLsizei shaderSourceCount = 1;
			const GLchar* shaderSources[] =
			{
			
				reinterpret_cast<GLchar*>(shaderSource)
			};
			const GLint* sourcesAreNullTerminated = NULL;
			glShaderSource(vertexShaderId, shaderSourceCount, shaderSources, sourcesAreNullTerminated);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to set the vertex shader source code: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
	}
	// Compile the shader source code
	{
		glCompileShader(vertexShaderId);
		GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get compilation info
			// (this won't be used unless compilation fails
			// but it can be useful to look at when debugging)
			std::string compilationInfo;
			{
				GLint infoSize;
				glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoSize);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					sLogInfo info(static_cast<size_t>(infoSize));
					GLsizei* dontReturnLength = NULL;
					glGetShaderInfoLog(vertexShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						compilationInfo = info.memory;
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get compilation info of the vertex shader source code: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get the length of the vertex shader compilation info: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Check to see if there were compilation errors
			GLint didCompilationSucceed;
			{
				glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didCompilationSucceed == GL_FALSE)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "The vertex shader failed to compile:\n" << compilationInfo;
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to find out if compilation of the vertex shader source code succeeded: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to compile the vertex shader source code: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}
	// Attach the shader to the program
	{
		glAttachShader(o_programID, vertexShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to attach the vertex shader to the program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}

OnExit:

	if (vertexShaderId != 0)
	{
		// Even if the shader was successfully compiled
		// once it has been attached to the program we can (and should) delete our reference to it
		// (any associated memory that OpenGL has allocated internally will be freed
		// once the program is deleted)
		glDeleteShader(vertexShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to delete the vertex shader ID: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
		}
		vertexShaderId = 0;
	}
	if (shaderSource != NULL)
	{
		free(shaderSource);
		shaderSource = NULL;
	}

	return !wereThereErrors;
}

bool eae6320::Graphics::GraphicEffect::LoadFragmentShader()
{
	// Verify that compiling shaders at run-time is supported
	{
		GLboolean isShaderCompilingSupported;
		glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
		if (!isShaderCompilingSupported)
		{
			eae6320::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
			return false;
		}
	}

	bool wereThereErrors = false;

	// Load the source code from file and set it into a shader
	GLuint fragmentShaderId = 0;
	void* shaderSource = NULL;
	{
		// Load the shader source code
		size_t fileSize;
		{
			//const char* sourceCodeFileName = "data/fragment.shader";
			std::string errorMessage;
			if (!LoadAndAllocateShaderProgram(o_fragmentShaderPath, shaderSource, fileSize, &errorMessage))
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print(errorMessage);
				goto OnExit;
			}
		}
		// Generate a shader
		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to get an unused vertex shader ID: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
			else if (fragmentShaderId == 0)
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print("OpenGL failed to get an unused vertex shader ID");
				goto OnExit;
			}
		}
		// Set the source code into the shader
		{
			const GLsizei shaderSourceCount = 1;
			const GLchar* shaderSources[] =
			{

				reinterpret_cast<GLchar*>(shaderSource)
			};
			const GLint* sourcesAreNullTerminated = NULL;
			glShaderSource(fragmentShaderId, shaderSourceCount, shaderSources, sourcesAreNullTerminated);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to set the vertex shader source code: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
	}
	// Compile the shader source code
	{
		glCompileShader(fragmentShaderId);
		GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get compilation info
			// (this won't be used unless compilation fails
			// but it can be useful to look at when debugging)
			std::string compilationInfo;
			{
				GLint infoSize;
				glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoSize);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					sLogInfo info(static_cast<size_t>(infoSize));
					GLsizei* dontReturnLength = NULL;
					glGetShaderInfoLog(fragmentShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						compilationInfo = info.memory;
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get compilation info of the vertex shader source code: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get the length of the vertex shader compilation info: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Check to see if there were compilation errors
			GLint didCompilationSucceed;
			{
				glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didCompilationSucceed == GL_FALSE)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "The vertex shader failed to compile:\n" << compilationInfo;
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to find out if compilation of the vertex shader source code succeeded: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to compile the vertex shader source code: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}
	// Attach the shader to the program
	{
		glAttachShader(o_programID, fragmentShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to attach the vertex shader to the program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}

OnExit:

	if (fragmentShaderId != 0)
	{
		// Even if the shader was successfully compiled
		// once it has been attached to the program we can (and should) delete our reference to it
		// (any associated memory that OpenGL has allocated internally will be freed
		// once the program is deleted)
		glDeleteShader(fragmentShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to delete the vertex shader ID: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
		}
		fragmentShaderId = 0;
	}
	if (shaderSource != NULL)
	{
		free(shaderSource);
		shaderSource = NULL;
	}

	return !wereThereErrors;
}

void eae6320::Graphics::GraphicEffect::SetRenderState()
{
	//Alpha_Transparency 
	if ((*render_state_value >> Alpha_Transparency) & 1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	//Depth_Testing 
	if ((*render_state_value >> Depth_Testing) & 1)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	//Depth_Writing 
	if ((*render_state_value >> Depth_Writing) & 1)
	{
		glDepthMask(GL_TRUE);
	}
	else
	{
		glDepthMask(GL_FALSE);
	}
	//Face_Culling 
	if ((*render_state_value >> Face_Culling) & 1)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

bool eae6320::Graphics::GraphicEffect::LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage)
{
	bool wereThereErrors = false;

	// Load the shader source from disk
	o_shader = NULL;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	{
		// Open the file
		{
			const DWORD desiredAccess = FILE_GENERIC_READ;
			const DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
			SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
			const DWORD onlySucceedIfFileExists = OPEN_EXISTING;
			const DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
			const HANDLE dontUseTemplateFile = NULL;
			fileHandle = CreateFile(i_path, desiredAccess, otherProgramsCanStillReadTheFile,
				useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile);
			if (fileHandle == INVALID_HANDLE_VALUE)
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to open the shader file: " << windowsErrorMessage;
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		// Get the file's size
		{
			LARGE_INTEGER fileSize_integer;
			if (GetFileSizeEx(fileHandle, &fileSize_integer) != FALSE)
			{
				assert(fileSize_integer.QuadPart <= SIZE_MAX);
				o_size = static_cast<size_t>(fileSize_integer.QuadPart);
			}
			else
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to get the size of shader: " << windowsErrorMessage;
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
			// Add an extra byte for a NULL terminator
			o_size += 1;
		}
		// Read the file's contents into temporary memory
		o_shader = malloc(o_size);
		if (o_shader)
		{
			DWORD bytesReadCount;
			OVERLAPPED* readSynchronously = NULL;
			if (ReadFile(fileHandle, o_shader, o_size,
				&bytesReadCount, readSynchronously) == FALSE)
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to read the contents of shader: " << windowsErrorMessage;
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			if (o_errorMessage)
			{
				std::stringstream errorMessage;
				errorMessage << "Failed to allocate " << o_size << " bytes to read in the shader program " << i_path;
				*o_errorMessage = errorMessage.str();
			}
			goto OnExit;
		}
		// Add the NULL terminator
		reinterpret_cast<char*>(o_shader)[o_size - 1] = '\0';
	}

OnExit:

	if (wereThereErrors && o_shader)
	{
		free(o_shader);
		o_shader = NULL;
	}
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		if (CloseHandle(fileHandle) == FALSE)
		{
			if (!wereThereErrors && o_errorMessage)
			{
				std::string windowsError = eae6320::GetLastWindowsError();
				std::stringstream errorMessage;
				errorMessage << "Windows failed to close the shader file handle: " << windowsError;
				*o_errorMessage = errorMessage.str();
			}
			wereThereErrors = true;
		}
		fileHandle = INVALID_HANDLE_VALUE;
	}

	return !wereThereErrors;
}

GLint eae6320::Graphics::GraphicEffect::GetUniformHandle(int i_vertexOrFragmentShader, const char* i_uniformName)
{
	return glGetUniformLocation(o_programID, i_uniformName);
}

void eae6320::Graphics::GraphicEffect::SetUniformHandle(int o_vertexOrFragmentShader, UniformData i_uniformData)
{
	int uniformCount = 1;

	switch (i_uniformData.valueCountToSet)
	{
	case 1:
		glUniform1fv(i_uniformData.uniformHandle, uniformCount, i_uniformData.values);
		break;

	case 2:
		glUniform2fv(i_uniformData.uniformHandle, uniformCount, i_uniformData.values);
		break;

	case 3:
		glUniform3fv(i_uniformData.uniformHandle, uniformCount, i_uniformData.values);
		break;

	case 4:
		glUniform4fv(i_uniformData.uniformHandle, uniformCount, i_uniformData.values);
		break;
	}

}

GLint eae6320::Graphics::GraphicEffect::GetSampler2DID(const char* i_uniformName)
{
	return glGetUniformLocation(o_programID, i_uniformName);
}

void eae6320::Graphics::GraphicEffect::SetSampler2DID(Texture::tSampler2D i_sampler, Texture::tTextureHandle i_textureHandle, tTextureUnit i_textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + i_textureUnit);
	glBindTexture(GL_TEXTURE_2D, i_textureHandle);
	glUniform1i(i_sampler, i_textureUnit);
}