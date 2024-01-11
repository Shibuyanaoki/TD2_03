#include "FollowCamera.h"
#include "ImGuiManager.h"
#include "MT.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {
	input_ = Input::GetInstance();
	viewProjection_.Initialize();
}

void FollowCamera::Update() { // ゲームパッドの状態を得る変数
	
	// 速さ
	const float rotation = 0.05f;
	const float translation = 0.5f;
	

	//カメラ視点
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_J)) {
		viewProjection_.rotation_.y -= rotation;
	} else if (input_->PushKey(DIK_L)) {
		viewProjection_.rotation_.y += rotation;
	}
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_I)) {
		viewProjection_.rotation_.x += rotation;
	} else if (input_->PushKey(DIK_K)) {
		viewProjection_.rotation_.x -= rotation;
	}


	//カメラ移動
	// 押した方向で移動ベクトルを変更（前後）
	if (input_->PushKey(DIK_UP)) {
		viewProjection_.translation_.z += translation;
	} else if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.translation_.z -= translation;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_RIGHT)) {
		viewProjection_.translation_.x += translation;
	} else if (input_->PushKey(DIK_LEFT)) {
		viewProjection_.translation_.x -= translation;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_SPACE)) {
		viewProjection_.translation_.y += translation;
	} else if (input_->PushKey(DIK_LSHIFT)) {
		viewProjection_.translation_.y -= translation;
	}
	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotation;
	}

	if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

	// 追従対象がいれば
	if (target_ && isDebugCameraActive_ == false) {
		// 追従対象からカメラまでのオフセット
		
		offset = {0.0f, 2.0f, -10.0f};

		Matrix4x4 rotationMatrix;

		rotationMatrix = MakeRotateYmatrix(viewProjection_.rotation_.y);

		offset = TransformNormal(offset, rotationMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}
	// カメラ自由移動
	if (isDebugCameraActive_ == true) {

		move = {0, 0, 0};
		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}

	viewProjection_.UpdateMatrix();

	ImGui::Begin("Follow Camera");
	ImGui::DragFloat3("Camera translation_", &viewProjection_.translation_.x, 0.01f);
	ImGui::DragFloat3("Camera Rotation", &viewProjection_.rotation_.x, 0.01f);
	ImGui::End();
}
void FollowCamera::SceneEndResetPosition() {
	viewProjection_.translation_ = {0.0f, 0.0f, 0.0f};
	viewProjection_.rotation_ = {0.0f, 0.0f, 0.0f};

	viewProjection_.UpdateMatrix();

}
