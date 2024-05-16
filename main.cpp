#include <Novice.h>
#include "Vector3.h"
#include "Function.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


const char kWindowTitle[] = "LD2A_02_ワダ_ケイタ";

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	// 2つの球の中心点間の距離を求める
	float distance = Length({ s2.center.x - s1.center.x,s2.center.y - s1.center.y,s2.center.z - s1.center.z });
	// 半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius) {
		return true;
	}
	return false;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };

	Sphere sphere1{ { 0.0f,0.0f,0.0f },0.5f };
	Sphere sphere2{ { 2.0f,0.0f,2.0f },0.8f };

	Vector3 scale{ 1.0f,1.0f,1.0f };
	Vector3 rotate{};
	Vector3 translate{};

	Vector3 cameraScale{ 1.0f,1.0f,1.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotation{ 0.26f,0.0f,0.0f };

	int sphere1_color = WHITE;

	float cameraTranslateSpeed = 0.05f;
	float cameraRotateSpeed = 0.01f;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		if (keys[DIK_W]) {
			cameraTranslate.z += cameraTranslateSpeed;
		}
		if (keys[DIK_S]) {
			cameraTranslate.z -= cameraTranslateSpeed;
		}
		if (keys[DIK_A]) {
			cameraTranslate.x -= cameraTranslateSpeed;
		}
		if (keys[DIK_D]) {
			cameraTranslate.x += cameraTranslateSpeed;
		}
		if (keys[DIK_LEFT]) {
			cameraRotation.y -= cameraRotateSpeed;
		}
		if (keys[DIK_RIGHT]) {
			cameraRotation.y += cameraRotateSpeed;
		}
		if (keys[DIK_UP]) {
			cameraRotation.x -= cameraRotateSpeed;
		}
		if (keys[DIK_DOWN]) {
			cameraRotation.x += cameraRotateSpeed;
		}

		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		Matrix4x4 sphere1WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere1.center);
		Matrix4x4 sphere2WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere2.center);

		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotation, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		Matrix4x4 sphere1WorldViewProjectionMatrix = Multiply(sphere1WorldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 sphere2WorldViewProjectionMatrix = Multiply(sphere2WorldMatrix, Multiply(viewMatrix, projectionMatrix));


		// 当たり判定
		if (IsCollision(sphere1, sphere2) == true) {
			sphere1_color = RED;
		}
		else {
			sphere1_color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);


		DrawSphere(sphere1, sphere1WorldViewProjectionMatrix, viewportMatrix, sphere1_color);
		DrawSphere(sphere2, sphere2WorldViewProjectionMatrix, viewportMatrix, WHITE);

#ifdef _DEBUG


		ImGui::Begin("camera");
		ImGui::DragFloat3("scale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("Translation", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("rotate", &cameraRotation.x, 0.01f);
		ImGui::DragFloat("translateSpeed", &cameraTranslateSpeed, 0.01f);
		ImGui::DragFloat("rotateSpeed", &cameraRotateSpeed, 0.01f);

		ImGui::End();

		ImGui::Begin("sphere");
		ImGui::DragFloat3("sphere1.position", &sphere1.center.x, 0.01f);
		ImGui::DragFloat("sphere1.radius", &sphere1.radius, 0.01f);
		ImGui::DragFloat3("sphere2.position", &sphere2.center.x, 0.01f);
		ImGui::DragFloat("rsphere2.adius", &sphere2.radius, 0.01f);
		ImGui::End();

#endif // _DEBUG


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
