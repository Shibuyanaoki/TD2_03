#pragma once
#include "WorldTransform.h"

class Base {

public:
	virtual void OnCollision(Base* other);

protected:
	WorldTransform worldTransform_;

public:

	Vector3 GetWorldPosition() {
		Vector3 worldPos;

		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	}

};
