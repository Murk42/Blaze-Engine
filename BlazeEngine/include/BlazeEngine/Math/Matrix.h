#pragma once
#include "BlazeEngine/Math/Vector.h"
#include "BlazeEngine/Math/Quaternion.h"

namespace Blaze
{
	template<typename T, size_t Sx, size_t Sy>
	struct Matrix
	{
		std::array<std::array<T, Sx>, Sy> arr;

		constexpr Matrix()
			: arr({ })
		{
		}						
		constexpr Matrix(const std::array<T, Sx * Sy>& values)
		{
			memcpy(&arr, &values, sizeof(T) * Sx * Sy);
		}		
		constexpr Matrix(T* ptr)
		{
			for (size_t i = 0; i != Sx * Sy; ++i)
				arr[i] = ptr[i];
		}

		constexpr Matrix<T, Sy, Sx> Transposed() const
		{
			Matrix<T, Sy, Sx> mat = *this;			
			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sy; ++j)
					mat.arr[j][i] = arr[i][j];

			return mat;
		}

		template<size_t Sx2>
		constexpr Matrix<T, Sx2, Sy> operator* (const Matrix<T, Sx2, Sx>& m) const
		{
			Matrix<T, Sx2, Sy> out = Matrix<T, Sx2, Sy>(0.0f);
			
			for (size_t i = 0; i < Sy; ++i)
				for (size_t k = 0; k < Sx; ++k)				
					for (size_t j = 0; j < Sx2; ++j)					
						out.arr[i][j] += arr[i][k] * m.arr[k][j];

			return out;
		}
		constexpr Matrix operator* (const T& v) const
		{
			Matrix mat = *this;

			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					mat.arr[i][j] *= v;

			return mat;
		}
		constexpr Vector<T, Sy> operator* (const Vector<T, Sy>& v) const
		{
			Vector<T, Sy> out;

			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != Sx; ++j)
					out.arr[i] += v.arr[j] * arr[i][j];

