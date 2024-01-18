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

	void Draw(ViewProjection& viewProjection_, bool out);

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
	int  direction_ = 0;

	float radian = 0;
	Vector3 position_ = {0.0f, 0.0f, -30.0f};
	

	float rot;
	float acceleration = 0.0f;
};
