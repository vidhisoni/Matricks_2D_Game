// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//Modified by: Vidhi Soni
//- End Header --------------------------------------------------------
#version 430 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bloom;

struct lightdata
{
	vec2 pos;
	vec3 direction;
	float spread;
	float distance;
};

#define POINT_LIGHTS 100

in vec2 u_TexCoord;
uniform sampler2D mTexture;
in vec2 pixelpos;
uniform int lightnum;
uniform lightdata lightpos[POINT_LIGHTS];
uniform float Ambient;
uniform bool EnableLighting;
uniform bool BloomObject;
uniform bool BloomEnabled;

void main()
{
		color = texture(mTexture, u_TexCoord);
		if(EnableLighting)
		{
			if(lightnum > 0)
			{
				float res = 0.0;
				for(int i = 0; i < lightnum; ++i)
				{
					vec3 vector = vec3(pixelpos,0.0) - vec3(lightpos[i].pos,0.0);
					vec3 dir = normalize(vector);
					float len = length(vector);
					if(len > lightpos[i].distance)
					{
						res = max(res,Ambient);
					}
					else
					{
						float dotprod = max(dot(dir,lightpos[i].direction),0.0);
						float x = (acos(dotprod)*(Ambient - 1.0)/(lightpos[i].spread*0.01745))+1.0;
						float y = (len*(Ambient - 1.0)/lightpos[i].distance)+1.0;
						res = max(res,max(min(x,y),Ambient));
					}
				}
				res *= 1.7;
				color = color * vec4(res,res,res,1.0);
			}
			else
			{
				color = color * vec4(Ambient,Ambient,Ambient,1.0);
			}
		}
		if(BloomEnabled)
		{
			if(BloomObject)
			{
				bloom = color;
			}
			else
			{
				bloom = vec4(0.0,0.0,0.0,color.w);
			}
		}
}