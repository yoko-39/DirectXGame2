#include "Player.h"
#include "TextureManager.h"

// コンストラクタ
Player::Player() {

}

// デストラクタ
Player::~Player() {

	delete modelPlayer_;
}

// 初期化
void Player::Initialize(ViewProjection viewProjection) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;
	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();
	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	
	// 変更行列を定数バッファーに転送
	worldTransformPlayer_.TransferMatrix();
	//インプットクラス
	input_ = Input::GetInstance();
}

// 更新
void Player::Update() {

	// 移動
	// 右へ移動
	if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}

	if (input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_W)) {
		worldTransformPlayer_.translation_.y += 0.1f;
	}

    if (input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_S)) {
		worldTransformPlayer_.translation_.y -= 0.1f;
	}


	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformPlayer_.TransferMatrix();

	if (worldTransformPlayer_.translation_.x > 4) {
		worldTransformPlayer_.translation_.x = 4;
	}
	if (worldTransformPlayer_.translation_.x < -4) {
		worldTransformPlayer_.translation_.x = -4;
	}
	if (worldTransformPlayer_.translation_.y < 0) {
		worldTransformPlayer_.translation_.y = 0;
	}
	if (worldTransformPlayer_.translation_.y > 3) {
		worldTransformPlayer_.translation_.y = 3;
	}
}

// プレイヤー
void Player::Draw3D() {
	
	   modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}

void Player::Start() {
	worldTransformPlayer_.translation_.x = 0; 
	worldTransformPlayer_.translation_.y = 0;
}
