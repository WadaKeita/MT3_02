#include <Novice.h>
#include "Vector3.h"
#include "Function.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


const char kWindowTitle[] = "LD2A_02_ワダ_ケイタ";

struct Line {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

struct Ray {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

struct Segment {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result = Multiply(Dot(v1, Normalize(v2)), Normalize(v2));

	return result;
};

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 result{};

	result = Add(segment.origin, Project(Subtract(point, segment.origin), segment.diff));

	result.x = Clamp(result.x, segment.origin.x, segment.origin.x + segment.diff.x);
	result.y = Clamp(result.y, segment.origin.y, segment.origin.y + segment.diff.y);
	result.z = Clamp(result.z, segment.origin.z, segment.origin.z + segment.diff.z);

	return result;
};


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
	Vector3 point{ -1.5f,0.6f,0.6f };



	Vector3 scale{ 1.0f,1.0f,1.0f };
	Vector3 rotate{};
	Vector3 translate{};

	Vector3 cameraScale{ 1.0f,1.0f,1.0f };
	Vector3 cameraPosition{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotation{ 0.26f,0.0f,0.0f };


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


		Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
		Vector3 closestPoint = ClosestPoint(point, segment);

		Sphere pointSphere{ point,0.01f };
		Sphere closestPointSphere{ closestPoint,0.01f };


		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		Matrix4x4 sphereWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, pointSphere.center);
		Matrix4x4 closestSphereWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, closestPointSphere.center);

		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotation, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);



		Matrix4x4 sphereWorldViewProjectionMatrix = Multiply(sphereWorldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 closestSphereWorldViewProjectionMatrix = Multiply(closestSphereWorldMatrix, Multiply(viewMatrix, projectionMatrix));


		// 線分
		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldViewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawSphere(pointSphere, sphereWorldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, closestSphereWorldViewProjectionMatrix, viewportMatrix, BLACK);

#ifdef _DEBUG


		ImGui::Begin("camera");
		ImGui::DragFloat3("scale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("position", &cameraPosition.x, 0.01f);
		ImGui::DragFloat3("rotate", &cameraRotation.x, 0.01f);
		ImGui::End();

		ImGui::Begin("sphere");
		ImGui::DragFloat3("position", &point.x, 0.01f);
		ImGui::End();

		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
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
