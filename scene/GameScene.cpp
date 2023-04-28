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
}

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
	if (beamflag == 1) {
	    worldTransformBeam_.translation_.z += 1;
		worldTransformBeam_.rotation_.x += 0.1f;
	}

	if (worldTransformBeam_.translation_.z > 40) {

	beamflag = 0;
	}
}
void GameScene::BeamBorn()
{
	if (beamflag == 0) {

		if (input_->PushKey(DIK_SPACE)) {
			worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.y = worldTransformPlayer_.translation_.y;
			worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		    beamflag= 1;
		}
	}
}

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
	if (enemyflag == 0) {
		enemyflag = 1;
		worldTransformEnemy_.translation_.z = 40.0f;

		
			int x = rand() % 80;
			float x2 = (float)x / 10 + -4;
			worldTransformEnemy_.translation_.x = x2;
		
	}

}
void GameScene::EnemyMove() 
{ 
         
	if (enemyflag == 1) {
	    worldTransformEnemy_.translation_.z -= 0.5f;
		worldTransformEnemy_.rotation_.x -= 0.1f;
	}

	if (worldTransformEnemy_.translation_.z < -5.0f) {
		enemyflag = 0;
	}
}

void GameScene::Update() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
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
	spriteBG_->Draw();
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

	//ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
	
	//プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	//弾
	if (beamflag == 1) {
	modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_); 
	}

	// 敵
	if (enemyflag == 1) {
	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandEnemy_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
