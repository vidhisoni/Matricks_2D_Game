/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Transform;

class Camera
{
public:
	Camera();
	~Camera();

	void Update();
	void Init();
	inline bool IsCameraControl() { return CameraControl; }
	inline glm::vec2 GetPosition() { return mPosition; }
	void Serialize(rapidjson::Value::Object camera);
	void SnaptoY();
	void Reset();

	// direction = 0 (X-axis) , 1 (Y-axis)
	void MoveCamera(short direction, float distance, bool look = false);
	inline void NotLooking() { Looking = false; }

	// zoom = 0 (ZoomIn) , 1 (ZoomOut)
	void ZoomCamera(short zoom, float factor, glm::vec2 position = glm::vec2(0.0f));
	inline void ResetCamera() { mZoomTo = glm::vec2(1.0f); }

private:
	void Zooming();

public:
	glm::mat4 mView;
private:
	Transform* mPlayerTransform;
	glm::vec2 mPosition, mTempPosition;
	bool LevelCamera;
	glm::vec2 mLastPosition;
	glm::vec2 mOffset;
	float MoveinX, MoveinY;
	bool mMoveinLevel; //Move Camera for this level
	bool Looking; //Hacky
	glm::vec2 mLerpSpeed;

	//Debug Camera
	bool CameraControl;
	
	//Zooming
	bool Zoom;
	glm::vec2 mZoomTo, mZoomPos;
	glm::vec2 mScale, mTempScale;

	//LevelCamera
	float mDelay;
};
