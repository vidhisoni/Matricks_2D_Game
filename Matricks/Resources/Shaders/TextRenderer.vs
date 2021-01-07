// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Vidhi Soni
//- End Header --------------------------------------------------------
#version 430 core
layout(location = 0) in vec4 vertex; // <vec3 po, vec2 tex>
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 view;
out vec2 TexCoords;
void main()
{
	gl_Position = projection * view * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}