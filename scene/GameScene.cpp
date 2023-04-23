#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete debugcamera_;
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 読み込み処理
	textureHandle_ = TextureManager::Load("sample.png");

	// 生成
	sprite_ = Sprite::Create(textureHandle_, {10, 10});

	// 読み込み処理
	modeltextureHandle_ = TextureManager::Load("sample.png");
	// 3D生成
	model_ = Model::Create();

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();

	// サウンドデータの読み込み
	sonudDateHandle_ = audio_->LoadWave("fanfare.wav");
	// 音声再生
	audio_->PlayWave(sonudDateHandle_, true);
	// 音声再生
	voiceHandle_ = audio_->PlayWave(sonudDateHandle_, true);
	// ライン描画が参照するビュープロジェクションを指定する(アドレスなし）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewprojection_);
	// デバッグカメラの生成
	debugcamera_ = new DebugCamera(100, 100);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレスなし)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugcamera_->GetViewProjection());
}

void GameScene::Update() 
{
	// スプライト移動
	Vector2 pos = sprite_->GetPosition();
	pos.x += 1;
	pos.y += 1;
	sprite_->SetPosition(pos);

	if (input_->TriggerKey(DIK_SPACE)) {
		// 音声停止
		audio_->StopWave(voiceHandle_);
	}

	// デバッグテキスト
	ImGui::ShowDemoWindow();
	ImGui::Begin("Debug1");
	ImGui::Text("kamata Tarou %d,%d,%d", 2050, 12, 31);
	ImGui::InputFloat3("InputFloat3", inputFloat);
	ImGui::SliderFloat3("SliderFloat3", inputFloat, 0.0f, 1.0f);
	ImGui::End();

	// デバッグカメラの更新
	debugcamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
	model_->Draw(worldTransform_, debugcamera_->GetViewProjection(), textureHandle_);
	// model_->Draw(worldTransform_, viewprojection_, textureHandle_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	// ライン描画
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0},{0,10,0,}, {1.0f, 0.0f, 0.0f, 1.0f});

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
