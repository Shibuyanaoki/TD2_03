#include "Audio.h"
#include "AxisIndicator.h"
#include "ClearScene.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "Scene.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "WinApp.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	ClearScene* clearScene = nullptr;
	GameOver* gameOverScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// タイトルシーンの初期化
	titleScene = new TitleScene();
	titleScene->Initialize();

	// クリアシーンの初期化
	clearScene = new ClearScene;
	clearScene->Initialize();

	//ゲームオーバーシーンの初期化
	gameOverScene = new GameOver;
	gameOverScene->Initialize();

	Scene scene = Scene::GAME;

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		switch (scene) {
		case Scene::TITLE:

			if (titleScene->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = titleScene->NextScene();
				titleScene->Reset();
			}

			// タイトルシーンの毎フレーム処理
			titleScene->Update();

			break;

		case Scene::GAME:
			// ゲームシーンの毎フレーム処理
			gameScene->Update();

			if (gameScene->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = gameScene->NextScene();
				gameScene->Reset();
			} else if (gameScene->IsGameOverScene() == true) {
				scene = gameScene->GameOver();
				gameScene->Reset();
			}

			break;

		case Scene::GAMECLEAR:

			if (clearScene->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = clearScene->NextScene();
				clearScene->Reset();
			}

			// リザルトシーンの毎フレーム処理
			clearScene->Update();

			break;
		case Scene::GAMEOVER:

			if (gameOverScene->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = gameOverScene->NextScene();
				gameOverScene->Reset();
			}

			// リザルトシーンの毎フレーム処理
			gameOverScene->Update();

			break;
		}

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		switch (scene) {
		case Scene::TITLE:
			// タイトルシーンの描画
			titleScene->Draw();

			break;

		case Scene::GAME:
			// ゲームシーンの描画
			gameScene->Draw();

			break;
		case Scene::GAMECLEAR:
			// リザルトシーンの描画
			clearScene->Draw();

			break;
		case Scene::GAMEOVER:
			// リザルトシーンの描画
			gameOverScene->Draw();

			break;
		}

		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}