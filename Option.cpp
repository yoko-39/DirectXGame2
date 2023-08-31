#include "Option.h"

Option::Option() {

}

Option::~Option() { 
	delete spriteOption_; 
}

void Option::Initialize(ViewProjection viewProjection) {
	// オプション
	textureHandleOption_ = TextureManager::Load("source/option.png");
	spriteOption_ = Sprite::Create(textureHandleOption_, {0, 0});
}

void Option::Start() {}

int Option::Update() { 
	return 1;
}

void Option::Draw2DNear() { 
	
	spriteOption_->Draw();

}
