#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"
class Enemy 
{
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化
	void Initialize(ViewProjection viewProjection);

	// 更新
	void Update();

	// 3D背景描画
	void Draw3D();

	//移動
	void Move();

	//発生(発射)
	void Born();

private:
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	// 敵
	uint32_t textureHandEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	int aliveFlag_ = 0;
	float enemySpeed_ = 2; 
	
};
