// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Vidhi Soni
//- End Header --------------------------------------------------------
#version 440 core

in vec2 TexCoords;
in vec4 parColor;
out vec4 color;

uniform sampler2D sprite;
//uniform float alpha;

void main()
{
	//to be set from code
	//alpha =1.0f;
	color = texture(sprite, TexCoords) * parColor; //* ParticleColor   ;vec4(0.5f, 0.5f, 0.0f, alpha)
}