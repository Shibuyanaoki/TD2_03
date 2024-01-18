#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <math.h>
#include <cassert>
#include <AxisIndicator.h>

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
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	// スカイドームのモデル
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));

	//プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	//敵の生成と初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(modelEnemy_.get());

	// 追従カメラの生成と初期化処理
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	//地面の生成と初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	// スカイドームの生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	// 自キャラのワールドトランスフォームを追従カメラのセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// 自キャラのビュープロジェクションに追従カメラのビュープロジェクションをセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() { 

	// カメラの向きと自機の向きをそろえる
	//player_->SetViewRotate(followCamera_->GetViewRotate());

	player_->Update();

	followCamera_->Update();

	viewProjection_.UpdateViewMatrix();

	skydome_->Update();

	ground_->Update();

	OnCollisions();

	if (input_->TriggerKey(DIK_LSHIFT) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_LSHIFT) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

	// カメラ処理
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		followCamera_->Update();

		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

		
		viewProjection_.TransferMatrix();
	}
	ImGui::Begin("Collision");
	ImGui::InputInt("CollisionTime", &collisionTime_);
	ImGui::InputInt("CollisionFlag", &collisionFlag_);
	ImGui::End();

	ImGui::Begin("Camera");
	float positionTranslation[3] = {
	    viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z};

	float positionRotation[3] = {
	    viewProjection_.rotation_.x, viewProjection_.rotation_.y,
		viewProjection_.rotation_.z};


	ImGui::SliderFloat3("Camera Translation", positionTranslation, -65.0f, 65.0f);
	ImGui::SliderFloat3("Camera Rotation", positionRotation, -5.0f, 5.0f);

	viewProjection_.translation_.x = positionTranslation[0];
	viewProjection_.translation_.y = positionTranslation[1];
	viewProjection_.translation_.z = positionTranslation[2];

	viewProjection_.rotation_.x = positionRotation[0];
	viewProjection_.rotation_.y = positionRotation[1];
	viewProjection_.rotation_.z = positionRotation[2];

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

	player_->Draw(viewProjection_,outFlag);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

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

void GameScene::OnCollisions() {
	float dx = player_->GetWorldPosition().x - enemy_->GetWorldPosition().x;
	float dz = player_->GetWorldPosition().z - enemy_->GetWorldPosition().z;
	float dy = player_->GetWorldPosition().y - enemy_->GetWorldPosition().y;
	float dist = dx * dx + dy * dy + dz * dz;
	dist = sqrtf(dist);
	// 4 = 二つの円の半径足したもの
	if (dist <= 4) {
		//outFlag = true;
		hitFlag = true;
		timeFlag = true;
		if (collisionFlag_ == 1) {
			player_->OnCollision(enemy_.get());
			collisionFlag_ = 0;
		}
	} else {
		outFlag = false;
		
	}

	if (collisionFlag_ == 0) {
		collisionTime_++;
	}
	if (collisionTime_ >= 60) {
		collisionFlag_ = 1;
		collisionTime_ = 0;
	}
	//Base* base = player_.get();
	//base->GetWorldPosition();		// playerのGetWorldPosition()
	//player_->GetWorldPosition();

	//base->OnCollision(enemy_.get());

	

	 if (timeFlag) {
		time++;
		if(time >= 60) {
			resetFlag();
		}
	 }

}

void GameScene::resetFlag() {
	 timeFlag = false;
	 hitFlag = false;
	 outFlag = false;
	 time = 0;
}
