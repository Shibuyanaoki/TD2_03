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

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

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

		// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			// 移動量
			joyMove_ = {
			    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, // Lスティックの横成分
			    0.0f,
			    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed, // Lスティックの縦成分
			};
		}
		// Matrix4x4 rotationXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotationYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
		// Matrix4x4 rotationZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
		// Matrix4x4 rotationXYZMatrix =Multiply(rotationXMatrix, Multiply(rotationYMatrix,
		// rotationZMatrix));
		Matrix4x4 rotation = MakeRotateYmatrix(rot);

		// 移動量に速さを反映(θ度の移動ベクトル)
		// rotation = (viewProjection_->rotation_.y);

		// move_ = Transform(move_, rotationYMatrix);
		move_ = Transform(keyMove_, rotation);
		move_ = Transform(joyMove_, rotation);

		// 移動量に速さを反映
		move_ = Multiply(speed + acceleration, Normalize(keyMove_));

		if (direction_ == 0) {
			if (acceleration > 0.0f) {
				acceleration -= 0.01f;
				rot -= 0.08f;
				keyMove_.x = -cosf(rot);
				keyMove_.z = -sinf(rot);
				rotationSpeed_ -= 0.01f;
			}
		}
		if (direction_ == 1) {
			if (acceleration > 0.0f) {
				acceleration -= 0.01f;
				rot -= 0.08f;
				keyMove_.x = +cosf(rot);
				keyMove_.z = -sinf(rot);
				rotationSpeed_ -= 0.01f;
			}
		}

		// 移動量に速さを反映
		move_ = Multiply(speed + acceleration, Normalize(joyMove_));

		if (direction_ == 0) {
			if (acceleration > 0.0f) {
				acceleration -= 0.01f;
				rot -= 0.08f;
				joyMove_.x = -cosf(rot);
				joyMove_.z = -sinf(rot);
				rotationSpeed_ -= 0.01f;
			}
		}
		if (direction_ == 1) {
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
		if (direction_ == 0) {
			worldTransform_.rotation_.y -= rotationSpeed_;
		}
		if (direction_ == 1) {
			worldTransform_.rotation_.y += rotationSpeed_;
		}
		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
		// 行列を定数バッファに転送
		worldTransform_.UpdateMatrix();

		ImGui::Begin("rotation_");
		ImGui::InputInt("Direction", &direction_);
		// ImGui::InputInt("RotationSpeed", &rotationSpeed_);
		ImGui::End();

		//ImGui::Begin("Player");
		/*float Position[3] = {
		    worldTransform_.translation_.x, worldTransform_.translation_.y,
		    worldTransform_.translation_.z};

		ImGui::SliderFloat3("Player Translation", Position, -65.0f, 65.0f);
		ImGui::End();*/
	}
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
//
// Vector3 Player::GetWorldPosition() {
//	Vector3 worldPos;
//
//	worldPos.x = worldTransform_.matWorld_.m[3][0];
//	worldPos.y = worldTransform_.matWorld_.m[3][1];
//	worldPos.z = worldTransform_.matWorld_.m[3][2];
//
//	return worldPos;
//}
