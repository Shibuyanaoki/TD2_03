#include "TitleScene.h"
void TitleScene::Initialize() {

	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	bgmHandle_ = audio_->LoadWave("BGM/TitleBGM.mp3");
	isBGM_ = false;
	buttonSound_ = audio_->LoadWave("BGM/Button1.mp3");

	// 背景のスプライト
	textureHandle_ = TextureManager::Load("TitleAvoid.png");
	sprite_ = Sprite::Create(textureHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

}

void TitleScene::Update() {

	if (isBGM_ == false) {
		playBGM_ = audio_->PlayWave(bgmHandle_, true, 0.5f);
		isBGM_ = true;
	}

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			Sleep(1 * 300);
			isSceneEnd = true;
			audio_->PlayWave(buttonSound_);
		}
	}
}

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

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

void TitleScene::Reset() {
	audio_->StopWave(playBGM_);
	isBGM_ = false;
	isSceneEnd = false; }