			return out;
		}

		constexpr Matrix& operator*= (const T& v)
		{
			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					arr[i][j] *= v;

			return *this;
		}
		constexpr Matrix& operator*= (const Matrix<T, Sx, Sy>& m)
		{
			Matrix res;
			
			for (size_t i = 0; i < Sy; ++i)
				for (size_t k = 0; k < Sx; ++k)
					for (size_t j = 0; j < Sx; ++j)
						res.arr[i][j] += arr[i][k] * m.arr[k][j];

			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					arr[i][j] = res.arr[i][j];			

			return *this;
		}

		constexpr bool operator== (const Matrix<T, Sx, Sy>& m) const 
		{ 
			return memcmp(arr, m.arr, sizeof(T) * Sx * Sy) == 0;
		}
		constexpr bool operator!= (const Matrix<T, Sx, Sy>& m) const 
		{
			return memcmp(arr, m.arr, sizeof(T) * Sx * Sy) != 0; 
		}

		constexpr std::array<T, Sy>& operator[](size_t index)
		{
			return arr[index];
		}
		constexpr const std::array<T, Sy>& operator[](size_t index) const
		{
			return arr[index];
		}
	};

	template<typename T, size_t S>
	struct Matrix<T, S, S>
	{
		static constexpr size_t Sx = S;
		static constexpr size_t Sy = S;
		std::array<std::array<T, Sy>, Sx> arr;

		constexpr Matrix()
			: arr({ })
		{
		}
		constexpr Matrix(const std::array<T, Sx * Sy>& values)
		{
			memcpy(&arr, &values, sizeof(T) * Sx * Sy);
		}
		constexpr Matrix(T* ptr)
		{
			for (size_t i = 0; i != Sx * Sy; ++i)
				arr[i] = ptr[i];
		}

		constexpr void Transpose()
		{
			for (size_t i = 1; i < Sy; ++i)
				for (size_t j = 0; j < i; ++j)
					std::swap(this->arr[i][j], this->arr[j][i]);
		}
		constexpr Matrix<T, Sx, Sy> Transposed() const
		{
			Matrix<T, Sx, Sy> mat = *this;
			mat.Transpose();
			return mat;
		}

		template<size_t S2>
		constexpr Matrix<T, S2, Sy> operator* (const Matrix<T, S2, Sy>& m) const
		{
			Matrix<T, S2, Sy> out;
		
			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != S2; ++j)
					for (size_t k = 0; k != Sx; ++k)
						out.arr[i][j] += arr[i][k] * m.arr[k][j];

			return out;
		}
		constexpr Matrix operator* (const T& v) const
		{
			Matrix mat = *this;
			
			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
				mat.arr[i] *= v;

			return mat;
		}
		constexpr Vector<T, Sy> operator* (const Vector<T, Sy>& v) const
		{
			Vector<T, Sy> out;

			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != Sx; ++j)
					out[i] += v[j] * arr[i][j];

			return out;
		}

		constexpr Matrix& operator*= (const T& v)
		{
			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != Sx; ++j)
					arr[i][j] *= v;

			return *this;
		}
		constexpr Matrix& operator*= (const Matrix<T, Sx, Sy>& m)
		{
			Matrix<T, S, S> res;			
			
			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					for (size_t k = 0; k < Sx; ++k)
						res.arr[i][j] += arr[i][k] * m.arr[k][j];

			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					arr[i][j] = res.arr[i][j];

			return *this;
		}

		constexpr bool operator== (const Matrix<T, Sx, Sy>& m) const 
		{ 
			return memcmp(&arr, &m.arr, sizeof(T) * Sx * Sy) == 0; 
		}
		constexpr bool operator!= (const Matrix<T, Sx, Sy>& m) const 
		{ 
			return memcmp(&arr, &m.arr, sizeof(T) * Sx * Sy) != 0; 
		}		

		constexpr std::array<T, Sy>& operator[](size_t index)
		{
			return arr[index];
		}
		constexpr const std::array<T, Sy>& operator[](size_t index) const
		{
			return arr[index];
		}

		static constexpr Matrix<T, Sx, Sy> Identity()
		{
			Matrix<T, Sx, Sy> mat;

			for (int i = 0; i < Sx; ++i)
				mat.arr[i][i] = 1;

			return mat;
		}
	};

	template<typename T>
	struct Matrix<T, 4, 4>
	{
		static constexpr size_t Sx = 4;
		static constexpr size_t Sy = 4;
		std::array<std::array<T, Sy>, Sx> arr;

		constexpr Matrix()
			: arr({ })
		{
		}
		constexpr Matrix(const std::array<T, Sx* Sy>& values)
		{
			memcpy(&arr, &values, sizeof(T) * Sx * Sy);
		}
		constexpr Matrix(T* ptr)
		{
			for (size_t i = 0; i != Sx * Sy; ++i)
				arr[i] = ptr[i];
		}

		constexpr void Transpose()
		{
			for (size_t i = 1; i < Sy; ++i)
				for (size_t j = 0; j < i; ++j)
					std::swap(this->arr[i][j], this->arr[j][i]);
		}
		constexpr Matrix<T, Sx, Sy> Transposed() const
		{
			Matrix<T, Sx, Sy> mat = *this;
			mat.Transpose();
			return mat;
		}
		
		static constexpr Matrix TranslationMatrix(Vec3<T> v)
		{
			return Matrix<T, 4, 4>({
				1, 0, 0, v.x,
				0, 1, 0, v.y,
				0, 0, 1, v.z,
				0, 0, 0, 1,
				});
		}		
		static constexpr Matrix ScalingMatrix(Vec3<T> v)
		{
			return Matrix<T, 4, 4>({
				v.x, 0, 0, 0,
				0, v.y, 0, 0,
				0, 0, v.z, 0,
				0, 0, 0, 1,
				});
		}		
		static constexpr Matrix ScalingMatrix(T v)
		{
			return Matrix<T, 4, 4>({
				v, 0, 0, 0,
				0, v, 0, 0,
				0, 0, v, 0,
				0, 0, 0, 1,
				});
		}
		static constexpr Matrix RotationMatrix(Quat<T> q)
		{
			const T xx = q.x * q.x;
			const T yy = q.y * q.y;
			const T zz = q.z * q.z;
			const T xy = q.x * q.y;
			const T xz = q.x * q.z;
			const T yz = q.y * q.z;
			const T wx = q.w * q.x;
			const T wy = q.w * q.y;
			const T wz = q.w * q.z;
			return Matrix({
				1 - 2 * yy - 2 * zz, 2 * xy - 2 * wz, 2 * xz + 2 * wy, 0,
				2 * xy + 2 * wz, 1 - 2 * xx - 2 * zz, 2 * yz - 2 * wx, 0,
				2 * xz - 2 * wy, 2 * yz + 2 * wx, 1 - 2 * xx - 2 * yy, 0,
				0, 0, 0, 1
				});
		}
		static constexpr Matrix RotationMatrixAxisX(T radians)
		{
			const T sin = Math::Sin(radians);
			const T cos = Math::Cos(radians);
			return Matrix({
				1,   0,   0,   0,
				0, cos,-sin,   0,
				0, sin, cos,   0,
				0,   0,   0,   1
				});
		}
		static constexpr Matrix RotationMatrixAxisY(T radians)
		{
			const T sin = Math::Sin(radians);
			const T cos = Math::Cos(radians);
			return Matrix({
				 cos,   0, sin,  0,
				   0,   1,   0,  0,
				-sin,   0, cos,  0,
				   0,   0,   0,  1
				});
		}
		static constexpr Matrix RotationMatrixAxisZ(T radians)
		{
			const T sin = Math::Sin(radians);
			const T cos = Math::Cos(radians);
			return Matrix({
				cos,-sin,   0,   0,
				sin, cos,   0,   0,
				  0,   0,   1,   0,
				  0,   0,   0,   1
				});
		}
		
		static constexpr Matrix<T, 4, 4> OrthographicMatrix(T left, T right, T bottom, T top, T near, T far)
		{
			T rml = right - left;
			T tmb = top - bottom;
			T fmn = far - near;
			Matrix m({
				T(2) / rml, T(0), T(0), T(0),
				T(0), T(2) / tmb, T(0), T(0),
				T(0), T(0), T(2) / fmn, T(0),
				-(right + left) / rml, -(top + bottom) / tmb, (far + near) / fmn, T(1),
				});
			m.Transpose();
			return m;
		}
		static constexpr Matrix<T, 4, 4> PerspectiveMatrix(T fov, T aspectRatio, T near, T far)
		{
			T S = T(1) / Math::Tan(fov / T(2));

			return Matrix<T, 4, 4>({
				S	, T(0)				, T(0)							, T(0),
				T(0), S * aspectRatio	, T(0)							, T(0),
				T(0), T(0)				, (far + near) / (far - near)	,-2 * near * far / (far - near),
				T(0), T(0)				, 1	, 0
				});			
		}

		template<size_t S2>
		constexpr Matrix<T, S2, Sy> operator* (const Matrix<T, S2, Sy>& m) const
		{
			Matrix<T, S2, Sy> out;

			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != S2; ++j)
					for (size_t k = 0; k != Sx; ++k)
						out.arr[i][j] += arr[i][k] * m.arr[k][j];

			return out;
		}
		constexpr Matrix operator* (const T& v) const
		{
			Matrix mat = *this;

			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					mat.arr[i] *= v;

			return mat;
		}
		constexpr Vector<T, Sy> operator* (const Vector<T, Sy>& v) const
		{
			Vector<T, Sy> out;

			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != Sx; ++j)
					out[i] += v[j] * arr[i][j];

			return out;
		}

		constexpr Matrix& operator*= (const T& v)
		{
			for (size_t i = 0; i != Sy; ++i)
				for (size_t j = 0; j != Sx; ++j)
					arr[i][j] *= v;

			return *this;
		}
		constexpr Matrix& operator*= (const Matrix<T, Sx, Sy>& m)
		{
			Matrix<T, Sx, Sy> res;

			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					for (size_t k = 0; k < Sx; ++k)
						res.arr[i][j] += arr[i][k] * m.arr[k][j];

			for (size_t i = 0; i < Sy; ++i)
				for (size_t j = 0; j < Sx; ++j)
					arr[i][j] = res.arr[i][j];

			return *this;
		}

		constexpr bool operator== (const Matrix<T, Sx, Sy>& m) const
		{
			return memcmp(&arr, &m.arr, sizeof(T) * Sx * Sy) == 0;
		}
		constexpr bool operator!= (const Matrix<T, Sx, Sy>& m) const
		{
			return memcmp(&arr, &m.arr, sizeof(T) * Sx * Sy) != 0;
		}

		constexpr std::array<T, Sy>& operator[](size_t index)
		{
			return arr[index];
		}
		constexpr const std::array<T, Sy>& operator[](size_t index) const
		{
			return arr[index];
		}

		static constexpr Matrix<T, Sx, Sy> Identity()
		{
			Matrix<T, Sx, Sy> mat;

			for (int i = 0; i < Sx; ++i)
				mat.arr[i][i] = 1;

			return mat;
		}
	};
	
	template<typename T>
	using Mat2 = Matrix<T, 2, 2>;
	template<typename T>
	using Mat3 = Matrix<T, 3, 3>;
	template<typename T>
	using Mat4 = Matrix<T, 4, 4>;

	using Mat2f = Mat2<float>;
	using Mat2d = Mat2<double>;
	using Mat3f = Mat3<float>;
	using Mat3d = Mat3<double>;
	using Mat4f = Mat4<float>;
	using Mat4d = Mat4<double>;
	
}