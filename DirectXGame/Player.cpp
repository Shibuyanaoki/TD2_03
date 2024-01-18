#include "Player.h"
#include "ImGuiManager.h"

#include <cassert>

void Player::Initialize(Model* model) {

	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	// worldTransform_.rotation_.x = 10.0f;
	//  NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
}

void Player::Update() {

	Reverse();

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, // Lスティックの横成分
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed, // Lスティックの縦成分
		};

		/*Matrix4x4 rotationXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotationYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotationZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
		Matrix4x4 rotationXYZMatrix =
		    Multiply(rotationXMatrix, Multiply(rotationYMatrix, rotationZMatrix));*/

		//// 移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量に速さを反映(θ度の移動ベクトル)
		// rotation = (viewProjection_->rotation_.y);

		//move = Transform(move, rotationXYZMatrix);

		/*if (move.y != 0 || move.z != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}*/

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	ImGui::Begin("Player");
	float Position[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	ImGui::SliderFloat3("Player Translation", Position, -65.0f, 65.0f);

	worldTransform_.translation_.x = Position[0];
	worldTransform_.translation_.y = Position[1];
	worldTransform_.translation_.z = Position[2];

	ImGui::End();

	falling();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

const WorldTransform& Player::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します

	return worldTransform_;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::falling() {

	if (worldTransform_.translation_.x >= 63 || worldTransform_.translation_.x <= -61 ||
	    worldTransform_.translation_.z >= 61 || worldTransform_.translation_.z <= -63) {
		worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	}
}

void Player::Reverse() {

	if (rightRotate == true) {
		worldTransform_.rotation_.y += 0.5f;
	}
}
