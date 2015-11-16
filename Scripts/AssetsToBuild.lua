return
{
	meshes = 
	{
		source = "",
		target = "",
		tool = "MeshBuilder.exe",
		assets =
		{
			"rectangle.mesh",
            "triangle.mesh",
			"box.mesh",
			"floor.mesh",
			"floorMaya.mesh",
			"house.mesh",
			"man.mesh",
			"sphere.mesh",
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
		},
		dependencies =
		{
		},
	},
}