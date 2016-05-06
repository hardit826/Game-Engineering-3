return
{
	meshes = 
	{
		source = "",
		target = "",
		tool = "MeshBuilder.exe",
		assets =
		{
			"Ceiling.mesh",
			"Floor.mesh",
			"Metal.mesh",
			"Railing.mesh",
			"Lambert2.mesh",
			"Walls.mesh",
			"Cement.mesh",
			"Cylinder.mesh",
			"Robot.mesh"
		},
		dependencies = 
		{
		},
	},
	vertexShaders = 
	{
		source = "",
		target = "",
		tool = "VertexShaderBuilder.exe",
		assets =
		{
			"vertex.shader",
		},
		dependencies = 
		{
			"shaders.inc",
		},
	},
	fragmentShaders = 
	{
		source = "",
		target = "",
		tool = "FragmentShaderBuilder.exe",
		assets =
		{
			"fragment.shader",
			"transparent_fragment.shader",
		},
		dependencies =
		{
			"shaders.inc",
		},
	},
	effect = 
	{
		source = "",
		target = "",
		tool = "EffectBuilder.exe",
		assets =
		{
			"effect.lua",
			"effect_transparent.lua",
		},
		dependencies =
		{
		},
	},
	material = 
	{
		source = "",
		target = "",
		tool = "MaterialBuilder.exe",
		assets =
		{
			"ceiling.lua",
			"floor.lua",
			"metal.lua",		
			"cement.lua",
			"wall.lua",
			"railing.lua",
			"robot.lua"
		},
		dependencies =
		{
		},
	},
	textures =
	{
		source = "",
		target = "",
		tool = "TextureBuilder.exe",
		assets =
		{
			"floor_D.png",
			"metal_brace_D.png",
			"railing_D.png",
			"wall_D.png",
			"cement_wall_D.png",
			"numbers.png",
			"logo.png",
			"checkbox.png",
			"button.png",
			"robot.png"
		},
		dependencies =
		{
		},
	},
}	