#ifndef IUVO_MATH
#define IUVO_MATH

#include "IuvoNumbers.h"
#include "IuvoStructs.h"

namespace IuvoMath {

	using namespace IuvoStructs;

	Matrix4x4 IdentityMatrix() {
		Matrix4x4 newMatrix;

		newMatrix.xx = 1.0f;			newMatrix.xy = 0.0f;		newMatrix.xz = 0.0f;			newMatrix.xw = 0.0f;
		newMatrix.yx = 0.0f;			newMatrix.yy = 1.0f;		newMatrix.yz = 0.0f;			newMatrix.yw = 0.0f;
		newMatrix.zx = 0.0f;			newMatrix.zy = 0.0f;		newMatrix.zz = 1.0f;			newMatrix.zw = 0.0f;
		newMatrix.wx = 0.0f;			newMatrix.wy = 0.0f;		newMatrix.wz = 0.0f;			newMatrix.ww = 1.0f;

		return newMatrix;
	}

	Matrix4x4 TranslationMatrix(float translationX, float translationY, float translationZ) {
		Matrix4x4 newMatrix;

		newMatrix.xx = 1.0f;			newMatrix.xy = 0.0f;		newMatrix.xz = 0.0f;			newMatrix.xw = 0.0f;
		newMatrix.yx = 0.0f;			newMatrix.yy = 1.0f;		newMatrix.yz = 0.0f;			newMatrix.yw = 0.0f;
		newMatrix.zx = 0.0f;			newMatrix.zy = 0.0f;		newMatrix.zz = 1.0f;			newMatrix.zw = 0.0f;
		newMatrix.wx = translationX;			newMatrix.wy = translationY;		newMatrix.wz = translationZ;			newMatrix.ww = 1.0f;

		return newMatrix;
	}

	Matrix4x4 MakeARotationX(float degrees)
	{
		float radians = degrees * (PIE / 180.0f);
		Matrix4x4 m;

		m.xx = 1.0f;					m.xy = 0.0f;				m.xz = 0.0f;					m.xw = 0.0f;
		m.yx = 0.0f;					m.yy = cos(radians);		m.yz = -sin(radians);			m.yw = 0.0f;
		m.zx = 0.0f;					m.zy = sin(radians);		m.zz = cos(radians);			m.zw = 0.0f;
		m.wx = 0.0f;					m.wy = 0.0f;				m.wz = 0.0f;					m.ww = 1.0f;

		return m;
	}

	Matrix4x4 MakeARotationY(float rotateSpeed)
	{
		float radians = rotateSpeed * (PIE / 180.0f);
		Matrix4x4 m;

		m.xx = cos(radians);			m.xy = 0;					m.xz = sin(radians);			m.xw = 0;
		m.yx = 0;						m.yy = 1;					m.yz = 0;						m.yw = 0;
		m.zx = -sin(radians);			m.zy = 0;					m.zz = cos(radians);			m.zw = 0;
		m.wx = 0;						m.wy = 0;					m.wz = 0;						m.ww = 1;

		return m;
	}

	Matrix4x4 MakeARotationZ(float rotateSpeed)
	{
		float radians = rotateSpeed * (PIE / 180.0f);
		Matrix4x4 m;

		m.xx = cos(radians);			m.xy = -sin(radians);		m.xz = 0;						m.xw = 0;
		m.yx = sin(radians);			m.yy = cos(radians);		m.yz = 0;						m.yw = 0;
		m.zx = 0;						m.zy = 0;					m.zz = 1;						m.zw = 0;
		m.wx = 0;						m.wy = 0;					m.wz = 0;						m.ww = 1;

		return m;
	}


	Matrix4x4 OrthonormalInverse(const Matrix4x4& min)
	{
		Matrix4x4 mOut;

		// Transpose the rotation part (upper-left 3x3)
		mOut.xx = min.xx; mOut.xy = min.yx; mOut.xz = min.zx;
		mOut.yx = min.xy; mOut.yy = min.yy; mOut.yz = min.zy;
		mOut.zx = min.xz; mOut.zy = min.yz; mOut.zz = min.zz;

		// Invert the bottom row (translation) 
		mOut.wx = -(min.xx * min.wx + min.xy * min.wy + min.xz * min.wz);
		mOut.wy = -(min.yx * min.wx + min.yy * min.wy + min.yz * min.wz);
		mOut.wz = -(min.zx * min.wx + min.zy * min.wy + min.zz * min.wz);
		mOut.ww = 1; // Always 1 for affine transformation matrices

		return mOut;
	}

	Matrix4x4 MakePerspectiveMatrix(float fovY, float aspectRatio, float nearPlane, float farPlane) {
		Matrix4x4 m;
		float halfFOV = (fovY / 2.0f);
		float yScale = (1.0f / tan(halfFOV));

		float xScale = yScale * aspectRatio;

		m.xx = xScale;			// X axis scaling factor
		m.xy = 0.0f;
		m.xz = 0.0f;
		m.xw = 0.0f;

		m.yx = 0.0f;
		m.yy = yScale;			// Y axis scaling factor
		m.yz = 0.0f;
		m.yw = 0.0f;

		m.zx = 0.0f;
		m.zy = 0.0f;
		m.zz = farPlane / (farPlane - nearPlane);
		m.zw = 1.0f;

		m.wx = 0.0f;
		m.wy = 0.0f;
		m.wz = -(farPlane * nearPlane) / (farPlane - nearPlane);
		m.ww = 0.0f;

		return m;
	}

