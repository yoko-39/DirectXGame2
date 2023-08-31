#include "GameClear.h"

GameClear::GameClear() {}

GameClear::~GameClear() {}

void GameClear::Initialize(ViewProjection viewProjection) {

// ゲームクリア
	textureHandleClear_ = TextureManager::Load("source/gameclear.png");
	spriteClear_ = Sprite::Create(textureHandleClear_, {0, 0});
	// エンター
	textureHandleEnter_ = TextureManager::Load("source/Enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});
}

void GameClear::Update() { 
	gameTimer_ += 1; 
}

void GameClear::Draw2DNear() {
	spriteClear_->Draw();
	// エンター表示
	if (gameTimer_ % 40 >= 20) {
		// エンター表示
		spriteEnter_->Draw();
	}
}

void GameClear::Start() {}

void GameClear::Stop() {}
