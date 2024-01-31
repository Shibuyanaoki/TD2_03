﻿#include "Item.h"
#include <cassert>

void Item::Initialize(Model* model,Vector3 position) {
	worldTransform_.Initialize();
	assert(model);

	worldTransform_.translation_ = position;

	model_ = model;

}

void Item::Update() {}

void Item::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

//const WorldTransform& Item::GetWorldPosition() { return worldTransform_; }



