#include "Stage.h"

//コンストラクタ
Stage::Stage() 
{

}

//デストラクタ
Stage::~Stage() 
{ 
	delete spriteBG_; 
	delete modelStage_;
}

//初期化
void Stage::Initialize(ViewProjection viewProjection) 
{
	//メンバー変数に代入
	viewProjection_ = viewProjection;
	//BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage2.jpg");
	modelStage_ = Model::Create();
	for (int u = 0; u < 20; u++) {
		worldTransformStageTable_[u].Initialize();
	}
	
	// ステージの位置を変更
	for (int u = 0; u < 20; u++) {

		worldTransformStageTable_[u].translation_ = {0, -1.5f, 2.0f * u - 5};
		worldTransformStageTable_[u].scale_ = {4.5f, 1, 1};

		// 変更行列を更新
		worldTransformStageTable_[u].matWorld_ = MakeAffineMatrix(
		    worldTransformStageTable_[u].scale_, worldTransformStageTable_[u].rotation_,
		    worldTransformStageTable_[u].translation_);

		// 変更行列を定数バッファーに転送
		worldTransformStageTable_[u].TransferMatrix();
	}

}

//更新
void Stage::Update() {
	for (int u = 0; u < 20; u++) {
		//手前に移動
		worldTransformStageTable_[u].translation_.z -= 0.1f;
		//端まで来たら奥へ戻る
		if (worldTransformStageTable_[u].translation_.z < -5) {
			worldTransformStageTable_[u].translation_.z += 40;
		}
		//変換行列を更新
		worldTransformStageTable_[u].matWorld_ = MakeAffineMatrix(
		    worldTransformStageTable_[u].scale_, worldTransformStageTable_[u].rotation_,
		    worldTransformStageTable_[u].translation_);
		// 変更行列を定数バッファーに転送
		worldTransformStageTable_[u].TransferMatrix();
	}
}
//2D背景描画
void Stage::Draw2DFar()
{
	//背景
	spriteBG_->Draw();
}

//3D描画
void Stage::Draw3D() 
{
 //ステージ
	for (int u = 0; u < 20; u++) {
		modelStage_->Draw(worldTransformStageTable_[u], viewProjection_, textureHandleStage_);
	}
	}

