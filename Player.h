#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"
class Player 
{
public:
	//コンストラクタ
	Player();

	//デストラクタ
	~Player();

	//初期化
	void Initialize(ViewProjection viewProjection);
	
	//更新
	void Update();

	//3D背景描画
	void Draw3D();

	private:

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//インプットクラス
	Input* input_ = nullptr;
};
