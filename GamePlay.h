#pragma once
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "DebugText.h"

class GamePlay 
{
public:
	GamePlay();

	~GamePlay();

	void Initialize(ViewProjection viewProjection);

	void Update();

	void Draw2DFar();
	
	void Draw3D();
	
	void Draw2DNear();
	 
	// 衝突処理 (プレイヤーと敵)
	void CollisionPlayerEnemy();
	// 衝突処理 (ビームと敵)
	void CollisionBeamEnemy();

	private:

	// 各クラス
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beam_ = nullptr;     // ビーム
	Enemy* enemy_ = nullptr;   // 敵
	DebugText* debugText_ = nullptr;
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	int gameScore_ = 0;
	int playerLife_ = 3;
};
