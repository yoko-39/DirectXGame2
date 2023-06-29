#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "DebugText.h"
#include "Input.h"
class Title 
{
public:

	//コンストラクタ
	Title();

	//デストラクタ
	~Title();

	//初期化
	void Initialize();

	// 更新
	int Update();

	//2D前景描画
	void Draw2DNear();

	private:

	// タイトル
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// エンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;
	int gameTimer_ = 0;
	// インプットクラス
	Input* input_ = nullptr;

};
