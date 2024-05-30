#include <Novice.h>
#include "Vector3.h"
#include "Function.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

const char kWindowTitle[] = "LD2A_02_ワダ_ケイタ";

/// <summary>
/// 三角形
/// </summary>
struct Triangle {
	Vector3 vertices[3]; //!< 頂点
};

/// <summary>
/// 衝突判定：三角形と線
/// </summary>
/// <param name="triangle"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment) {
	// 三角形の頂点から平面を求める
	Plane plane;
	Vector3 v1, v2;
	v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);

	plane.normal = Normalize(Cross(v1, v2));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	// 線と平面が当たっているか
	if (IsCollision(segment, plane) == true) {

		float dot = Dot(plane.normal, segment.diff);
		// 垂直=平行であるので、衝突しているはずがない
		if (dot == 0.0f) {
			return false;
		}
		// tを求める
		float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

		Vector3 point = Multiply(t, segment.diff);
		point = Add(point, segment.origin);

		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(point, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(point, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(point, triangle.vertices[0]));

		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {

			return true;
		}
	}
	return false;
}

/// <summary>
/// 三角形描画
/// </summary>
/// <param name="triangle"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewortMatrix"></param>
/// <param name="color"></param>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 points[3];
	for (int32_t index = 0; index < 3; index++) {
		points[index] = Transform(Transform(triangle.vertices[index], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[2].x, (int)points[2].y, (int)points[0].x, (int)points[0].y, color);
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


	//Plane plane{ {0.0f,1.0f,0.0f},1.0f };
	Triangle triangle{
		{
			{1.0f,0.0f,0.0f},
			{-1.0f,0.0f,0.0f},
			{0.0f,1.5f,0.0f}
		}
	};


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
		if (IsCollision(triangle, segment) == true) {
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

		DrawTriangle(triangle, worldViewProjectionMatrix, viewportMatrix, WHITE);

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


		ImGui::DragFloat3("triangle.vertices[0]", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("triangle.vertices[1]", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("triangle.vertices[2]", &triangle.vertices[2].x, 0.01f);

		//ImGui::DragFloat3("plane.normal", &plane.normal.x, 0.01f);
		//plane.normal = Normalize(plane.normal);
		//ImGui::DragFloat("plane.distance", &plane.distance, 0.01f);
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
