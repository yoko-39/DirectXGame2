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
#include "Audio.h"
class Option {
public:
	// コンストラクタ
	Option();

	// デストラクタ
	~Option();

	// 初期化
	void Initialize(ViewProjection viewProjection);

	void Start();

	// 更新
	int Update();

	void Draw2DNear();
	 
private:
	
	// インプットクラス
	Input* input_ = nullptr;
	// オプション
	uint32_t textureHandleOption_ = 0;
	Sprite* spriteOption_ = nullptr;
	/// サウンド
	Audio* audio_ = nullptr;
	uint32_t soundDatahandleBGM_ = 0;
	uint32_t voiceHandleBGM_ = 0;

};

