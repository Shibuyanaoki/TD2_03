#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <math.h>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();

	//プレイヤーのモデル	
	modelPlayer_.reset(Model::CreateFromOBJ("Player", true));
	// 敵のモデル
	modelEnemy_.reset(Model::CreateFromOBJ("Enemy", true));
	// 地面のモデル
	modelground_.reset(Model::CreateFromOBJ("ground", true));
	// スカイドームのモデル
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));

	//プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	//敵の生成と初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(modelEnemy_.get());

	// 追従カメラの生成と初期化処理
	/*followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();*/

	// デバッグカメラ
	// ビュープロジェクション行列の転送debugCamera_ = std::make_unique<DebugCamera>();

	//地面の生成と初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelground_.get());

	// スカイドームの生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	//float degree = degree * (M_PI)

	viewProjection_.translation_ = {0.0f,130.0f,-8.7f};

	viewProjection_.rotation_ = { 89.5f,0.0f, 0.0f};

}

void GameScene::Update() { 

	// カメラの向きと自機の向きをそろえる
	//player_->SetViewRotate(followCamera_->GetViewRotate());

	player_->Update();

	//followCamera_->Update();

	ground_->Update();

	skydome_->Update();

	//debugCamera_->Update();

	ImGui::Begin("Camera");
	float Position[3] = {
	    viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z};

	ImGui::SliderFloat3("Camera Translation", Position, -65.0f, 65.0f);

	viewProjection_.translation_.x = Position[0];
	viewProjection_.translation_.y = Position[1];
	viewProjection_.translation_.z = Position[2];

	ImGui::End();

	//// ビュープロジェクションの反映
	//viewProjection_.matView = followCamera_->GetViewProjection().matView;
	//viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	
	viewProjection_.UpdateMatrix();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
