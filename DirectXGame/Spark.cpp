#include "Spark.h"

void Spark::Initialize(Model* model) {
	model_ = model;

	// X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// X,Y,Z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	//// X,Y,Z方向の平行移動を設定
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Spark::Update() { worldTransform_.UpdateMatrix(); }

void Spark::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
