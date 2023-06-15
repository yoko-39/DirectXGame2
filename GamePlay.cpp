#include "GamePlay.h"

GamePlay::GamePlay() {}

GamePlay::~GamePlay() {
 // 各クラスの削除
	delete stage_;  // ステージ
	delete player_; // プレイヤー
	delete beam_;   // ビーム
	delete enemy_;
}

void GamePlay::Initialize(ViewProjection viewProjection) {

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();
	// 各クラスの生成
	stage_ = new Stage(); // ステージ
	player_ = new Player(); // プレイヤ-
	beam_ = new Beam(); // ビーム
	enemy_ = new Enemy();
	
	// 各クラスの初期化
	stage_->Initialize(viewProjection_);
	player_->Initialize(viewProjection_);
	beam_->Initialize(viewProjection_, player_);
	enemy_->Initialize(viewProjection_);

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

void GamePlay::Update() {
	// 各クラスの更新
	stage_->Update();  // ステージ
	player_->Update(); // プレイヤー
	beam_->Update();   // ビーム
	enemy_->Update();  // 敵

	//衝突判定(プレイヤーと敵)
	CollisionPlayerEnemy();
	// 衝突判定(ビームと敵)
	CollisionBeamEnemy();
}

void GamePlay::Draw2DFar() {
	// 背景の描画
	stage_->Draw2DFar();
}

void GamePlay::Draw3D() {
	stage_->Draw3D();    //ステージ
	player_->Draw3D();   //プレイヤー
	beam_->Draw3D();    //ビーム
	enemy_->Draw3D();   //敵
}

void GamePlay::Draw2DNear() {
	//ゲームプレイ
	char str[100];
	sprintf_s(str, "SCORE %d ", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d ", playerLife_);
	debugText_->Print(str, 800, 10, 2);

	debugText_->DrawAll();
}

void GamePlay::CollisionPlayerEnemy() {
	// 敵が存在すれば
	if (enemy_->GetFlag() == 1) {
		// 差を求める
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		if (dx < 1 && dz < 1) {
			// 衝突処理
			enemy_->Hit();
			playerLife_ -= 1;
		}
	}
}

void GamePlay::CollisionBeamEnemy() {
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
