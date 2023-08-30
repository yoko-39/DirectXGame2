// Enemy.cpp

#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() { delete modelEnemy_; }

void Enemy::Initialize(ViewProjection viewProjection) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;
	// 敵
	textureHandEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	// 敵の数ループする
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
}

void Enemy::Update() {
	Jump();
	Move();
	Born();
	// 変更行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformEnemy_.TransferMatrix();
}

void Enemy::Born() {
	if (rand() % 8 == 0) {
		if (aliveFlag_ == 0) {
			aliveFlag_ = 1;
			worldTransformEnemy_.translation_.y = 0;
			worldTransformEnemy_.translation_.z = 40.0f;

			int x = rand() % 100;
			int y = rand() % 40;
			float x2 = (float)x / 10 + -5.0f;
			float y2 = (float)y / 10 + -0.1f;
			worldTransformEnemy_.translation_.x = x2;
			worldTransformEnemy_.translation_.y = y2;

			if (rand() % 2 == 0) {
				enemySpeed_ = 0.1f;
			} else {
				enemySpeed_ = -0.1f;
			}
		}
	}
}

void Enemy::Start() { 
	aliveFlag_ = 0; 
	worldTransformEnemy_.translation_.y = 0;

}

void Enemy::Jump() { 
	if (aliveFlag_ == 2) {
		worldTransformEnemy_.translation_.y += JumpSpeed_;
	//速度を減らす
		JumpSpeed_ -= 0.1f;
		worldTransformEnemy_.translation_.x += enemySpeed_ * 4;

		if (worldTransformEnemy_.translation_.y < -3) {
			aliveFlag_ = 0;
		}

	}
}

void Enemy::Move() {
	if (aliveFlag_ == 1) {
		worldTransformEnemy_.translation_.z -= 0.4f;
		
		if (worldTransformEnemy_.translation_.z < -5.0f) {
			aliveFlag_ = 0;
		}
	}
}

void Enemy::Draw3D() {
	if (aliveFlag_ != 0) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandEnemy_);
	}
}
