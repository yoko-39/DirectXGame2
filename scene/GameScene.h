#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
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

	/// <summary>
	/// プレイヤー更新
	/// </summary>
	void PlayerUpdate();

   /// <summary>
   /// 弾の更新
   /// </summary>
	void BeamUpdate();

   /// <summary>
   /// 弾の移動
   /// </summary>
	void BeamMove();

   /// <summary>
   ///	ビーム発生
   /// </summary>
	void BeamBorn();

   /// <summary>
   /// 敵更新
   /// </summary>
	void EnemyUpdate(); 
   
   /// <summary>
   /// 敵移動
   /// </summary>
	void EnemyMove();

   /// <summary>
   /// 敵発生
   /// </summary>
	void EnemyBorn();

   /// <summary>
   ///衝突判定 
   /// </summary>
	void Collision();

   /// <summary>
   /// 衝突判定（プレイヤーと敵)
   /// </summary>
	void CollisionPlayerEnemy();

    /// <summary>
    /// 衝突判定(ビームとの)
    /// </summary>
    void CollisionBeamEnemy();

	private:
    /// <summary>
    ///  ゲームプレイ更新
    /// </summary>
	void GamePlayUpdate();

    /// <summary>
    ///  ゲームプレイ3D表示
    /// </summary>
	void GamePlayDraw3d();

    /// <summary>
    ///  ゲームプレイ背景2D表示
    /// </summary>
	void GamePlayDraw2DBack();

	/// <summary>
    ///  ゲームプレイ近景2D表示
    /// </summary>
	void GamePlayDraw2DNear();

	/// <summary>
    /// タイトル更新
    /// </summary>
	void TitleUpdate();

	/// <summary>
    ///  タイトル2D
    /// </summary>
	void TitleDraw2DNear();

   /// <summary>
   /// 
   /// </summary>
	void GameOverDraw2DNear();

   /// <summary>
   /// 
   /// </summary>
	void GamePlayStart();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	///
	
	/// BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	//ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;
	   
	//プレイヤー
	uint32_t  textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_[5];
	int beamFlag_[10] = {};
	int beamTimer_ = 0;
	//敵
	uint32_t textureHandEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10];
	int enemyFlag_[10] = {};
	float enemyspeed_[10] = {}; 



	DebugText* debugText_ = nullptr;
	//ゲームスコア
	int gameScore_ = 0;
	//プレイヤーライフ
	int playerLife_ = 3;

	//シーンモード(0:ゲームプレイ 1:タイトル)
	int sceneMode_ = 1;

	//タイトル
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	int gameTimer_ = 0;
	//エンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;
	//ゲームオーバー
	uint32_t textureHandleOver_ = 0;
	Sprite* spriteOver_ = nullptr;

};
