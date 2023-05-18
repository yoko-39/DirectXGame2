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


}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, { 0, 0 });

	//ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	//ステージの位置を変更
	worldTransformStage_.translation_ = { 0, -1.5f, 0 };
	worldTransformStage_.scale_ = { 4.5f, 1, 40 };

	//変更行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
		worldTransformStage_.scale_, worldTransformStage_.rotation_,
		worldTransformStage_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformStage_.TransferMatrix();

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
	// 変換行列を更新
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
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
				worldTransformEnemy_[i].translation_.z = 40.0f;

				int x = rand() % 80;
				float x2 = (float)x / 10 + -4;
				worldTransformEnemy_[i].translation_.x = x2;

				if (rand() % 2 == 0) {
					enemyspeed_[i] = 0.1f;
				} else {
					enemyspeed_[i] = -0.1f;
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
			worldTransformEnemy_[i].translation_.x += enemyspeed_[i];
			worldTransformEnemy_[i].translation_.z -= 0.5f;
			worldTransformEnemy_[i].rotation_.x -= 0.1f;
		}
		if (worldTransformEnemy_[i].translation_.z < -5.0f) {
			enemyFlag_[i] = 0;
		}

		if (worldTransformEnemy_[i].translation_.x > 4) {
			enemyspeed_[i] *= -1.0f; 
		}
		if (worldTransformEnemy_[i].translation_.x < -4) {
			enemyspeed_[i] *= -1.0f; 
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
				// 存在しない
				enemyFlag_[i] = 0;
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
					enemyFlag_[i] = 0;
					beamFlag_[j] = 0;
					gameScore_ += 100;
				}
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
			sceneMode_ = 1;
		}
	}

}

void GameScene::GamePlayUpdate()
{
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
}

void GameScene::GamePlayStart() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			gameScore_ = 0;
			playerLife_ = 3;
			enemyFlag_[i] = 0;
			beamFlag_[j] = 0;
			gameTimer_ = 0;

			worldTransformPlayer_.translation_.x = 0;

			worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
			    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
			    worldTransformPlayer_.translation_);

			// 変更行列を定数バッファーに転送
			worldTransformPlayer_.TransferMatrix();
		}
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
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// プレイヤー

		modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	

	// 弾
	    for (int j = 0; j < 5; j++) {
		if (beamFlag_[j] == 1) {
			modelBeam_->Draw(worldTransformBeam_[j], viewProjection_, textureHandleBeam_);
		}
	    }
	// 敵
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
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
	// ゲームスコア
	char str[100];
	sprintf_s(str, "score %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "Life %d", playerLife_);
	debugText_->Print(str, 800, 10, 2);

	
	   
		debugText_->DrawAll();
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
