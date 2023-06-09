#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	delete modelEnemy_;
}

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

void Enemy::Move() { 
	if (aliveFlag_ == 1) {
		
		    worldTransformEnemy_.translation_.z -= 0.5f;
		    worldTransformEnemy_.rotation_.x -= 0.1f;
			if (worldTransformEnemy_.translation_.z < -5.0f) {
			    aliveFlag_ = 0;
		    }

	}
}

void Enemy::Born() {
	if (rand() % 10 == 0) {
		    if (aliveFlag_ == 0) {
			    aliveFlag_ = 1;
			    worldTransformEnemy_.translation_.y = 0;
			    worldTransformEnemy_.translation_.z = 40.0f;

			    int x = rand() % 80;
			    float x2 = (float)x / 10 + -4;
			    worldTransformEnemy_.translation_.x = x2;
		    }
	}
}
	void Enemy::Update() {

	 Move();
	 Born();
	    // 変更行列を更新
	    worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	        worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	        worldTransformEnemy_.translation_);

	    // 変更行列を定数バッファーに転送
	    worldTransformEnemy_.TransferMatrix();
}

void Enemy::Draw3D() { 
	if (aliveFlag_ == 1) {
	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandEnemy_);
	    }
}
