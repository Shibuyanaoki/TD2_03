#pragma once

#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {

	public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary
	FollowCamera();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FollowCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//カメラの角度取得
	Vector3 GetViewRotate() { return viewProjection_.rotation_; }

	void SceneEndResetPosition();

	/// <summary>
	/// プロジェクション行列計算用のメンバ設定関数群
	/// </summary>
	void SetFovAngleY(float value) { viewProjection_.fovAngleY = value; }
	void SetAspectRatio(float value) { viewProjection_.aspectRatio = value; }
	void SetNearZ(float value) { viewProjection_.nearZ = value; }
	void SetFarZ(float value) { viewProjection_.farZ = value; }

	private:
	Input* input_ = nullptr;

	// ビューポートプロジェクション
	ViewProjection viewProjection_;

	bool isDebugCameraActive_ = false;
	// 追従対象
	const WorldTransform* target_ = nullptr;

	// ゲームパッドの状態を得る変数
	//XINPUT_STATE joyState;

	// 追従対象からカメラまでのオフセット
	Vector3 offset = {0.0f, 0.0f, 30.0f};
	Vector3 move = {0.0f, 0.0f, 0.0f};

};
