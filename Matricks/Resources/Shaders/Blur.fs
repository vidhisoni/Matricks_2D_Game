// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//- End Header --------------------------------------------------------
#version 330 core

//Credits: LearnOpenGL Guassian blur

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;

uniform bool horizontal;
//uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
uniform float weight[10] = float[] (0.132572,0.125472,0.106373,0.08078,0.05495,0.033482,0.018275,0.008934,0.003912,0.001535);

void main()
{          


     vec2 tex_offset = 1.0 / textureSize(scene, 0); // gets size of single texel
     vec3 result = texture(scene, TexCoords).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 10; ++i)
         {
            result += texture(scene, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(scene, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 10; ++i)
         {
             result += texture(scene, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(scene, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec4(result, 1.0);
}

