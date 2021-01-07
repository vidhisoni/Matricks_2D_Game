/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
- End Header --------------------------------------------------------*/
#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Dependencies/OpenGL/Shader.h"
#include <GL/GL.h>

struct Character
{
	GLuint textID; //ID handle of the glyph texture
	glm::vec2 size; //size of glyph
	glm::vec2 bearing; //offset from baseline to left/top of glyph
	GLuint advance; //horizontal offset to advance to next glyph
};

struct TextData
{
	std::string Text;
	float xPos, yPos, Scale;
	glm::vec3 Color;
};

class TextRenderer
{
public:
	void Init(Shader* shader);
	void Load();
	void SetTexture();
	void Destroy();
	void RenderText(TextData* data, bool isHUD);

private:
	std::map<GLchar, Character> mCharacters;
	GLuint VAO_Text, VBO_Text;
	FT_Library ft;
	FT_Face face;
	Shader* mShader;
};