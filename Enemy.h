// Enemy.h

#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Enemy {
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化
	void Initialize(ViewProjection viewProjection);

	// 更新
	void Update();

	float GetX() { return worldTransformEnemy_.translation_.x; }
	float GetZ() { return worldTransformEnemy_.translation_.z; }
	float GetFlag() { return aliveFlag_; }
	// 3D背景描画
	void Draw3D();

	// 移動
	void Move();

	// 発生(発射)
	void Born();

	void Start();

	// 衝突処理
	void Hit() { aliveFlag_ = 0; }

private:
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	// 敵
	uint32_t textureHandEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	float aliveFlag_ = 0;
	float enemySpeed_ = 0;
};
