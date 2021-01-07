// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Vidhi Soni
//- End Header --------------------------------------------------------
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) uniform mat4 u_Proj;
layout(location = 2) uniform mat4 u_View;
layout(location = 3) uniform mat4 u_Model;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * position;
}