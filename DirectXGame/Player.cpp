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
	if (input_->PushKey(DIK_R)) {
		worldTransform_.translation_ = {0.0f, 0.0f, -30.0f};
		move_ = {0, 0, 0};
		keyMove_ = {0, 0, 0};
		acceleration = 0.0f;
		rot = 0.0f;
	}

	if (input_->TriggerKey(DIK_SPACE) && direction_ == 0) {
		direction_ = 1;
	} else if (input_->TriggerKey(DIK_SPACE) && direction_ == 1) {
		direction_ = 0;
	}
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
		// rotationZMatrix));
		 rotation = MakeRotateYmatrix(rot);

	// 移動量に速さを反映(θ度の移動ベクトル)
	// rotation = (viewProjection_->rotation_.y);

	 move_ = Transform(move_, rotationYMatrix);
	move_ = Transform(keyMove_, rotation);

	// 移動量に速さを反映
	move_ = Multiply(speed + acceleration, Normalize(keyMove_));

	if (direction_ == false) {
		if (acceleration > 0.0f) {
			acceleration -= outRation;
			rot -= inRation;
			keyMove_.x = -cosf(rot);
			keyMove_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;
		}
	}
	if (direction_ == true) {
		if (acceleration > 0.0f) {
			acceleration -= outRation;
			rot -= inRation;
			keyMove_.x = +cosf(rot);
			keyMove_.z = -sinf(rot);
			rotationSpeed_ -= 0.01f;
		}
	}

	//move_ = Transform(joyMove_, rotation);

	// // 移動量に速さを反映
	// move_ = Multiply(speed + acceleration, Normalize(joyMove_));
	// if (direction_ == 0) {

	//	if (acceleration > 0.0f) {

	//		acceleration -= 0.01f;
	//		rot -= 0.08f;
	//		joyMove_.x = -cosf(rot);
	//		joyMove_.z = -sinf(rot);
	//		rotationSpeed_ -= 0.01f;
	//	}
	// }
	// if (direction_ == 1) {
	//	if (acceleration > 0.0f) {
	//		acceleration -= 0.01f;
	//		rot -= 0.08f;
	//		joyMove_.x = +cosf(rot);
	//		joyMove_.z = -sinf(rot);
	//		rotationSpeed_ -= 0.01f;
	//	}
	// }

	

	
	if (direction_ == false) {
		worldTransform_.rotation_.y -= rotationSpeed_;
	}
	if (direction_ == true) {
		worldTransform_.rotation_.y += rotationSpeed_;
	}

	float move[3]{move_.x, move_.y, move_.z};
	float keyMove[3]{keyMove_.x, keyMove_.y, keyMove_.z};
	float joyMove[3]{joyMove_.x, joyMove_.y, joyMove_.z};
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	ImGui::Begin("Rotation");
	ImGui::Checkbox(" Direction \n false = Right \n true = Left", &direction_);
	ImGui::InputFloat3("move", move);
	ImGui::InputFloat3("joyMove", joyMove);
	ImGui::InputFloat3("KeyMove", keyMove);

	  move_.x = move[0];
	  move_.y = move[1];
	  move_.z = move[2];

	  keyMove_.x = keyMove[0];
	  keyMove_.y = keyMove[1];
	  keyMove_.z = keyMove[2];

	  joyMove_.x = joyMove[0];
	  joyMove_.y = joyMove[1];
	  joyMove_.z = joyMove[2];

	// ImGui::InputInt("RotationSpeed", &rotationSpeed_);
	ImGui::End();

	
	ImGui::Begin("RotationSpeed");
	ImGui::InputFloat("Rot", &inRation, 0.01f);
	ImGui::InputFloat("Acceleration", &outRation, 0.01f);
	ImGui::End();

	// ImGui::Begin("Player");
	/*float Position[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	ImGui::SliderFloat3("Player Translation", Position, -65.0f, 65.0f);
	ImGui::End();*/
}

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
	// 反射角
	rot = -(radian + 3.14f / 4);
	acceleration = 1.0f;
	rotationSpeed_ = 0.1f;
}
