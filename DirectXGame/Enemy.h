#pragma once
#include "Audio.h"
#include "Base.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Enemy : public Base {
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

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

	bool IsDead() const { return isDead_; }

public: // オーバーライド
	void OnCollision(Base* other) override;

	void falling();

	bool GetDirection() { return direction_; }

	bool SetSirection(bool direction) { return direction_ = direction; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// モデル
	Model* model_ = nullptr;

	// デスフラグ
	bool isDead_ = false;

	bool direction_ = 0;

	// ポジション
	Vector3 position_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	float radius_ = 2;

	float rot;
	float acceleration = 0.0f;
};
