// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Vidhi Soni
//- End Header --------------------------------------------------------
#version 430 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bloom;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;
uniform int Fade;
uniform float FadeValue;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	if(Fade != -1)
		color = vec4(textColor, 1.0) * sampled * vec4(FadeValue,FadeValue,FadeValue,1.0);
	else
		color = vec4(textColor, 1.0) * sampled;
	bloom = vec4(0.0,0.0,0.0,1.0);
}