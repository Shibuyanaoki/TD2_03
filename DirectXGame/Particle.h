#pragma once
#include "MT.h"
#include "Model.h"
#include "WorldTransform.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Particle {
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDelete() const { return isDelete_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	static const int32_t kLifeTime = 30;
	int32_t deathTimer_ = kLifeTime;
	bool isDelete_ = false;
	Vector3 velocity_;
	float speed;
	float random;
};