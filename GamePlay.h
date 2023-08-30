#pragma once
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "EnemyBeam.h"
#include "DebugText.h"
#include "Input.h"
#include "Audio.h"



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

	void DrawScore();

	// 衝突処理 (プレイヤーと敵)
	void CollisionPlayerEnemy();
	// 衝突処理 (ビームと敵)
	void CollisionBeamEnemy();

	void CollisionEnemyBeamPlayer();

	int GetFlag() { return sceneMode_; }
	private:

	// 各クラス
	Stage* stage_ = nullptr; // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beamTable_[20] = {}; // ビーム
	Enemy* enemyTable_[15] = {}; // 敵
	EnemyBeam* enemybeamTable_[20] = {};
	// スコア
	uint32_t textureHandleScore_ = 0;
	Sprite* spriteScore_ = nullptr;
	// サウンド
	Audio* audio_ = nullptr;
	uint32_t soundDatahandleBGM_ = 0;
	uint32_t voiceHandleBGM_ = 0;
	// インプットクラス
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;  
	//敵ヒットSE
	uint32_t soundDateHandleEnemySE_ = 0;
	//プレイヤーヒットSE
	uint32_t soundDateHandlePlayerSE_ = 0;
	//スコア数値(スプライト)
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[5] = {};
	// ライフ
	uint32_t textureHandleLife_ = 0;
	Sprite* spriteLife_[3] = {};
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	int gameScore_ = 0;
	int playerLife_ = 3;
	int sceneMode_ = 1;
	int shotTimer_ = 0;
	int gameTimer_ = 0;
	int playerTimer_ = 0;

};
