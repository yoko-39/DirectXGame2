#include "EnemyBeam.h"

EnemyBeam::EnemyBeam() {}

EnemyBeam::~EnemyBeam() { 
	delete modelEBeam_; 

}

void EnemyBeam::Initialize(ViewProjection viewProjection, Enemy* enemy) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;
	enemy_ = enemy;
	// 弾
	textureHandleEBeam_ = TextureManager::Load("source/enemybeam.png");
	modelEBeam_ = Model::Create();
	worldTransformEBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformEBeam_.Initialize();

	// インプットクラス
	input_ = Input::GetInstance();
}
void EnemyBeam::Move() {
	if (EbeamFlag_ == 1) {
		worldTransformEBeam_.translation_.z -= 1.0f;
		worldTransformEBeam_.rotation_.x += 0.1f;
	}
	if (worldTransformEBeam_.translation_.z < -3) {
		EbeamFlag_ = 0;
		worldTransformEBeam_.translation_.z = -5;
	}
}

void EnemyBeam::Born() {
	if (EbeamFlag_ == 0 && enemy_->GetFlag() == 1)  {
		worldTransformEBeam_.translation_.x = enemy_->GetX();
		worldTransformEBeam_.translation_.y = enemy_->GetY();
		worldTransformEBeam_.translation_.z = enemy_->GetZ();
		EbeamFlag_ = 1;
	}
}

void EnemyBeam::Start() {
	EbeamFlag_ = 0;
	worldTransformEBeam_.translation_.z = 0;
}

void EnemyBeam::Update() {
	Move();

	// 変更行列を更新
	worldTransformEBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformEBeam_.scale_, worldTransformEBeam_.rotation_,
	    worldTransformEBeam_.translation_);

	// 変更行列を定数バッファーに転送
	worldTransformEBeam_.TransferMatrix();
}

void EnemyBeam::Draw3D() {
	if (EbeamFlag_ == 1) {
		modelEBeam_->Draw(worldTransformEBeam_, viewProjection_, textureHandleEBeam_);
	}
}

