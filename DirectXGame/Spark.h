#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Spark {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
};
