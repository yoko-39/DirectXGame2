#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene()
{
  
}

GameScene::~GameScene() 
{
   //各クラスの削除
	delete stage_;   //ステージ
    delete player_;   //プレイヤー 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();
	//各クラスの生成
	stage_ = new Stage();     //ステージ
	//各クラスの初期化
	stage_->Initialize(viewProjection_);

	player_ = new Player();   //プレイヤ-
	player_->Initialize(viewProjection_);

}

void GameScene::Update() 
{
  //各クラスの更新
	stage_->Update();  // ステージ
	player_->Update();  //プレイヤー
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

	//背景の描画
	stage_->Draw2DFar(); 
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
	stage_->Draw3D();
	player_->Draw3D();
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
