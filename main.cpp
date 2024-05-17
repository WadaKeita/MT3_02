#include <Novice.h>
#include "Vector3.h"
#include "Function.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

const char kWindowTitle[] = "LD2A_02_ワダ_ケイタ";

/// <summary>
/// 線分と平面
/// </summary>
/// <param name="segment"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const Plane& plane) {
	// まずは垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	if (t < 0) {
		return false;
	}
	else if (t > 1) {
		return false;
	}
	return true;
}

/// <summary>
/// 直線と平面
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Line& line, const Plane& plane) {
	// まずは垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	if (t == 0) {
		return false;
	}
	return true;
}

/// <summary>
/// 半直線と平面
/// </summary>
/// <param name="ray"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Ray& ray, const Plane& plane) {
	// まずは垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;

	if (t < 0) {
		return false;
	}
	return true;
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
	int segment_color = WHITE;
	//Sphere sphere1{ { 0.0f,0.0f,0.0f },0.5f };


	Plane plane{ {0.0f,1.0f,0.0f},1.0f };

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
		//Matrix4x4 sphere1WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere1.center);

		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotation, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		//Matrix4x4 sphere1WorldViewProjectionMatrix = Multiply(sphere1WorldMatrix, Multiply(viewMatrix, projectionMatrix));


		// 当たり判定
		if (IsCollision(segment, plane) == true) {
			segment_color = RED;
		}
		else {
			segment_color = WHITE;
		}


		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldViewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segment_color);

		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, WHITE);

		//DrawSphere(sphere1, sphere1WorldViewProjectionMatrix, viewportMatrix, sphere1_color);

#ifdef _DEBUG


		ImGui::Begin("camera");
		ImGui::DragFloat3("scale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("Translation", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("rotate", &cameraRotation.x, 0.01f);
		ImGui::DragFloat("translateSpeed", &cameraTranslateSpeed, 0.01f);
		ImGui::DragFloat("rotateSpeed", &cameraRotateSpeed, 0.01f);

		ImGui::End();

		ImGui::Begin("Object");
		ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f);

		ImGui::DragFloat3("plane.normal", &plane.normal.x, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::DragFloat("plane.distance", &plane.distance, 0.01f);
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
