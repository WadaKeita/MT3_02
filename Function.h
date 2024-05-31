#pragma once
#define _USE_MATH_DEFINES
#include "math.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "cmath"
#include <cassert>
#include "Novice.h"

/// <summary>
/// 球
/// </summary>
struct Sphere {
	Vector3 center;
	float radius;
};

/// <summary>
/// 直線
/// </summary>
struct Line {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

/// <summary>
/// 半直線
/// </summary>
struct Ray {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

/// <summary>
/// 線分
/// </summary>
struct Segment {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

/// <summary>
/// 平面
/// </summary>
struct Plane {
	Vector3 normal;	// !< 法線
	float distance;	// !< 距離
};

/// <summary>
/// 三角形
/// </summary>
struct Triangle {
	Vector3 vertices[3]; //!< 頂点
};

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ（ノルム）
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列
Matrix4x4 MakeOrthographMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

/// <summary>
/// グリッド描画
/// </summary>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

/// <summary>
/// 球描画
/// </summary>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

float Clamp(float x, float a, float b);

/// <summary>
/// 正射影ベクトル
/// </summary>
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 最近接点
/// </summary>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// 衝突判定：球と球
/// </summary>
bool IsCollision(const Sphere& s1, const Sphere& s2);

/// <summary>
/// 衝突判定：平面と球
/// </summary>
bool IsCollision(const Sphere& sphere, const Plane& plane);

Vector3 Perpendicular(const Vector3& vector);

/// <summary>
/// 平面描画
/// </summary>
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// 衝突判定：線分と平面
/// </summary>
bool IsCollision(const Segment& segment, const Plane& plane);

/// <summary>
/// 直線と平面
/// </summary>
bool IsCollision(const Line& line, const Plane& plane);

/// <summary>
/// 衝突判定：半直線と平面
/// </summary>
bool IsCollision(const Ray& ray, const Plane& plane);

/// <summary>
/// 衝突判定：三角形と線
/// </summary>
bool IsCollision(const Triangle& triangle, const Segment& segment);

/// <summary>
/// 三角形描画
/// </summary>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);