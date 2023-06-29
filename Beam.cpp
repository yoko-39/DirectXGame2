#include "Beam.h"
#include "Player.h"

Beam::Beam() {}

Beam::~Beam() { delete modelBeam_; }

void Beam::Initialize(ViewProjection viewProjection, Player* player) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;
	player_ = player;
	// 弾
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();

	// インプットクラス
	input_ = Input::GetInstance();
}

void Beam::Born() {
	if (beamFlag_ == 0) {
		if (input_->PushKey(DIK_SPACE)) {
			worldTransformBeam_.translation_.x = player_->GetX();
			beamFlag_ = 1;
		}
	}
}
void Beam::Start() {}
void Beam::Move() {
	if (beamFlag_ == 1) {
		worldTransformBeam_.translation_.z += 0.5;
		worldTransformBeam_.rotation_.x += 0.1f;
	}
	if (worldTransformBeam_.translation_.z > 40) {
		beamFlag_ = 0;
		worldTransformBeam_.translation_.z = 0;
	}
}
void Beam::Update() {
	Move();

	// 変更行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformBeam_.TransferMatrix();
}

void Beam::Draw3D() {
	if (beamFlag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
}
