// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Vidhi Soni
//- End Header --------------------------------------------------------
#version 440 core

layout(location = 0) in vec2 position; 
layout(location = 1) in vec2 textCoords;
layout (location = 2) uniform mat4 u_Model;
layout (location = 3) uniform mat4 u_View;
layout (location = 4) uniform mat4 u_Proj;
layout(location = 5) uniform vec2 offset;
layout(location = 6) uniform vec4 ParticleColor;

out vec2 TexCoords;
out vec4 parColor;

void main()
{
	float scale = 0.1f;
	TexCoords = textCoords;
	parColor = ParticleColor;
	gl_Position = u_Proj *u_View*u_Model* vec4((position*scale), 0.0, 1.0);
}