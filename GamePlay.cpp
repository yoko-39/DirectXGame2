#include "GamePlay.h"

GamePlay::GamePlay() {

}

GamePlay::~GamePlay() {
	// 各クラスの削除
	
	delete stage_; // ステージ
	delete player_; // プレイヤー
	for (Beam* beam : beamTable_) {
		delete beam; // ビーム
	}
	for (Enemy* enemy : enemyTable_) {
		delete enemy; // 敵
	}
	delete spriteScore_;
	for (int k = 0; k < 5; k++) {
		delete spriteNumber_[k];
	}

	for (int l = 0; l < 3; l++) {
		delete spriteLife_[l]; 
	}
}
void GamePlay::Initialize(ViewProjection viewProjection) {

	// ビュープロジェクションの初期化(カメラの位置がずれる)
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// 各クラスの生成

	stage_= new Stage(); // ステージ
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
	// サウンドデータの読み込み
	audio_ = Audio::GetInstance();
	soundDatahandleBGM_ = audio_->LoadWave("Audio/Ring08.wav");

	soundDateHandleEnemySE_ = audio_->LoadWave("Audio/chord.wav");
	soundDateHandlePlayerSE_ = audio_->LoadWave("Audio/tada.wav");
	//スコア数値(2Dスプライト)
	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int k = 0; k < 5; k++) {
		spriteNumber_[k] = Sprite::Create(textureHandleNumber_, {300.0f + k * 26, 0});
	}
	// スコア
	textureHandleScore_ = TextureManager::Load("score.png");
	spriteScore_ = Sprite::Create(textureHandleScore_, {170.0f, 0});

	// ライフ
	textureHandleLife_ = TextureManager::Load("player.png");

	for (int l = 0; l < playerLife_; l++) {
		spriteLife_[l] = Sprite::Create(textureHandleLife_, {800.0f + l * 60, 0});
		spriteLife_[l]->SetSize({40, 40});
	}
}

int GamePlay::Update() {
	Shot();
	gameTimer_ += 1;
	// 各クラスの更新
	stage_->Update(); // ステージ
	player_->Update(); // プレイヤー
	for (Beam* beam : beamTable_) {
		beam->Update(); // ビーム
	}
	for (Enemy* enemy : enemyTable_) {
		enemy->Update(gameTimer_); // 敵
	}

	if (playerTimer_ > 0) {
		playerTimer_ -= 1;
	}
	//衝突判定(プレイヤーと敵)
	CollisionPlayerEnemy();
	// 衝突判定(ビームと敵)
	CollisionBeamEnemy();



	if (playerLife_ <= 0) {
		audio_->StopWave(voiceHandleBGM_);
		return 2;
	}
	    return 0;

}

void GamePlay::Draw2DFar() {
	// 背景の描画
		stage_->Draw2DFar();
}

void GamePlay::Draw3D() {
    stage_->Draw3D(); // ステージ
	    if (playerLife_ >= 0) {
		if (playerTimer_ % 4 < 2) {
			player_->Draw3D(); // プレイヤー
		}
	    }
	for (Beam* beam : beamTable_) {
	beam->Draw3D(); // ビーム
	}
	for (Enemy* enemy : enemyTable_) {
	enemy->Draw3D(); // 敵
	}
}

void GamePlay::Draw2DNear() {
	
	DrawScore();
	debugText_->DrawAll();
}

void GamePlay::Start() { 
	playerLife_ = 3;
	gameScore_ = 0;
	gameTimer_ = 0;
	playerTimer_ = 0;
	for (Enemy* enemy : enemyTable_) {
	enemy->Start();
	}
	for (Beam* beam : beamTable_) {
	beam->Start();
	}
	player_->Start();
	// BGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDatahandleBGM_, true);
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

void GamePlay::DrawScore() {
	//各桁の値を取り出す
	int eachNumber[5] = {};
	int number = gameScore_;

	int keta = 10000;
	for (int k = 0; k < 5; k++) {
	eachNumber[k] = number / keta;
	number = number % keta;
	keta = keta / 10;
	}
    //各桁の数値を描画
	for (int k = 0; k < 5; k++) {
	spriteNumber_[k]->SetSize({32, 64});
	spriteNumber_[k]->SetTextureRect({32.0f * eachNumber[k], 0}, {32, 64});
	spriteNumber_[k]->Draw();
	}
	spriteScore_->Draw();

		// ライフ
	for (int l = 0; l < playerLife_; l++) {
	spriteLife_[l]->Draw();
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
			audio_->PlayWave(soundDateHandlePlayerSE_);
			// 衝突処理
			enemy->Hit();
			playerLife_ -= 1;
			playerTimer_ = 60;
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
				audio_->PlayWave(soundDateHandleEnemySE_);
				gameScore_ += 100;
			}
		}
	}
	}

}
