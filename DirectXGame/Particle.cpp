#include "Particle.h"

void Particle::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {position.x, position.y, position.z};
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	velocity_ = {0.4f};
	speed = 0.01f;
}

void Particle::Update() {
	if (--deathTimer_ <= 0) {
		isDelete_ = true;
	}

	/*Vector3 move = {0.0f, 0.01f, speed*10};
	move = TransformNormal(move, worldTransform_.matWorld_);
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);*/

	worldTransform_.UpdateMatrix();
}

void Particle::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
