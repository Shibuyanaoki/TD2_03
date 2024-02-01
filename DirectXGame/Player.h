#pragma once
#include "Audio.h"
#include "Base.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "math.h"


class Player : public Base {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection_);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	//void SetMove(Vector3 setMove) { setMove = move; }
	// ワールドトランスフォーム取得
	const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	// Vector3 GetWorldPosition();

	// カメラの向きと自機の向きを合わせる
	void SetViewRotate(const Vector3 parent) { worldTransform_.rotation_ = parent; }

	void falling();

	void Reset();

	// getter
	float GetRadius() { return radius_; }

	bool GetDirection() { return direction_; }

	bool GetIsSceneFlag() { return isSceneFlag; }

	//setter
	bool SetSirection(bool direction) { return direction_ = direction; }
	
	float SetInRotation(const float inRot) { return inRotation += inRot; }

	float SetOutRotation(const float OutRot) { return outRotation += OutRot; }

public: // オーバーライド
	void OnCollision(Base* other) override;
	

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// モデル
	Model* model_ = nullptr;

	// WorldTransform worldTransform_;
	Matrix4x4 rotation;

	Vector3 move_{0, 0, 0};
	Vector3 joyMove_{0, 0, 0};
	Vector3 keyMove_{0, 0, 0};
	//WorldTransform worldTransform_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	float radius_ = 2;
	float rotationSpeed_ = 0.0f;
	bool  direction_ = 0;

	float radian = 0;
	Vector3 position_ = {0.0f, 0.0f, -30.0f};
	
	float joyX = 0;
	float joyY = 0;
		
	float rot;
	float acceleration = 0.0f;
	
	float inRotation = 0.15f;
	float outRotation = 0.01f;

	const float inRotMax = 0.18f; // 0.25f;
	const float outRotMax = 0.11f;

	const float inRotMin = 0.15f;
	const float outRotMin = 0.01f;

	bool isSceneFlag = false;

};
