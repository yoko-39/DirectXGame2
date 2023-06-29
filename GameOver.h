#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Stage.h"
#include "Beam.h"
#include "Enemy.h"
#include "Player.h"
#include "DebugText.h"
#include "Input.h"

class GameOver 
{
public:

	GameOver();
	
	~GameOver();
	
	void Initialize(ViewProjection viewProjection);
	
	void Update();

	void Draw2DNear();

	private:
		// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	// エンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;
	int gameTimer_ = 0;
	// ゲームオーバー
	uint32_t textureHandleOver_ = 0;
	Sprite* spriteOver_ = nullptr;
	// インプットクラス
	Input* input_ = nullptr;
	
};
