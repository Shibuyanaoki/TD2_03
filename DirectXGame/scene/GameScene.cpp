#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <AxisIndicator.h>
#include <cassert>
#include <fstream>
#include <math.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();

	// プレイヤーのモデル
	modelPlayer_.reset(Model::CreateFromOBJ("Player", true));
	// 敵のモデル
	modelEnemy_.reset(Model::CreateFromOBJ("Enemy", true));
	// 地面のモデル
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	// スカイドームのモデル
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// アイテムのモデル
	modelItem_.reset(Model::CreateFromOBJ("Bard", true));

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	// 敵の生成と初期化
	// enemy_ = std::make_unique<Enemy>();
	// enemy_->Initialize(modelEnemy_.get());

	//// アイテムの生成と初期化
	// item_ = std::make_unique<Item>();
	// item_->Initialize(modelItem_.get(),);

	// 追従カメラの生成と初期化処理
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 地面の生成と初期化
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

	viewProjection_.translation_ = {0.0f, 150.0f, -8.7f};

	viewProjection_.rotation_ = {89.5f, 0.0f, 0.0f};

	// 敵のCSVファイル読み込み
	LoadEnemyPopData();

	// アイテムのCSVファイル読み込み
	LoadItemPopData();
}

void GameScene::Update() {

	// カメラの向きと自機の向きをそろえる
	// player_->SetViewRotate(followCamera_->GetViewRotate());

	player_->Update();

	followCamera_->Update();

	viewProjection_.UpdateViewMatrix();

	skydome_->Update();

	ground_->Update();

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

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
	/*ImGui::InputInt("CollisionTime", &collisionTime_);
	ImGui::InputInt("CollisionFlag", &collisionFlag_);*/
	ImGui::End();

	ImGui::Begin("Camera");
	float positionTranslation[3] = {
	    viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z};

	float positionRotation[3] = {
	    viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z};

	ImGui::SliderFloat3("Camera Translation", positionTranslation, -65.0f, 65.0f);
	ImGui::SliderFloat3("Camera Rotation", positionRotation, 0.0f, 1.0f);

	viewProjection_.translation_.x = positionTranslation[0];
	viewProjection_.translation_.y = positionTranslation[1];
	viewProjection_.translation_.z = positionTranslation[2];

	viewProjection_.rotation_.x = positionRotation[0];
	viewProjection_.rotation_.y = positionRotation[1];
	viewProjection_.rotation_.z = positionRotation[2];

	ImGui::End();

	//// ビュープロジェクションの反映
	// viewProjection_.matView = followCamera_->GetViewProjection().matView;
	// viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// デスフラグの立ったアイテムを削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->IsDead()) {
			enemy.release();
			return true;
		}
		return false;
	});

	/*if (SceneEndTitle <= 0) {
	    isSceneEnd = true;
	}*/

	// 敵のCSVファイルの更新処理
	UpdataEnemyPopCommands();

	// アイテムのCSVファイルの更新処理
	UpdataItemPopCommands();

	viewProjection_.UpdateMatrix();

	if (player_->GetRotationNum() <= 0) {
		isSceneEnd = true;
	}
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

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (const std::unique_ptr<Item>& item : items_) {
		item->Draw(viewProjection_);
	}

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

void GameScene::LoadEnemyPopData() {
	enemyPopCommands.clear();
	std::ifstream file;
	file.open("Resources/EnemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::LoadItemPopData() {
	itemPopCommands.clear();
	std::ifstream file;
	file.open("Resources/ItemPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	itemPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdataEnemyPopCommands() {
	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyGenerate({x, y, z});
		}
	}
}

void GameScene::UpdataItemPopCommands() { // 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(itemPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			ItemGenerate({x, y, z});
		}
	}
}

void GameScene::EnemyGenerate(Vector3 position) {
	// 敵の生成と初期化処理
	Enemy* enemy = new Enemy();
	enemy->Initialize(modelEnemy_.get(), position);

	enemys_.push_back(static_cast<std::unique_ptr<Enemy>>(enemy));
}

void GameScene::ItemGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Item* item = new Item();
	item->Initialize(modelItem_.get(), position);

	items_.push_back(static_cast<std::unique_ptr<Item>>(item));
}

void GameScene::OnCollisions() {

#pragma region PlayerとEnemyの当たり判定

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		float dist = CollisionDetection(player_->GetWorldPosition(), enemy->GetWorldPosition());

		// 4 = 二つの円の半径足したもの
		if (dist <= 4 && enemyCollisionFlag_ == false) {

				player_->OnCollision(enemy.get());

				if (player_->GetDirection() == enemy->GetDirection()) {
					player_->SetRotationNum(-10.0f);
				}

				if (player_->GetDirection() != enemy->GetDirection()) {
					player_->SetRotationNum(10.0f);
				}

				enemyCollisionFlag_ = true;
		}
	}

	if (enemyCollisionFlag_ == true) {
		enemyTimer_--;

		if (enemyTimer_ <= 0) {
			    enemyCollisionFlag_ = false;
			    enemyTimer_ = 60;
		}

	}

#pragma endregion

#pragma region PlayerとItemの当たり判定
	for (const std::unique_ptr<Item>& item : items_) {

		float itemPlayDist =
		    CollisionDetection(player_->GetWorldPosition(), item->GetWorldPosition());

		if (itemPlayDist <= 4 && itemCollisionFlag_ == false) {
			if (player_->GetDirection() == false) {
				player_->SetSirection(true);

			} else if (player_->GetDirection() == true) {
				player_->SetSirection(false);
			}

			itemCollisionFlag_ = true;

		}
	}

	if (itemCollisionFlag_ == true) {
		itemTimer_--;

		if (itemTimer_ <= 0) {
			itemCollisionFlag_ = false;
			itemTimer_ = 60;
		}
	}

#pragma endregion

#pragma region EnemyとItemの当たり判定
	for (const std::unique_ptr<Item>& item : items_) {
		for (const std::unique_ptr<Enemy>& enemy : enemys_) {

			float itemEnemyDist =
			    CollisionDetection(item->GetWorldPosition(), enemy->GetWorldPosition());

			if (itemEnemyDist <= 4) {
				if (enemy->GetDirection() == false) {
					enemy->SetSirection(true);
				} else if (enemy->GetDirection() == true) {
					enemy->SetSirection(false);
				}
			}
		}
	}

#pragma endregion

}

void GameScene::resetFlag() {}
