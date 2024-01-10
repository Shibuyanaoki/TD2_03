#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model) {

	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	//worldTransform_.rotation_.x = 10.0f;
	// NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
	worldTransform_.translation_ = Position;
}

void Player::Update(){

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	
	// 速さ
	const float speed = 0.3f;
	if (input_->PushKey(DIK_W)) {
		move.z += speed;
	} else if (input_->PushKey(DIK_S)) {
		move.z -= speed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move.x -= speed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += speed;
	}
	

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動量
		 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, // Lスティックの横成分
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed, // Lスティックの縦成分
		};
	}
	Matrix4x4 rotationXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
	Matrix4x4 rotationYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
	Matrix4x4 rotationZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
	Matrix4x4 rotationXYZMatrix =
	    Multiply(rotationXMatrix, Multiply(rotationYMatrix, rotationZMatrix));

	// 移動量に速さを反映
	move = Multiply(speed, Normalize(move));

	// 移動量に速さを反映(θ度の移動ベクトル)
	// rotation = (viewProjection_->rotation_.y);

	move = Transform(move, rotationYMatrix);

	if (move.y != 0 || move.z != 0) {
		 worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection, bool out) {

	if (out == false) {
		 model_->Draw(worldTransform_, viewProjection); 
	}
}

const WorldTransform& Player::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します

	return worldTransform_;
}
void Player::OnCollision(Base* other) {
	other->GetWorldPosition().x;
	GetWorldPosition();
	radian = getRadian(
	    GetWorldPosition().x, GetWorldPosition().z, other->GetWorldPosition().x,
	    other->GetWorldPosition().z);
	 move.x -= cosf(radian *3.14f/2) * 0.5f;
	 move.z -= sinf(radian * 3.14f / 2) * 0.5f;
	//SetMove({cosf(radian * 3.14f / 2), 0.0f, sinf(radian * 3.14f / 2)});

}
//
//Vector3 Player::GetWorldPosition() {
//	Vector3 worldPos;
//
//	worldPos.x = worldTransform_.matWorld_.m[3][0];
//	worldPos.y = worldTransform_.matWorld_.m[3][1];
//	worldPos.z = worldTransform_.matWorld_.m[3][2];
//
//	return worldPos;
//}
