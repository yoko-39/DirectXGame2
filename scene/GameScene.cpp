#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"
#include "time.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete spriteBG_; 
	delete modelStage_;
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
	delete spriteTitle_;
	delete spriteEnter_;
	for (int k = 0; k < 5; k++) {
		delete spriteNumber_[k];
	}
	delete spriteScore_;
	for (int l = 0; l < 3; l++) {
		delete spriteLife_[l];
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, { 0, 0 });

	//ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage2.jpg");
	modelStage_ = Model::Create();
	//ステージの数ループする
	for (int u = 0; u < 20; u++) {
		worldTransformStage_[u].Initialize();
	}
	//ステージの位置を変更
	// ステージの数ループする
	for (int u = 0; u < 20; u++) {
		worldTransformStage_[u].translation_ = {0, -1.5f, 2.0f * u - 5};
		worldTransformStage_[u].scale_ = {4.5f, 1, 1};

		// 変更行列を更新
		worldTransformStage_[u].matWorld_ = MakeAffineMatrix(
		    worldTransformStage_[u].scale_, worldTransformStage_[u].rotation_,
		    worldTransformStage_[u].translation_);

		// 変更行列を定数バッファーに転送
		worldTransformStage_[u].TransferMatrix();
	}
	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = { 0.5f, 0.5f, 0.5f };
	worldTransformPlayer_.Initialize();

	//弾
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	for (int j = 0; j < 5; j++) {
		worldTransformBeam_[j].scale_ = {0.3f, 0.3f, 0.3f};
		worldTransformBeam_[j].Initialize();
	}
	// 敵
	textureHandEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	//敵の数ループする
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}
	//乱数調整
	srand((unsigned int)time(NULL));

	//デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

	//タイトル
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0,0});
	//エンター
	textureHandleEnter_ = TextureManager::Load("Enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});
	//ゲームオーバー
	textureHandleOver_ = TextureManager::Load("gameover.png");
	spriteOver_ = Sprite::Create(textureHandleOver_, {0, 0});

   //サウンドデータの読み込み
	soundDateHandleTitleBGM_ = audio_->LoadWave("Ring05.wav");
	soundDateHandleGamePlayBGM_ = audio_->LoadWave("Ring08.wav");
	soundDateHandleGameOverBGM_ = audio_->LoadWave("Ring09.wav");
	soundDateHandleEnemyHitSE_ = audio_->LoadWave("chord.wav");
	soundDateHandlePlayerHitSE_ = audio_->LoadWave("tada.wav");

	//タイトルBGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDateHandleTitleBGM_, true);
  
	//スコア数値(2Dスプライト)
	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int k = 0; k < 5; k++) {
		spriteNumber_[k] = Sprite::Create(textureHandleNumber_, {300.0f + k * 26, 0});
		spriteNumber_[k] = Sprite::Create(textureHandleNumber_, {320.0f + k * 26, 0});
		spriteNumber_[k] = Sprite::Create(textureHandleNumber_, {340.0f + k * 26, 0});
		spriteNumber_[k] = Sprite::Create(textureHandleNumber_, {360.0f + k * 26, 0});
		spriteNumber_[k] = Sprite::Create(textureHandleNumber_, {380.0f + k * 26, 0});
	}

	//スコア
	textureHandleScore_ = TextureManager::Load("score.png");
	spriteScore_ = Sprite::Create(textureHandleScore_, {250.0f, 0});

	//ライフ
	textureHandleLife_ = TextureManager::Load("player.png");
	
	for (int l = 0; l < playerLife_; l++) {
		spriteLife_[l] = Sprite::Create(textureHandleLife_, {800.0f + l * 60, 0});
		spriteLife_[l]->SetSize({40, 40});
	}
}
    //**************************************************
//タイトル
//**************************************************
//タイトル更新
void GameScene::TitleUpdate() 
{ 
	//エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN))
	{
		GamePlayStart();
		//モードをゲームプレイに変更
		sceneMode_ = 0;

		//BGM切り替え
		audio_->StopWave(voiceHandleBGM_);//現在のBGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDateHandleGamePlayBGM_, true); //ゲームプレイBGMを再生
	}
}


