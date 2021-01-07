#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) uniform mat4 u_Model;
layout (location = 3) uniform mat4 u_View;
layout (location = 4) uniform mat4 u_Proj;

uniform vec2 Tile;

out vec2 fragCoord;
out vec2 u_TexCoord;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 0.0, 1.0);
	fragCoord = (u_Model * vec4(position, 0.0, 1.0)).xy;
	u_TexCoord = TexCoord * Tile;
}