	Vertex MultiplyMatrixByVertex(const Matrix4x4& m1, const Vertex& m2)
	{
		Vertex result;

		result.position.x = m1.xx * m2.position.x + m1.yx * m2.position.y + m1.zx * m2.position.z + m1.wx * m2.position.w;
		result.position.y = m1.xy * m2.position.x + m1.yy * m2.position.y + m1.zy * m2.position.z + m1.wy * m2.position.w;
		result.position.z = m1.xz * m2.position.x + m1.yz * m2.position.y + m1.zz * m2.position.z + m1.wz * m2.position.w;
		result.position.w = m1.xw * m2.position.x + m1.yw * m2.position.y + m1.zw * m2.position.z + m1.ww * m2.position.w;
		result.uv = m2.uv;
		return result;
	}

	float DotProduct(const Vertex& v1, const Vertex& v2)
	{
		return (v1.position.x * v2.position.x) + (v1.position.y * v2.position.y) + (v1.position.z * v2.position.z) + (v1.position.w * v2.position.w);
	}

	Matrix4x4 MultiplyMatrixByMatrix(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;

		result.xx = DotProduct(Vertex(m1.xx, m1.xy, m1.xz, m1.xw), Vertex(m2.xx, m2.yx, m2.zx, m2.wx));
		result.xy = DotProduct(Vertex(m1.xx, m1.xy, m1.xz, m1.xw), Vertex(m2.xy, m2.yy, m2.zy, m2.wy));
		result.xz = DotProduct(Vertex(m1.xx, m1.xy, m1.xz, m1.xw), Vertex(m2.xz, m2.yz, m2.zz, m2.wz));
		result.xw = DotProduct(Vertex(m1.xx, m1.xy, m1.xz, m1.xw), Vertex(m2.xw, m2.yw, m2.zw, m2.ww));

		result.yx = DotProduct(Vertex(m1.yx, m1.yy, m1.yz, m1.yw), Vertex(m2.xx, m2.yx, m2.zx, m2.wx));
		result.yy = DotProduct(Vertex(m1.yx, m1.yy, m1.yz, m1.yw), Vertex(m2.xy, m2.yy, m2.zy, m2.wy));
		result.yz = DotProduct(Vertex(m1.yx, m1.yy, m1.yz, m1.yw), Vertex(m2.xz, m2.yz, m2.zz, m2.wz));
		result.yw = DotProduct(Vertex(m1.yx, m1.yy, m1.yz, m1.yw), Vertex(m2.xw, m2.yw, m2.zw, m2.ww));

		result.zx = DotProduct(Vertex(m1.zx, m1.zy, m1.zz, m1.zw), Vertex(m2.xx, m2.yx, m2.zx, m2.wx));
		result.zy = DotProduct(Vertex(m1.zx, m1.zy, m1.zz, m1.zw), Vertex(m2.xy, m2.yy, m2.zy, m2.wy));
		result.zz = DotProduct(Vertex(m1.zx, m1.zy, m1.zz, m1.zw), Vertex(m2.xz, m2.yz, m2.zz, m2.wz));
		result.zw = DotProduct(Vertex(m1.zx, m1.zy, m1.zz, m1.zw), Vertex(m2.xw, m2.yw, m2.zw, m2.ww));

		result.wx = DotProduct(Vertex(m1.wx, m1.wy, m1.wz, m1.ww), Vertex(m2.xx, m2.yx, m2.zx, m2.wx));
		result.wy = DotProduct(Vertex(m1.wx, m1.wy, m1.wz, m1.ww), Vertex(m2.xy, m2.yy, m2.zy, m2.wy));
		result.wz = DotProduct(Vertex(m1.wx, m1.wy, m1.wz, m1.ww), Vertex(m2.xz, m2.yz, m2.zz, m2.wz));
		result.ww = DotProduct(Vertex(m1.wx, m1.wy, m1.wz, m1.ww), Vertex(m2.xw, m2.yw, m2.zw, m2.ww));

		return result;
	}

	Vertex PerspectiveDivide(const Vertex& v) {
		Vertex divided = v;
		divided.position.x /= v.position.w;
		divided.position.y /= v.position.w;
		divided.position.z /= v.position.w;
		return divided;
	}

	float VectorDotProduct(const Vertex& v1, const Vertex& v2) {
		return (v1.position.x * v2.position.x) + (v1.position.y * v2.position.y) + (v1.position.z * v2.position.z);
	}

	float VectorDotProduct(const Vertex& v1, const Normal& v2) {
		return (v1.position.x * v2.x) + (v1.position.y * v2.y) + (v1.position.z * v2.z);
	}

	Vertex VectorCrossProduct(const Vertex& v1, const Vertex& v2) {
		float crossX = (v1.position.y * v2.position.z) - (v1.position.z * v2.position.y);
		float crossY = (v1.position.z * v2.position.x) - (v1.position.x * v2.position.z);
		float crossZ = (v1.position.x * v2.position.y) - (v1.position.y * v2.position.x);

		return Vertex(Float4(crossX, crossY, crossZ, 1.0f), Color(), UV());
	}





};

#endif