//プレイヤー
void GameScene::PlayerUpdate() {

	//移動
	//右へ移動
	if (input_->PushKey(DIK_RIGHT))
	{
		worldTransformPlayer_.translation_.x += 0.1f;
	}

	if (input_->PushKey(DIK_LEFT))
	{
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	//変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
		worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
		worldTransformPlayer_.translation_);

	//変更行列を定数バッファーに転送
	worldTransformPlayer_.TransferMatrix();

	if (worldTransformPlayer_.translation_.x > 4) {
		worldTransformPlayer_.translation_.x = 4;
	}if (worldTransformPlayer_.translation_.x < -4) {
		worldTransformPlayer_.translation_.x = -4;
	}
	
	if (playerLife_ <= 0) {
		sceneMode_ = 2;
		audio_->StopWave(voiceHandleBGM_);
		voiceHandleBGM_ = audio_->PlayWave(soundDateHandleGameOverBGM_, true);
	}

	if (playerTimer_ > 0) {
		playerTimer_ -= 1;
	}
}

//ビーム
void GameScene::BeamUpdate()
{
	//移動
	BeamMove();
	//ビーム発生
	BeamBorn();

	// 変換行列を更新
	for (int j = 0; j < 5; j++) {
		worldTransformBeam_[j].matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_[j].scale_, worldTransformBeam_[j].rotation_,
		    worldTransformBeam_[j].translation_);

		// 変更行列を定数バッファーに転送
		worldTransformBeam_[j].TransferMatrix();
	}
}

void GameScene::BeamMove() {
	for (int j = 0; j < 5; j++) {
		if (beamFlag_[j] == 1) {
			worldTransformBeam_[j].translation_.z += 0.5;
			worldTransformBeam_[j].rotation_.x += 0.1f;
		}

		if (worldTransformBeam_[j].translation_.z > 40) {

			beamFlag_[j] = 0;
		}
	}
}

void GameScene::BeamBorn() {

	for (int j = 0; j < 5; j++) {
		if (beamFlag_[j] == 0) {
			if (input_->PushKey(DIK_SPACE)) {
				if (beamTimer_ == 0) {
					worldTransformBeam_[j].translation_.x = worldTransformPlayer_.translation_.x;
					worldTransformBeam_[j].translation_.y = worldTransformPlayer_.translation_.y;
					worldTransformBeam_[j].translation_.z = worldTransformPlayer_.translation_.z;
					beamFlag_[j] = 1;
					beamTimer_ = 1;
					break;
				} else {
					beamTimer_++;
					if (beamTimer_ > 10) {
						beamTimer_ = 0;
					}
				}
			}
		}
	}
}
//敵
void GameScene::EnemyUpdate() 
{
	EnemyMove();
	EnemyBorn();
	EnemyJump();
	// 変換行列を更新
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, 
			worldTransformEnemy_[i].rotation_,
		    worldTransformEnemy_[i].translation_);

		// 変更行列を定数バッファーに転送
		worldTransformEnemy_[i].TransferMatrix();
	}
}


void GameScene::EnemyBorn() {
	if (rand() % 10 == 0) {
		for (int i = 0; i < 10; i++) {
			if (enemyFlag_[i] == 0) {
				enemyFlag_[i] = 1;
				worldTransformEnemy_[i].translation_.y = 0;
				worldTransformEnemy_[i].translation_.z = 40.0f;

				int x = rand() % 80;
				float x2 = (float)x / 10 + -4;
				worldTransformEnemy_[i].translation_.x = x2;

				if (rand() % 2 == 0) {
					enemySpeed_[i] = 0.1f;
				} else {
					enemySpeed_[i] = -0.1f;
				}

				break;
			}
		}
	}
}


