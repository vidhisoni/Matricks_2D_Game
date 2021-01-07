// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//Modified by: Vidhi Soni
//- End Header --------------------------------------------------------
#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) uniform mat4 u_Model;
layout (location = 3) uniform mat4 u_View;
layout (location = 4) uniform mat4 u_Proj;

uniform vec2 Tile;
out vec2 u_TexCoord;
out vec2 pixelpos;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position,0.0,1.0);
	pixelpos = vec2(u_Model*vec4(position,0.0,1.0));
	u_TexCoord = TexCoord * Tile;
}