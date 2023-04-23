#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	 uint32_t textureHandle_ = 0;

	Sprite* sprite_ = nullptr;

	// モデルテクスチャーハンドル
	uint32_t modeltextureHandle_ = 0;

	// モデル
	Model* model_ = nullptr;

	// ワールドトランスフォム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewprojection_;

	// サウンドデータファイル
	uint32_t sonudDateHandle_ = 0;

	// 音声ハンドル
	uint32_t voiceHandle_ = 0;

	// デバッグテキスト
	float inputFloat[3] = {0, 0, 0};

	// デバッグカメラ
	DebugCamera* debugcamera_ = nullptr;
};
