// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//- End Header --------------------------------------------------------
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blur;
uniform int Fade;
uniform float FadeValue;

void main()
{             
    vec3 sceneColor = texture(scene, TexCoords).rgb;      
    vec4 bloomColor = texture(blur, TexCoords);
	sceneColor += bloomColor.rgb; // additive blending
	if(Fade != -1)
	{
		FragColor = vec4(sceneColor,1.0) * vec4(FadeValue,FadeValue,FadeValue,1.0);
	}
	else
	{
		FragColor = vec4(sceneColor, 1.0);
	}
}