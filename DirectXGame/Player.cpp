#include "Player.h"
#include "ImGuiManager.h"

#include "MT.h"
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
	worldTransform_.translation_ = position_;
}

void Player::Update() {

	Reverse();

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		//// 速さ
		const float speed = 0.3f;

		//if (input_->PushKey(DIK_W)) {
		//	keyMove_.z += speed;
		//} else if (input_->PushKey(DIK_S)) {
		//	keyMove_.z -= speed;
		//}
		//// 押した方向で移動ベクトルを変更（左右）
		//if (input_->PushKey(DIK_A)) {
		//	keyMove_.x -= speed;
		//} else if (input_->PushKey(DIK_D)) {
		//	keyMove_.x += speed;
		//}

		// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			// 移動量
			joyMove = {
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

		// move_ = Transform(move_, rotationYMatrix);
		move_ = Transform(joyMove, rotation);

		// 移動量に速さを反映
		move_ = Multiply(speed + acceleration, Normalize(keyMove_));

		// 移動量に速さを反映
		move_ = Multiply(speed + acceleration, Normalize(joyMove));

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
		if (acceleration > 0.0f) {
			acceleration -= 0.05f;
			rot -= 0.10f;
			joyMove.x = -cosf(rot);
			joyMove.z = -sinf(rot);
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

		if (move_.y != 0 || move_.z != 0) {
			// worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}
		worldTransform_.rotation_.y += 0.02f;

		falling();

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
		// 行列を定数バッファに転送
		worldTransform_.UpdateMatrix();
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
	 
	/*Vector3 Player::GetWorldPosition() {
	    Vector3 worldPos;

	    worldPos.x = worldTransform_.matWorld_.m[3][0];
	    worldPos.y = worldTransform_.matWorld_.m[3][1];
	    worldPos.z = worldTransform_.matWorld_.m[3][2];

	    return worldPos;
	}*/

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