void GameScene::EnemyMove() 
{ 
    for (int i = 0; i < 10; i++) {
		
		if (enemyFlag_[i] == 1) {
			worldTransformEnemy_[i].translation_.x += enemySpeed_[i];
			worldTransformEnemy_[i].translation_.z -= 0.5f;
			worldTransformEnemy_[i].rotation_.x -= 0.1f;
			if (worldTransformEnemy_[i].translation_.z < -5.0f) {
				enemyFlag_[i] = 0;
			}

			if (worldTransformEnemy_[i].translation_.x > 4) {
				enemySpeed_[i] *= -1.0f;
			}
			if (worldTransformEnemy_[i].translation_.x < -4) {
				enemySpeed_[i] *= -1.0f;
			}
			//敵が手前に移動する
			//タイマ―により速度を設定
			worldTransformEnemy_[i].translation_.z -= -0.1f;
			worldTransformEnemy_[i].translation_.z -= gameTimer_ / 100.0f;
		}

	}
}



//衝突判定
void GameScene::Collision() 
{
	//衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();
	//衝突判定 (弾と敵)
	CollisionBeamEnemy();
	
}

void GameScene::CollisionPlayerEnemy() 
{
	for (int i = 0; i < 10; i++) {
		// 敵が存在すれば
		if (enemyFlag_[i] == 1) {
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);

			// 衝突したら
			if (dx < 1 && dz < 1) {
				playerLife_ -= 1;
				playerTimer_ = 60;
				// 存在しない
				enemyFlag_[i] = 0;

				//プレイヤ―ヒットSE
				audio_->PlayWave(soundDateHandlePlayerHitSE_);
			}
		}
	}
}

void GameScene::CollisionBeamEnemy() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			if (enemyFlag_[i] == 1 && beamFlag_[j] == 1) {
				float dx1 = abs(
				    worldTransformBeam_[j].translation_.x - worldTransformEnemy_[i].translation_.x);
				float dz1 = abs(
				    worldTransformBeam_[j].translation_.z - worldTransformEnemy_[i].translation_.z);

				if (dx1 < 1 && dz1 < 1) {
					enemyFlag_[i] = 2;
					beamFlag_[j] = 0;
					gameScore_ += 100;
					enemyJumpSpeed_[i] = 1;
					audio_->PlayWave(soundDateHandleEnemyHitSE_);

				}
			}
		}
	}
}
// 敵ジャンプ
void GameScene::EnemyJump() {
	// 敵でループ
	for (int i = 0; i < 10; i++) {
		// 消滅演出なら
		if (enemyFlag_[i] == 2) {
			// 移動（Y座標に速度を加える
			worldTransformEnemy_[i].translation_.y += enemyJumpSpeed_[i];
			// 速度を減らす
			enemyJumpSpeed_[i] -= 0.1f;
			// 斜め移動
			worldTransformEnemy_[i].translation_.x += enemySpeed_[i] * 4;
			// 下に落ちると消滅
			if (worldTransformEnemy_[i].translation_.y < -3) {
				enemyFlag_[i] = 0;
			}
		}
	}
}

void GameScene::Update() 
{
 //各シーンの更新処理を呼び出す
	switch (sceneMode_)
	{
	case 0:
		
		GamePlayUpdate();
		break;
	case 1:
		
		gameTimer_ += 1;
		TitleUpdate();
		break;
    case 2:
		gameTimer_ += 1;
		

		if (input_->TriggerKey(DIK_RETURN)) {
			audio_->StopWave(voiceHandleBGM_);
			voiceHandleBGM_ = audio_->PlayWave(soundDateHandleTitleBGM_, true);
			sceneMode_ = 1;
		}
	}

}

void GameScene::StageUpdate() {
	//各ステージでループ
	for (int u = 0; u < 20; u++) {
	//手前に移動
		worldTransformStage_[u].translation_.z -= 0.1f;
	//端まで来たら奥へ戻る
		if (worldTransformStage_[u].translation_.z < -5) {
			worldTransformStage_[u].translation_.z += 40;
		}
		//変換行列を更新
		worldTransformStage_[u].matWorld_ = MakeAffineMatrix(
		    worldTransformStage_[u].scale_, worldTransformStage_[u].rotation_,
		    worldTransformStage_[u].translation_);
		//変換行列を定数バッファに転送
		worldTransformStage_[u].TransferMatrix();
	}
}

