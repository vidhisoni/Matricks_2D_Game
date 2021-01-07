/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
- End Header --------------------------------------------------------*/
#include "stdafx.h"

void TextRenderer::Init(Shader* shader)
{
	mShader = shader;
	//Init FreeType
	if (FT_Init_FreeType(&ft))
	{
		LogError("Error::Freetype: Could not init FreeType Library");
	}
	Load();
}

void TextRenderer::Load()
{
	//Load font as face
	if (FT_New_Face(ft, "Resources/Fonts/Kiddish.ttf", 0, &face))
	{
		LogError("Error::Freetype: Failed to load font");
	}
	//Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);
	//Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; ++c)
	{
		//Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LogError("Error::Freetype: Failed to load Glyph");
			continue;
		}

		//Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		SetTexture();
	
		glm::vec2 size_((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows);
		glm::vec2 bearing_((float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top);

		//Now store character for later use
		Character character = {
			texture,
			size_,
			bearing_,
			face->glyph->advance.x
		};
		mCharacters.insert(std::pair<GLchar, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	Destroy();

	//Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO_Text);
	glGenBuffers(1, &VBO_Text);
	glBindVertexArray(VAO_Text);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Text);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::SetTexture()
{
	//Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextRenderer::Destroy()
{
	//Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(TextData* data, bool isHUD)
{
	//Activate corresponding render state
	mShader->Bind();
	mShader->SetUniform3f("textColor", data->Color.x, data->Color.y, data->Color.z);
	if(isHUD)
		mShader->SetUniformMat4fLoc(2, glm::mat4(1.0f));
	else
		mShader->SetUniformMat4fLoc(2, RenderManager::GetInstance()->GetViewMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_Text);

	//Iterate through all characters
	std::string::const_iterator c;
	for (c = data->Text.begin(); c != data->Text.end(); ++c)
	{
		Character ch = mCharacters[*c];

		GLfloat xPos = data->xPos + ch.bearing.x * data->Scale;
		GLfloat yPos = data->yPos - (ch.size.y - ch.bearing.y) * data->Scale;

		GLfloat w = ch.size.x * data->Scale;
		GLfloat h = ch.size.y * data->Scale;

		//Update VBO_Text for each character
		GLfloat vertices[6][4] = {
			{xPos,		yPos + h,	0.0, 0.0},
			{xPos,		yPos,		0.0, 1.0},
			{xPos + w,	yPos,		1.0, 1.0},

			{xPos,		yPos + h,	0.0, 0.0},
			{xPos + w,	yPos,		1.0, 1.0},
			{xPos + w,	yPos + h,	1.0, 0.0}
		};

		//Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textID);
		//Update content of VBO_Text memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Text);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//Advance cursors for next glyph
		data->xPos += (ch.advance >> 6) * data->Scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}