#pragma once
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "DebugText.h"
#include "Input.h"

class GamePlay 
{
public:
	GamePlay();

	~GamePlay();

	void Initialize(ViewProjection viewProjection );

	int Update();

	void Draw2DFar();
	
	void Draw3D();
	
	void Draw2DNear();
	 
	void Start();

	void Shot();

	// 衝突処理 (プレイヤーと敵)
	void CollisionPlayerEnemy();
	// 衝突処理 (ビームと敵)
	void CollisionBeamEnemy();

	int GetFlag() { return sceneMode_; }
	private:

	// 各クラス
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beamTable_[20] = {}; // ビーム
	Enemy* enemyTable_[10] = {}; // 敵
	
	
	// インプットクラス
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;  
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	int gameScore_ = 0;
	int playerLife_ = 3;
	int sceneMode_ = 1;
	int shotTimer_ = 0;
};
