#include "GamePlay.h"

GamePlay::GamePlay() {

}

GamePlay::~GamePlay() {
	// 各クラスの削除
	delete stage_;  // ステージ
	delete player_; // プレイヤー
	for (Beam* beam : beamTable_) {
		delete beam; // ビーム
	}
	for (Enemy* enemy : enemyTable_) {
		delete enemy; // 敵
	}
}
void GamePlay::Initialize(ViewProjection viewProjection) {

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// 各クラスの生成
	stage_ = new Stage(); // ステージ
	player_ = new Player(); // プレイヤ-
	for (int j = 0; j < 20; j++) {
		beamTable_[j] = new Beam(); // ビーム
	}

	for (int i = 0; i < 10; i++) {
		enemyTable_[i] = new Enemy();
	}

	// 各クラスの初期化
	stage_->Initialize(viewProjection_);
	player_->Initialize(viewProjection_);
	for (Beam* beam : beamTable_) {
		beam->Initialize(viewProjection_, player_);
	}
	for (Enemy* enemy : enemyTable_) {
		enemy->Initialize(viewProjection_);
	}
	
	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
	// インプットクラス
	input_ = Input::GetInstance();
}

int GamePlay::Update() {
	Shot();
	// 各クラスの更新
	stage_->Update();  // ステージ
	player_->Update(); // プレイヤー
	for (Beam* beam : beamTable_) {
		beam->Update(); // ビーム
	}
	for (Enemy* enemy : enemyTable_) {
		enemy->Update(); // 敵
	}
	//衝突判定(プレイヤーと敵)
	CollisionPlayerEnemy();
	// 衝突判定(ビームと敵)
	CollisionBeamEnemy();
	if (playerLife_ <= 0) {
		return 2;
	}
	    return 0;
}

void GamePlay::Draw2DFar() {
	// 背景の描画
	stage_->Draw2DFar();
}

void GamePlay::Draw3D() {
	stage_->Draw3D();    //ステージ
	if (playerLife_ >= 0) {
	player_->Draw3D();   //プレイヤー
	}
	for (Beam* beam : beamTable_) {
	beam->Draw3D(); // ビーム
	}
	for (Enemy* enemy : enemyTable_) {
	enemy->Draw3D(); // 敵
	}
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

void GamePlay::Start() { 
	playerLife_ = 3;
	gameScore_ = 0;
}

void GamePlay::Shot() {
	if (shotTimer_ == 0) {
	// スぺースキーを押したら
	if (input_->PushKey(DIK_SPACE)) {
		// ビームでループ
		for (Beam* beam : beamTable_) {
			// 存在しなければ
			if (beam->GetFlag() == 0) {
				// 発射(発生)
				beam->Born();
				beam->Update();
				shotTimer_ = 1;
				break;
			}
		}
	}
	} else {
	// タイマー時間加算
	shotTimer_++;

	// 一定時間で
	if (shotTimer_ > 10) {
		// 発射できる状態
		shotTimer_ = 0;
	}
	}
}

void GamePlay::CollisionPlayerEnemy() {
	// 敵が存在すれば
	for (Enemy* enemy : enemyTable_) {
	if (enemy->GetFlag() == 1) {
		// 差を求める
		float dx = abs(player_->GetX() - enemy->GetX());
		float dz = abs(player_->GetZ() - enemy->GetZ());

		if (dx < 1 && dz < 1) {
			// 衝突処理
			enemy->Hit();
			playerLife_ -= 1;
		}
	}
	}
}

void GamePlay::CollisionBeamEnemy() {
	for (Enemy* enemy : enemyTable_) {
	for (Beam* beam : beamTable_) {
		if (enemy->GetFlag() == 1 && beam->GetFlag() == 1) {
			float dx1 = abs(beam->GetX() - enemy->GetX());
			float dz1 = abs(beam->GetZ() - enemy->GetZ());

			if (dx1 < 1 && dz1 < 1) {
				enemy->Hit();
				beam->Hit();
				gameScore_ += 100;
			}
		}
	}
	}
	
}