void GameScene::GamePlayUpdate()
{
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
	StageUpdate();
}

void GameScene::GamePlayStart() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			playerTimer_ = 0;
			gameScore_ = 0;
			playerLife_ = 3;
			enemyFlag_[i] = 0;
			beamFlag_[j] = 0;
			gameTimer_ = 0;
			worldTransformEnemy_[i].translation_.y = 0;
			worldTransformPlayer_.translation_.x = 0;

			worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
			    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
			    worldTransformPlayer_.translation_);

			// 変更行列を定数バッファーに転送
			worldTransformPlayer_.TransferMatrix();
		}

	}
}



void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	GamePlayDraw2DBack();
	//各シーンの背景2Dを呼び出す
	switch (sceneMode_) 
	{
	case 0:
		GamePlayDraw2DBack();
		break;
	
	}


	//spriteBG_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	GamePlayDraw3d();
	/// 各シーンの3D表示を呼び出す
	switch (sceneMode_)
	{
	case 0:
		GamePlayDraw3d();
		break;
	}
	/// 
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	// ゲームプレイ更新3D
	
#pragma endregion
	
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	GamePlayDraw2DNear();
	//各シーンの近景2D表示を呼び出す
	switch (sceneMode_)
	{
	case 0:
		GamePlayDraw2DNear();
		break;
	case 1:
		TitleDraw2DNear();
		break;
	case 2:
		GameOverDraw2DNear();
		
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//ゲームプレイ表示3D
void GameScene::GamePlayDraw3d() {
	// ステージ
	for (int u = 0; u < 20; u++) {
		modelStage_->Draw(worldTransformStage_[u], viewProjection_, textureHandleStage_);
	} 

	// プレイヤー
	if (playerTimer_ % 4 < 2) {
		modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	}

	// 弾
	    for (int j = 0; j < 5; j++) {
		if (beamFlag_[j] == 1) {
			modelBeam_->Draw(worldTransformBeam_[j], viewProjection_, textureHandleBeam_);
		}
	    }
	// 敵
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1 || enemyFlag_ [i] == 2) {
			modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandEnemy_);
		}
	}
}

//ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() 
{
 //背景
	spriteBG_->Draw();
}

//ゲームプレイ表示2D近景
void GameScene::GamePlayDraw2DNear() 
{	
	DrawScore();	   
	debugText_->DrawAll();
}
//スコア数値の表示
void GameScene::DrawScore() 
{
	///各桁の値を取り出す
	    int eachNumber[5] = {};       //各桁の値
	    int number = gameScore_;      //表示する数字

		int keta = 10000;      // 最初の桁
	    for (int k = 0; k < 5; k++)
		{
		eachNumber[k] = number / keta;     //今の桁の値を求める
		number = number % keta;     //次の桁以下の値を取り出す
		keta = keta / 10;     //桁を進める
		}
	//各桁の数値を描画
	    for (int k = 0; k < 5; k++) 
		{
		spriteNumber_[k]->SetSize({32, 64});
		spriteNumber_[k]->SetTextureRect({32.0f * eachNumber[k], 0}, {32, 64});
		spriteNumber_[k]->Draw();
	    }

		// ライフ
	    for (int l = 0; l < playerLife_; l++) 
	    {
		spriteLife_[l]->Draw();
	    }

		spriteScore_->Draw();
}

// タイトル表示2D
void GameScene::TitleDraw2DNear() {
	    // タイトル表示
	    spriteTitle_->Draw();

	    // エンター表示
	    if (gameTimer_ % 40 >= 20) {
		// エンター表示
		spriteEnter_->Draw();
	    }
}
// ゲームオーバー
void GameScene::GameOverDraw2DNear()
{ 
	spriteOver_->Draw();
	    // エンター表示
	    if (gameTimer_ % 40 >= 20) {
		// エンター表示
		spriteEnter_->Draw();
	    }
	   
}
