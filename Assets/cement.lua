return
{
	pathEffect = "data/effect.lua";
	
    uniform_data =
	{
		{
			name = "colorRGB",
			values = {1,1,1},
			shaderType ="fragment",
		},
	},
	textureData =
	{
		{
			name = "g_textureSampler",
			path = "data/cement_wall_D.png",
		},
	},
}