#include "Title.h"

Title::Title() {}

Title::~Title() { 
	delete spriteTitle_; 
}

void Title::Initialize() {
	// タイトル
	textureHandleTitle_ = TextureManager::Load("source/title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});
	// エンター
	textureHandleEnter_ = TextureManager::Load("source/Enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});

	


	// インプットクラス
	input_ = Input::GetInstance();

	//サウンドデータの読み込み
	audio_ = Audio::GetInstance();
	soundDatahandleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	Start();
}

void Title::Start() {
//BGMを再生
//	voiceHandleBGM_ = audio_->PlayWave(soundDatahandleBGM_, true);

}

int Title::Update() {
	gameTimer_ += 1;

	if (input_->TriggerKey(DIK_RETURN)) {
		audio_->StopWave(voiceHandleBGM_);
		
		return 0;
	}
	return 1;
}

void Title::Draw2DNear() {
    
	//背景
	spriteTitle_->Draw();

	// エンター表示
	if (gameTimer_ % 40 >= 20) {
		// エンター表示
		spriteEnter_->Draw();
	}
}


