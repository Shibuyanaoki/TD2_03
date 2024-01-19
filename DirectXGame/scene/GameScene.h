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
#include "Item.h"
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

	/// <summary>
	/// ポイントアイテム発生データを読み込み
	/// </summary>
	void LoadPointPopData();

	/// <summary>
	/// ポイントアイテム発生コマンドの更新
	/// </summary>
	void UpdataPointPopCommands();

	/// <summary>
	/// ポイントアイテムの生成
	/// </summary>
	/// <param name="position"></param>
	void PointGenerate(Vector3 position);

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
	// アイテムの3Dモデル
	std::unique_ptr<Model> modelItem_;
	
	// 自キャラ
	std::unique_ptr<Player> player_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	//デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 地面
	std::unique_ptr<Ground> ground_;
	//スカイドーム
	std::unique_ptr<Skydome> skydome_;
	// アイテム
	std::unique_ptr<Item> item_;
	// 　ポイントアイテム発生コマンド
	std::stringstream pointPopCommnds;
	//敵
	std::unique_ptr<Enemy> enemy_;
	// 敵
	std::list<std::unique_ptr<Enemy>> enemys_;
	// 敵発生コマンド
	std::stringstream enemyPopCommnds;


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
