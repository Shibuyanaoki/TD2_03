#include "Spark.h"

void Spark::Initialize(Model* model) {
	model_ = model;

	// X,Y,Z�����̃X�P�[�����O��ݒ�
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// X,Y,Z�����̉�]��ݒ�
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	//// X,Y,Z�����̕��s�ړ���ݒ�
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
}

void Spark::Update() { worldTransform_.UpdateMatrix(); }

void Spark::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
