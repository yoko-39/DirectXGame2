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
	delete beam_;    //ビーム
	delete enemy_;
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

	beam_ = new Beam();      //ビーム
	beam_->Initialize(viewProjection_, player_);
	 
	enemy_ = new Enemy();
	enemy_->Initialize(viewProjection_);

	//デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

void GameScene::Update() 
{
  //各クラスの更新
	stage_->Update();  // ステージ
	player_->Update();  //プレイヤー
	beam_->Update();   //ビーム
	enemy_->Update();   //敵
	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}

void GameScene::CollisionPlayerEnemy() {
//敵が存在すれば
	if (enemy_->GetFlag() == 1) {
	   //差を求める
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		if (dx < 1 && dz < 1) {
		   //衝突処理
			enemy_->Hit();
		   playerLife_ -= 1;
		}
	}
}

void GameScene::CollisionBeamEnemy() {

	if (enemy_->GetFlag() == 1 && beam_->GetFlag() == 1) {
		float dx1 = abs(beam_->GetX() - enemy_->GetX());
		float dz1 = abs(beam_->GetZ() - enemy_->GetZ());

		if (dx1 < 1 && dz1 < 1) {
			enemy_->Hit();
			beam_->Hit();
			gameScore_ += 100;
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
	beam_->Draw3D();
	enemy_->Draw3D();
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	char str[100];

	sprintf_s(str, "SCORE %d ", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d ", playerLife_);
	debugText_->Print(str, 800, 10, 2);

	debugText_->DrawAll();
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


