#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"
#include "player.h"

class Beam 
{
public:
	// コンストラクタ
	Beam();

	// デストラクタ
	~Beam();

	// 初期化
	void Initialize(ViewProjection viewProjection, Player* player);

	// 更新
	void Update();

	float GetX() { return worldTransformBeam_.translation_.x; }
	float GetZ() { return worldTransformBeam_.translation_.z; }
	float GetFlag() { return beamFlag_; }
 	// 3D背景描画
	void Draw3D();

	//移動
	void Move();

	//発生(発射)
	void Born();

	void Start();
	void Hit() { beamFlag_ = 0; 
	             worldTransformBeam_.translation_.z = 0; 
	           }

	private:
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	// インプットクラス
	Input* input_ = nullptr;

	//生存フラグ
	int aliveFlag_ = 0;
	float beamFlag_ = 0;


	//プレイヤー
	Player* player_ = nullptr;
};
