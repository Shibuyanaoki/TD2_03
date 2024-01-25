#include "Player.h"
#include "ImGuiManager.h"

#include "MT.h"
#include <cassert>
#include <imgui.h>

void Player::Initialize(Model* model) {

	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	// worldTransform_.rotation_.x = 10.0f;
	//  NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
	worldTransform_.translation_ = position_;
}

void Player::Update() {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// 速さ
	const float speed = 0.3f;
	if (input_->PushKey(DIK_W)) {
		keyMove_.z += speed;
	} else if (input_->PushKey(DIK_S)) {
		keyMove_.z -= speed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		keyMove_.x -= speed;
	} else if (input_->PushKey(DIK_D)) {
		keyMove_.x += speed;
	}

	falling();

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動量
		if ((float)joyState.Gamepad.sThumbLX / SHRT_MAX != 0) {
			joyMove_.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed; // Lスティックの横成分
		}
		if ((float)joyState.Gamepad.sThumbLY / SHRT_MAX != 0) {
			joyMove_.z = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed; // Lスティックの横成分
		}
	}

	// Matrix4x4 rotationXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
	Matrix4x4 rotationYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
	// Matrix4x4 rotationZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
	// Matrix4x4 rotationXYZMatrix =Multiply(rotationXMatrix, Multiply(rotationYMatrix,
	// rotationZMatrix));
	rotation = MakeRotateYmatrix(rot);

	// 移動量に速さを反映(θ度の移動ベクトル)
	// rotation = (viewProjection_->rotation_.y);

	// move_ = Transform(move_, rotationYMatrix);

	// move_ = Transform(keyMove_, rotation);
	//
	//// 移動量に速さを反映
	// move_ = Multiply(speed + acceleration, Normalize(keyMove_));

	// if (direction_ == 0) {
	//	if (acceleration > 0.0f) {
	//		acceleration -= 0.01f;
	//		rot -= 0.08f;
	//		keyMove_.x = -cosf(rot);
	//		keyMove_.z = -sinf(rot);
	//		rotationSpeed_ -= 0.01f;
	//	}
	// }
	// if (direction_ == 1) {
	//	if (acceleration > 0.0f) {
	//		acceleration -= 0.01f;
	//		rot -= 0.08f;
	//		keyMove_.x = +cosf(rot);
	//		keyMove_.z = -sinf(rot);
	//		rotationSpeed_ -= 0.01f;
	//	}
	// }

	move_ = Transform(joyMove_, rotation);

	// 移動量に速さを反映
	move_ = Multiply(speed + acceleration, Normalize(joyMove_));

	if (direction_ == false) {
		if (acceleration > 0.0f) {
			acceleration -= 0.01f;
			rot -= 0.08f;
			joyMove_.x = -cosf(rot);
			joyMove_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;
		}
	}
	if (direction_ == true) {
		if (acceleration > 0.0f) {
			acceleration -= 0.01f;
			rot -= 0.08f;
			joyMove_.x = +cosf(rot);
			joyMove_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;
		}
	}

	if (move_.y != 0 || move_.z != 0) {
		// worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
	if (direction_ == false) {
		worldTransform_.rotation_.y -= rotationSpeed_;
	}

	if (direction_ == true) {
		worldTransform_.rotation_.y += rotationSpeed_;
	}
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// ImGui::Begin("rotation_");
	// ImGui::InputInt("Direction", &direction_);
	//// ImGui::InputInt("RotationSpeed", &rotationSpeed_);
	// ImGui::End();

	// ImGui::Begin("Player");
	///*float Position[3] = {
	//    worldTransform_.translation_.x, worldTransform_.translation_.y,
	//    worldTransform_.translation_.z};

	// ImGui::SliderFloat3("Player Translation", Position, -65.0f, 65.0f);
	// ImGui::End();*/

	ImGui::Begin("rotationNum");
	ImGui::InputFloat("Num", &rotationNum_);

	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

const WorldTransform& Player::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します
	return worldTransform_;
}

void Player::falling() {

	if (worldTransform_.translation_.x >= 63 || worldTransform_.translation_.x <= -61 ||
	    worldTransform_.translation_.z >= 61 || worldTransform_.translation_.z <= -63) {
		worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	}
}

void Player::OnCollision(Base* other) {

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
}
