#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Item.h"
#include "Model.h"
#include "Particle.h"
#include "Player.h"
#include "Scene.h"
#include "Skydome.h"
#include "Spark.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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
	/// 敵の発生データを読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// アイテム発生データを読み込み
	/// </summary>
	void LoadItemPopData();

	/// <summary>
	/// 敵の発生コマンドの更新
	/// </summary>
	void UpdataEnemyPopCommands();

	/// <summary>
	/// アイテムの発生コマンドの更新
	/// </summary>
	void UpdataItemPopCommands();

	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <param name="position"></param>
	void EnemyGenerate(Vector3 position);

	/// <summary>
	/// アイテムの生成
	/// </summary>
	/// <param name="positioin"></param>
	void ItemGenerate(Vector3 position);

	void OnCollisions();

	void Load();

	void Reset();

	bool IsClearSceneEnd() { return isGameClearSceneEnd; }

	bool IsOverSceneEnd() { return isGameOverSceneEnd; }

	Scene NextSceneClear() { return Scene::GAMECLEAR; }

	Scene NextSceneOver() { return Scene::TITLE; }

	void ParticleReset();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// カメラのビュープロジェクション
	ViewProjection viewProjection_;

	Sprite* HPSprite_ = nullptr;

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

	std::unique_ptr<Model> modelParticle_;

	std::unique_ptr<Model> modelSpark_;

	// 自キャラ
	std::unique_ptr<Player> player_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 地面
	std::unique_ptr<Ground> ground_;
	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	// 敵
	std::unique_ptr<Enemy> enemys_[4];
	// 敵の発生コマンド
	std::stringstream enemyPopCommands;
	// アイテム
	std::list<std::unique_ptr<Item>> items_;
	// アイテムの発生コマンド
	std::stringstream itemPopCommands;

	std::unique_ptr<Particle> particle_;

	std::unique_ptr<Spark> spark_;

	bool isDebugCameraActive_ = false;

	// 当たり判定のフラグ
	bool enemyCollisionFlag_ = false;
	bool itemCollisionFlag_ = false;

	// 当たり判定タイマー
	int enemyTimer_ = 60;
	int itemTimer_ = 60;

	float radian = 0;

	int deadCount = 0;

	// シーンを終わらせるフラグ
	bool isGameOverSceneEnd = false;

	bool isGameClearSceneEnd = false;

	bool isGameOverScene = false;

	Sprite* gameOverSprite_ = nullptr;

	// サウンド
	uint32_t bgmHandle2_;
	uint32_t playBGM2_;
	bool isBGM2_;
	bool isGameOver_;

	uint32_t buttonSound_;

	// サウンド
	uint32_t bgmHandle_;
	uint32_t playBGM_;
	bool isBGM_;

	uint32_t sparkSE_[3];
	int randomSE_;

	float HP;

	Vector4 HPColor_ = {1.0f, 1.0f, 1.0f, 0.9f};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
