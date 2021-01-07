/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
Modified by: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once
class Transform;

class ParticleSystem;
class ParticleEmitter;

struct SpriteData
{
	Texture *mTexture;
	Transform* pTr;
	glm::vec2 Tiles;
	bool isHUD;
	bool Bloom;
	bool Portal;
};

struct RenderData
{
	VertexArray *VAO;
	IndexBuffer *IBO;
	SpriteData* mSprite;
	TextData* mText;
	ParticleEmitter *mParticle;
	float zCoord;
};

struct LightData
{
	glm::vec2 Position;
	float Angle;
	float Spread;
	float Distance;
};

enum SpriteShape
{
	QUAD,
	TRIANGLE,
	SCREEN_QUAD
};


class RenderManager
{
public:
	static RenderManager* CreateInstance();
	inline static RenderManager* GetInstance() { return pObject; }
	void Init();
	void Update();
	void Close();
	void ResolutionChanged();

	inline bool IsDebugOn() { return DebugDraw; }
	inline Shader* GetTextShader() { return mTextShader; }
	inline glm::mat4 GetViewMatrix() { return mCamera.mView; }
	inline Shader* GetShader() { return mParticleShader; }
	inline void SetAmbientLight(float light) { mAmbientLight = light; }
	inline void FadeIn() { mFade = 0; mFadeValue = 0.0f; }
	inline void FadeOut() { mFade = 1; mFadeValue = 1.0f; }
	inline float GetFadeValue() { return mFadeValue; }
	inline bool FadingOut() { return mFade == 1 ? true : false; }
	inline bool FadingIn() { return mFade == 0 ? true : false; }

private:
	void ZSorting();
	void PreRendering();
	void Rendering();
	void RenderQuad();
	void PostRendering();
	void ModelsInit();
	void LoadScreen();

public:
	std::map<SpriteShape,std::pair<VertexArray*,IndexBuffer*>> mShapes;
	std::deque<RenderData> mRenderQueue;
	Camera mCamera;

	std::deque<ParticleSystem*> mParticleQueue;
	std::deque<LightData> mLightQueue;
	glm::mat4 mProjection;

private:
	float mAmbientLight;
	ParticleSystem *mParticle;
	TextRenderer mTextRenderer;
	static RenderManager* pObject;
	Shader* mShader;
	RenderManager() : mShader(nullptr) {}
	Shader* mDebugShader;
	Shader* mTextShader;
	Shader* mParticleShader;
	Shader* mBlurShader;
	Shader* mBloomShader;
	Shader* mPortalShader;

	bool DebugDraw;
	Texture* LoadingScreen;

	//Bloom
	FrameBuffer *SceneFBO;
	FrameBuffer *BlurFBO[2];
	bool BloomEnabled;
	bool PortalEnabled;

	//Fade in-out, 0: Fade in, 1: Fade out
	int mFade;
	float mFadeValue;
};