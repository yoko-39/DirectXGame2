#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "GamePlay.h"
#include "Title.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Option.h"

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



private: 
	
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Title* title_ = nullptr; // タイトル
	GameOver* gameOver_ = nullptr; 
	GamePlay* gamePlay_ = nullptr;
	GameClear* gameclear_ = nullptr;
	Option* option_ = nullptr;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	int sceneMode_ = 1;
};
