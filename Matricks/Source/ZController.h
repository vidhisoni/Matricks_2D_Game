#pragma once

class ZController : public Component
{
public:
	ZController();
	~ZController();

	Component* Create();
	void Update();
	void Clear();

private:
	float getAngleFromMouse(glm::vec2 PlayerPos);
	float getAngleVector(glm::vec2 mVec1, glm::vec2 aVec2);
};