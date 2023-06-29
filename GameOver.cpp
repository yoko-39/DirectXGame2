#include "GameOver.h"

GameOver::GameOver() {

}

GameOver::~GameOver() {
	
}

void GameOver::Initialize(ViewProjection viewProjection) {
	// ゲームオーバー
	textureHandleOver_ = TextureManager::Load("gameover.png");
	spriteOver_ = Sprite::Create(textureHandleOver_, {0, 0});
	// エンター
	textureHandleEnter_ = TextureManager::Load("Enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});
	
	
}

void GameOver::Update() { 
	gameTimer_ += 1; 
}

void GameOver::Draw2DNear() {

	spriteOver_->Draw();
	// エンター表示
	if (gameTimer_ % 40 >= 20) {
		// エンター表示
		spriteEnter_->Draw();
	}
}
