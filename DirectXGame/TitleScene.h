#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"
#include "TextureManager.h"

class TitleScene {
public:
	void Initialize();

	void Update();

	void Draw();

	bool IsSceneEnd() { return isSceneEnd; }

	Scene NextScene() { return Scene::GAME; }

	void Reset();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	// サウンド
	uint32_t bgmHandle_;
	uint32_t playBGM_;
	bool isBGM_;

	uint32_t buttonSound_;

	Sprite* fadeSprite = nullptr;
	Vector4 fadeColor = {1.0f, 1.0f, 1.0f, 1.0f};

	// シーンを終わらせるフラグ
	bool isSceneEnd = false;
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;


};
