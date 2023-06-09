#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "DebugText.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 衝突処理 (プレイヤーと敵)
	void CollisionPlayerEnemy();
	//衝突処理 (ビームと敵)
	void CollisionBeamEnemy();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//各クラス
	Stage* stage_ = nullptr;      //ステージ
	Player* player_ = nullptr;    //プレイヤー
	Beam* beam_ = nullptr;      //ビーム
	Enemy* enemy_ = nullptr;    //敵
	DebugText* debugText_ = nullptr;
	//ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	int gameScore_ = 0;
	int playerLife_ = 3;
};
