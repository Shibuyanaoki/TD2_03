#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Base.h"
#include "ImGuiManager.h"

class Enemy :public Base {
public:
	void Initialize(Model* model);

	void Update(bool direction);

	void Draw(ViewProjection& viewProjection);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// ワールドトランスフォーム取得
	const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	//Vector3 GetWorldPosition();

	// カメラの向きと自機の向きを合わせる
	void SetViewRotate(const Vector3 parent) { worldTransform_.rotation_ = parent; }

	// getter
	float GetRadius() { return radius_; }

	public: // オーバーライド
	void OnCollision(Base* other) override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// モデル
	Model* model_ = nullptr;
	Matrix4x4 rotation;
	//WorldTransform worldTransform_;
	Vector3 move_{0.0f, 0, 0};
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	float radius_ = 2;
	float rotationSpeed_ = 0.0f;
	int direction_ = 0;
	float radian = 0;
	Vector3 position_ = {0.0f, 0.0f, -30.0f};

	float rot;
	float acceleration = 0.0f;

	float outRation = 0.01f;
	float inRation = 0.15f;

	bool collisionFlag = false;
};
