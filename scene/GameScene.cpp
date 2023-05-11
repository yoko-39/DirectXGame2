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
	worldTransformBeam_.scale_ = { 0.3f, 0.3f, 0.3f };
	worldTransformBeam_.Initialize();

	// 敵
	textureHandEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();

	//乱数調整
	srand((unsigned int)time(NULL));

	//デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
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
}

//ビーム
void GameScene::BeamUpdate()
{
	//移動
	BeamMove();
	//ビーム発生
	BeamBorn();

	// 変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
		worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
		worldTransformBeam_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformBeam_.TransferMatrix();

}

void GameScene::BeamMove()
{
	if (beamFlag_ == 1) {
	    worldTransformBeam_.translation_.z += 1;
		worldTransformBeam_.rotation_.x += 0.1f;
	}

	if (worldTransformBeam_.translation_.z > 40) {

	beamFlag_ = 0;
	}
}
void GameScene::BeamBorn()
{
	if (beamFlag_ == 0) {

		if (input_->PushKey(DIK_SPACE)) {
			worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.y = worldTransformPlayer_.translation_.y;
			worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		    beamFlag_= 1;
		}
	}
}

//敵
void GameScene::EnemyUpdate() 
{
	EnemyMove();
	EnemyBorn();
	// 変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformEnemy_.TransferMatrix();
}

void GameScene::EnemyBorn() {
	if (enemyFlag_ == 0) {
		enemyFlag_ = 1;
		worldTransformEnemy_.translation_.z = 40.0f;

		
			int x = rand() % 80;
			float x2 = (float)x / 10 + -4;
			worldTransformEnemy_.translation_.x = x2;
		
	}

}
void GameScene::EnemyMove() 
{ 
         
	if (enemyFlag_ == 1) {
	    worldTransformEnemy_.translation_.z -= 0.5f;
		worldTransformEnemy_.rotation_.x -= 0.1f;
	}

	if (worldTransformEnemy_.translation_.z < -5.0f) {
		enemyFlag_ = 0;
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
 //敵が存在すれば
	if (enemyFlag_ == 1) {
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);

	//衝突したら
		if (dx < 1 && dz < 1) {
			playerLife_ -= 1;
			// 存在しない
			enemyFlag_ = 0;
		}
	}
}

void GameScene::CollisionBeamEnemy() 
{ 
	if (enemyFlag_ == 1 && beamFlag_ == 1) {
		float dx1 = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz1 = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);

		if (dx1 < 1 && dz1 < 1) {
			enemyFlag_ = 0;
			beamFlag_ = 0;
			gameScore_ += 100;
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
	}

}

void GameScene::GamePlayUpdate()
{
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
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
	//各シーンの近景2D表示を呼び出す
	switch (sceneMode_)
	{
	case 0:
		GamePlayDraw2DNear();
		break;
	}
	debugText_->DrawAll();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//ゲームプレイ表示3D
void GameScene::GamePlayDraw3d() {
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// プレイヤー
	if (playerLife_ > 0) {
		modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	}

	// 弾
	if (beamFlag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}

	// 敵
	if (enemyFlag_ == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandEnemy_);
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


}
