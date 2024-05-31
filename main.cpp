#include <Novice.h>
#include "Vector3.h"
#include "Function.h"
#include <algorithm>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

const char kWindowTitle[] = "LD2A_02_ワダ_ケイタ";


bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };
	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	// 距離が半径よりも小さければ衝突
	if(distance <= sphere.radius){
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

	Sphere sphere;
	sphere.center = { 1.0f,1.0f,1.0f };
	sphere.radius = 0.5f;

	AABB aabb1{
		.min{ -0.5f, -0.5f, -0.5f },
		.max{ 0.5f, 0.5f, 0.5f },
	};

	int aabb1_color = WHITE;
	int sphere_color = WHITE;

	Vector3 scale{ 1.0f,1.0f,1.0f };
	Vector3 rotate{};
	Vector3 translate{};

	Vector3 cameraScale{ 1.0f,1.0f,1.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotation{ 0.26f,0.0f,0.0f };

	//int sphere1_color = WHITE;

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
		Matrix4x4 sphere1WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere.center);

		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotation, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		Matrix4x4 sphere1WorldViewProjectionMatrix = Multiply(sphere1WorldMatrix, Multiply(viewMatrix, projectionMatrix));


		// 当たり判定
		if (IsCollision(aabb1, sphere) == true) {
			aabb1_color = RED;
		}
		else {
			aabb1_color = WHITE;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, aabb1_color);
		DrawSphere(sphere, sphere1WorldViewProjectionMatrix, viewportMatrix, sphere_color);

#ifdef _DEBUG


		ImGui::Begin("camera");
		ImGui::DragFloat3("scale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("Translation", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("rotate", &cameraRotation.x, 0.01f);
		ImGui::DragFloat("translateSpeed", &cameraTranslateSpeed, 0.01f);
		ImGui::DragFloat("rotateSpeed", &cameraRotateSpeed, 0.01f);

		ImGui::End();

		ImGui::Begin("Object");

		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);

		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);

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
