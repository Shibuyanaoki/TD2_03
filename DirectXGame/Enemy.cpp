#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model,Vector3 position) {
	
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	// worldTransform_.rotation_.x = 10.0f;
	
	worldTransform_.translation_ = position;

	//  NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.UpdateMatrix();

}

void Enemy::Update() {

	falling();

	if (direction_ == false) {
		if (acceleration > 0.0f) {
			acceleration -= 0.01f;
			rot -= 0.08f;
			/*joyMove_.x = -cosf(rot);
			joyMove_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;*/
		}
	}
	if (direction_ == true) {
		if (acceleration > 0.0f) {
			acceleration -= 0.01f;
			rot -= 0.08f;
			/*joyMove_.x = +cosf(rot);
			joyMove_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;*/
		}
	}

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

const WorldTransform& Enemy::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します

	return worldTransform_;
}

//Vector3 Enemy::GetWorldPosition() {
//	Vector3 worldPos;
//
//	worldPos.x = worldTransform_.matWorld_.m[3][0];
//	worldPos.y = worldTransform_.matWorld_.m[3][1];
//	worldPos.z = worldTransform_.matWorld_.m[3][2];
//
//	return worldPos;
//}

void Enemy::OnCollision(Base* other) { 
	other->GetWorldPosition(); }

void Enemy::falling() {
	if (worldTransform_.translation_.x >= 63 || worldTransform_.translation_.x <= -61 ||
	    worldTransform_.translation_.z >= 61 || worldTransform_.translation_.z <= -63) {
		worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	}
}
