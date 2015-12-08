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
			"BigBox.mesh",
			"box1.mesh",
			"box2.mesh",
			"box3.mesh",
			"box4.mesh",
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
			"materialBlueTransparent.lua",
			"defaultMaterial.lua",
			"materialGreenOpaque.lua",
			"materialRedOpaque.lua",
			"materialYellowTransparent.lua",
		},
		dependencies =
		{
		},
	},
}