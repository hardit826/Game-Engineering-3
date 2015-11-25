return
{
	pathEffect = "data/effect_transparent.lua";
	
    uniform_data =
	{
		{
			name = "colorRGB",
			values = {1,0,0},
			shaderType ="transparent_fragment",
		},
		{
			name = "alphaModifier",
			values = {0.5},
			shaderType ="transparent_fragment",
		},
	},
}