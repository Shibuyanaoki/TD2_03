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

void Enemy::Update(bool direction) {
	// 速さ
	//const float speed = 0.3f;
	//move_ = Transform(move_, rotation);

	//// 移動量に速さを反映
	//move_ = Multiply(speed , Normalize(move_));
	//Matrix4x4 rotationYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
	//
	if (input_->PushKey(DIK_R)) {
		worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
		move_ = {0.0f, 0, 0};
	}
	rot -= 0.01f;
	rotation = MakeRotateYmatrix(rot);

	if (direction == false) {
		if (acceleration > 0.0f) {
			acceleration -= 0.01f;
			rot -= 0.15f;
			move_.x = -cosf(rot);
			move_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;
		}
	}
	if (direction == true) {
		if (acceleration > 0.0f) {
			acceleration -= 0.01f;
			rot -= 0.15f;
			move_.x = +cosf(rot);
			move_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;
		}
	}
	if (direction == true) {
		worldTransform_.rotation_.y -= rotationSpeed_;
	}
	if (direction == false) {
		worldTransform_.rotation_.y += rotationSpeed_;
	}
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

const WorldTransform& Enemy::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します

	return worldTransform_;
}




void Enemy::OnCollision(Base* other) {

	other->GetWorldPosition().x;
	GetWorldPosition();
	radian = getRadian(
	    GetWorldPosition().x, GetWorldPosition().z, other->GetWorldPosition().x,
	    other->GetWorldPosition().z);
	/*keyMove_.x -= cosf(radian + 3.14f / 2) * 1.0f;
	keyMove_.z -= sinf(radian + 3.14f / 2) * 1.0f;
	*/
	// 反射角
	rot = -(radian + 3.14f / 4);
	acceleration = 1.0f;
	rotationSpeed_ = 0.1f;
	// keyMove_.x = -cosf(radian + 3.14f / 4) * 1.0f;
	// keyMove_.z = -sinf(radian + 3.14f / 4) * 1.0f;

	// SetMove({cosf(radian * 3.14f / 2), 0.0f, sinf(radian * 3.14f / 2)});
void Enemy::falling() {
	if (worldTransform_.translation_.x >= 63 || worldTransform_.translation_.x <= -61 ||
	    worldTransform_.translation_.z >= 61 || worldTransform_.translation_.z <= -63) {
		worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	}
}
