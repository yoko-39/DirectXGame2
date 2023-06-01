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
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();


}

//更新
void Stage::Update() 
{

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
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
}

