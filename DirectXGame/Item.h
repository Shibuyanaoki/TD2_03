#pragma once
#include "Base.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Item : public Base {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:

	// モデル
	Model* model_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	

};
