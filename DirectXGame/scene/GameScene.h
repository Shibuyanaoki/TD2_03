#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "Ground.h"
#include "ImGuiManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void OnCollisions();

	void resetFlag();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// カメラのビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラの3Dモデル
	std::unique_ptr<Model> modelPlayer_;
	// 敵の3Dモデル
	std::unique_ptr<Model> modelEnemy_;
	// Skydomeの3Dモデル
	std::unique_ptr<Model> modelSkydome_;
	// 地面の3Dモデル
	std::unique_ptr<Model> modelGround_;
	
	// 自キャラ
	std::unique_ptr<Player> player_;
	//敵
	std::unique_ptr<Enemy> enemy_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	//デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 地面
	std::unique_ptr<Ground> ground_;
	//スカイドーム
	std::unique_ptr<Skydome> skydome_;

	bool isDebugCameraActive_ = false;

	int collisionFlag_ = 1;
	int collisionTime_ = 0;

	bool outFlag = false;
	bool hitFlag = false;

	int timeFlag = false;
	int time = 0;

	float radian = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
