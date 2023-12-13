#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model) {
	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	// worldTransform_.rotation_.x = 10.0f;
	//  NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
}

void Enemy::Update() {}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

const WorldTransform& Enemy::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します

	return worldTransform_;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
