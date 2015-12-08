return
{
	pathEffect = "data/effect_transparent.lua";
	
    uniform_data =
	{
		{
			name = "colorRGB",
			values = {1,1,0},
			shaderType ="fragment",
		},
		{
			name = "alphaModifier",
			values = {0.2},
			shaderType ="fragment",
		},
	},
}