#include "Function.h"

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {

	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
};

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
};

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;

	return result;
};

// 内積
float Dot(const Vector3& v1, const Vector3& v2) { return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z); };

// 長さ（ノルム）
float Length(const Vector3& v) { return sqrtf(Dot(v, v)); };

// 正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 result;

	result.x = v.x / Length(v);
	result.y = v.y / Length(v);
	result.z = v.z / Length(v);

	return result;
};

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) { return { {v1.y * v2.z - v1.z * v2.y}, {v1.z * v2.x - v1.x * v2.z}, {v1.x * v2.y - v1.y * v2.x} }; };

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int column = 0; column < 4; column++) {
		for (int row = 0; row < 4; row++) {

			result.m[column][row] = (m1.m[column][0] * m2.m[0][row]) + (m1.m[column][1] * m2.m[1][row]) + (m1.m[column][2] * m2.m[2][row]) + (m1.m[column][3] * m2.m[3][row]);
		}
	}

	return result;
};

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = { 0 };

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
};

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {
		{{1, 0, 0, 0}, {0, std::cos(radian), std::sin(radian), 0}, {0, -std::sin(radian), std::cos(radian), 0}, {0, 0, 0, 1}},
	};

	return result;
};

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {
		{{std::cos(radian), 0, -std::sin(radian), 0}, {0, 1, 0, 0}, {std::sin(radian), 0, std::cos(radian), 0}, {0, 0, 0, 1}},
	};

	return result;
};

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {
		{{std::cos(radian), std::sin(radian), 0, 0}, {-std::sin(radian), std::cos(radian), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	};

	return result;
};

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = { 0 };

	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
};

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = { 0 };

	result = Multiply(MakeScaleMatrix(scale), Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z))));
	result = Multiply(result, MakeTranslateMatrix(translate));

	return result;
};

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {
		{
		 {1.0f / (aspectRatio * std::tan(fovY / 2.0f)), 0, 0, 0},
		 {0, 1.0f / std::tan(fovY / 2.0f), 0, 0},
		 {0, 0, farClip / (farClip - nearClip), 1},
		 {0, 0, (-nearClip * farClip) / (farClip - nearClip), 0},
		 },
	};
	return result;
};

// 正射影行列
Matrix4x4 MakeOrthographMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {
		{
		 {2.0f / (right - left), 0, 0, 0},
		 {0, 2.0f / (top - bottom), 0, 0},
		 {0, 0, 1.0f / (farClip - nearClip), 0},
		 {(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1},
		 },
	};
	return result;
};

// ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {
		{
		 {width / 2.0f, 0, 0, 0},
		 {0, -(height / 2.0f), 0, 0},
		 {0, 0, maxDepth - minDepth, 0},
		 {left + (width / 2.0f), top + (height / 2.0f), minDepth, 1},
		 },
	};
	return result;
};

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};

	result.x = (vector.x * matrix.m[0][0]) + (vector.y * matrix.m[1][0]) + (vector.z * matrix.m[2][0]) + (1.0f * matrix.m[3][0]);
	result.y = (vector.x * matrix.m[0][1]) + (vector.y * matrix.m[1][1]) + (vector.z * matrix.m[2][1]) + (1.0f * matrix.m[3][1]);
	result.z = (vector.x * matrix.m[0][2]) + (vector.y * matrix.m[1][2]) + (vector.z * matrix.m[2][2]) + (1.0f * matrix.m[3][2]);
	float w = (vector.x * matrix.m[0][3]) + (vector.y * matrix.m[1][3]) + (vector.z * matrix.m[2][3]) + (1.0f * matrix.m[3][3]);
	//assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
};

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;

	float x = 1 / (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]

		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]

		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]

		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]

		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]

		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]

		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]

		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	result.m[0][0] = ((m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[1][3] * m.m[2][1] * m.m[3][2]) - (m.m[1][3] * m.m[2][2] * m.m[3][1]) -
		(m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2])) *
		x;

	result.m[0][1] = ((-m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[2][1] * m.m[3][2]) + (m.m[0][3] * m.m[2][2] * m.m[3][1]) +
		(m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2])) *
		x;

	result.m[0][2] = ((m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[3][2]) - (m.m[0][3] * m.m[1][2] * m.m[3][1]) -
		(m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2])) *
		x;

	result.m[0][3] = ((-m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) - (m.m[0][3] * m.m[1][1] * m.m[2][2]) + (m.m[0][3] * m.m[1][2] * m.m[2][1]) +
		(m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2])) *
		x;

	result.m[1][0] = ((-m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[1][3] * m.m[2][0] * m.m[3][2]) + (m.m[1][3] * m.m[2][2] * m.m[3][0]) +
		(m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2])) *
		x;

	result.m[1][1] = ((m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) + (m.m[0][3] * m.m[2][0] * m.m[3][2]) - (m.m[0][3] * m.m[2][2] * m.m[3][0]) -
		(m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2])) *
		x;

	result.m[1][2] = ((-m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) - (m.m[0][3] * m.m[1][0] * m.m[3][2]) + (m.m[0][3] * m.m[1][2] * m.m[3][0]) +
		(m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2])) *
		x;

	result.m[1][3] = ((m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) + (m.m[0][3] * m.m[1][0] * m.m[2][2]) - (m.m[0][3] * m.m[1][2] * m.m[2][0]) -
		(m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2])) *
		x;

	result.m[2][0] = ((m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[1][3] * m.m[2][0] * m.m[3][1]) - (m.m[1][3] * m.m[2][1] * m.m[3][0]) -
		(m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1])) *
		x;

	result.m[2][1] = ((-m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) - (m.m[0][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[2][1] * m.m[3][0]) +
		(m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1])) *
		x;

	result.m[2][2] = ((m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) + (m.m[0][3] * m.m[1][0] * m.m[3][1]) - (m.m[0][3] * m.m[1][1] * m.m[3][0]) -
		(m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1])) *
		x;

	result.m[2][3] = ((-m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) - (m.m[0][3] * m.m[1][0] * m.m[2][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0]) +
		(m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1])) *
		x;

	result.m[3][0] = ((-m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) - (m.m[1][2] * m.m[2][0] * m.m[3][1]) + (m.m[1][2] * m.m[2][1] * m.m[3][0]) +
		(m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1])) *
		x;

	result.m[3][1] = ((m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) + (m.m[0][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[2][1] * m.m[3][0]) -
		(m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1])) *
		x;

	result.m[3][2] = ((-m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) - (m.m[0][2] * m.m[1][0] * m.m[3][1]) + (m.m[0][2] * m.m[1][1] * m.m[3][0]) +
		(m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1])) *
		x;

	result.m[3][3] = ((m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) + (m.m[0][2] * m.m[1][0] * m.m[2][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0]) -
		(m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1])) *
		x;

	return result;
};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSobdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSobdivision);
	Vector3 linePos[2]{};
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSobdivision; ++xIndex) {
		linePos[0] = { -kGridHalfWidth, 0.0f, kGridHalfWidth - (kGridEvery * xIndex) };
		linePos[1] = { kGridHalfWidth, 0.0f, kGridHalfWidth - (kGridEvery * xIndex) };

		for (uint32_t i = 0; i < 2; i++) {
			Vector3 ndcVertex = Transform(linePos[i], viewProjectionMatrix);
			linePos[i] = Transform(ndcVertex, viewportMatrix);
		}
		if (xIndex == 5) {
			Novice::DrawLine(int(linePos[0].x), int(linePos[0].y), int(linePos[1].x), int(linePos[1].y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(linePos[0].x), int(linePos[0].y), int(linePos[1].x), int(linePos[1].y), 0xAAAAAAFF);
		}
	}
	// 左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSobdivision; ++zIndex) {
		linePos[0] = { kGridHalfWidth - (kGridEvery * zIndex), 0.0f, -kGridHalfWidth };
		linePos[1] = { kGridHalfWidth - (kGridEvery * zIndex), 0.0f, kGridHalfWidth };

		for (uint32_t i = 0; i < 2; i++) {
			Vector3 ndcVertex = Transform(linePos[i], viewProjectionMatrix);
			linePos[i] = Transform(ndcVertex, viewportMatrix);
		}
		if (zIndex == 5) {
			Novice::DrawLine(int(linePos[0].x), int(linePos[0].y), int(linePos[1].x), int(linePos[1].y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(linePos[0].x), int(linePos[0].y), int(linePos[1].x), int(linePos[1].y), 0xAAAAAAFF);
		}
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 10; // 分割数
	const float kLonEvery = (2.0f * (float)M_PI) / kSubdivision; // 経度分割1つ分の角度
	const float kLatEvery = (float)M_PI / kSubdivision; // 緯度分割1つ分の角度

	// 緯度の方向に分割 -π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -(float)M_PI / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 緯度方向に分割 0 ～ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			// world座標系でのa,b,cを求める
			Vector3 a, b, c;

			a = { sphere.radius * cosf(lat) * cosf(lon), sphere.radius * sinf(lat), sphere.radius * cosf(lat) * sin(lon) };
			b = { sphere.radius * cosf(lat + kLatEvery) * cosf(lon), sphere.radius * sinf(lat + kLatEvery), sphere.radius * cosf(lat + kLatEvery) * sin(lon) };
			c = { sphere.radius * cosf(lat) * cosf(lon + kLonEvery), sphere.radius * sinf(lat), sphere.radius * cosf(lat) * sin(lon + kLonEvery) };

			// a,b,cをScreen座標系まで変換
			Vector3 ndcVertex = Transform(a, viewProjectionMatrix);
			a = Transform(ndcVertex, viewportMatrix);

			ndcVertex = Transform(b, viewProjectionMatrix);
			b = Transform(ndcVertex, viewportMatrix);

			ndcVertex = Transform(c, viewProjectionMatrix);
			c = Transform(ndcVertex, viewportMatrix);

			// ab,bcで線を引く
			Novice::DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, color);
			Novice::DrawLine((int)a.x, (int)a.y, (int)c.x, (int)c.y, color);
		}
	}
}

float Clamp(float x, float a, float b) {
	if (a < b) {
		if (x < a) {
			return a;
		}
		else if (x > b) {
			return b;
		}
		else {
			return x;
		}
	}
	else if (a > b) {
		if (x > a) {
			return a;
		}
		else if (x < b) {
			return b;
		}
		else {
			return x;
		}
	}
	return 0;
}

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


bool IsCollision(const Sphere& s1, const Sphere& s2) {
	// 2つの球の中心点間の距離を求める
	float distance = Length({ s2.center.x - s1.center.x,s2.center.y - s1.center.y,s2.center.z - s1.center.z });
	// 半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

	float k = Dot(plane.normal, sphere.center) - plane.distance;

	k = k < 0 ? -k : k; // kの絶対値をとる

	if (k <= sphere.radius) {
		return true;
	}
	return false;
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return{ -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[2].x, (int)points[2].y, (int)points[1].x, (int)points[1].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[3].x, (int)points[3].y, (int)points[0].x, (int)points[0].y, color);
}


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