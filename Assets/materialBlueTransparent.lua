return
{
	pathEffect = "data/effect_transparent.lua",
	
    uniform_data =
	{
		{
			name = "colorRGB",
			values = {0,0,1},
			shaderType ="fragment",
		},
		{
			name = "alphaModifier",
			values = {0.7},
			shaderType ="fragment",
		},
	},
}