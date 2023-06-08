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

void Enemy::Update() {
	    // 変更行列を更新
	    worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	        worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	        worldTransformEnemy_.translation_);

	    // 変更行列を定数バッファーに転送
	    worldTransformEnemy_.TransferMatrix();
}

void Enemy::Draw3D() { 
	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandEnemy_);
}
