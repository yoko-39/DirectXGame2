#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"
#include "Enemy.h"


class EnemyBeam {
public:
	// コンストラクタ
	EnemyBeam();

	// デストラクタ
	~EnemyBeam();

	// 初期化
	void Initialize(ViewProjection viewProjection, Enemy* enemy);

	// 更新
	void Update();

	float GetX() { return worldTransformEBeam_.translation_.x; }
	float GetZ() { return worldTransformEBeam_.translation_.z; }
	float GetY() { return worldTransformEBeam_.translation_.y; }
	float GetFlag() { return EbeamFlag_; }
	// 3D背景描画
	void Draw3D();

	// 移動
	void Move();

	// 発生(発射)
	void Born();

	void Start();

	void Hit() {
		EbeamFlag_ = 0;
		worldTransformEBeam_.translation_.z = -5;
	}

private:
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// ビーム
	uint32_t textureHandleEBeam_ = 0;
	Model* modelEBeam_ = nullptr;
	WorldTransform worldTransformEBeam_;

	// インプットクラス
	Input* input_ = nullptr;

	// 生存フラグ
	int EaliveFlag_ = 0;
	float EbeamFlag_ = 0;

	// 敵
	Enemy* enemy_ = nullptr;

	
};




