#pragma once 

namespace Blaze
{
	namespace Math
	{
		template<typename T>
		constexpr T Sqrt(const T&);
	}

	template<typename T, size_t S>
	struct Vector;

	template<size_t S>
	using Veci = Vector<int, S>;
	template<size_t S>
	using Vecf = Vector<float, S>;
	template<size_t S>
	using Vecd = Vector<double, S>;

	template<typename T>
	struct Vector<T, 2>
	{
		union {
			struct {
				T x, y;
			};

			T arr[2]{ };
		};

		constexpr Vector() = default;
		constexpr Vector(const T& x, const T& y)
			: x(x), y(y)
		{
		}
		constexpr explicit Vector(const T& v) 
			: x(v), y(v) 
		{ 
		}		
		template<typename T2>
		constexpr Vector(const Vector<T2, 2>& v)
			: x(v.x), y(v.y)
		{			
		}

		constexpr Vector operator-() const { return Vector(-x, -y); }

		template<typename T2> constexpr Vector operator+(const Vector<T2, 2>& b) const { return Vector(x + b.x, y + b.y); }
		template<typename T2> constexpr Vector operator-(const Vector<T2, 2>& b) const { return Vector(x - b.x, y - b.y); }
		template<typename T2> constexpr Vector operator*(const Vector<T2, 2>& v) const { return Vector(x * v.x, y * v.y); }
		template<typename T2> constexpr Vector operator/(const Vector<T2, 2>& v) const { return Vector(x / v.x, y / v.y); }
		template<typename T2> constexpr Vector operator*(const T2& v) const { return Vector(x * v, y * v); }
		template<typename T2> constexpr Vector operator/(const T2& v) const { return Vector(x / v, y / v); }

		template<typename T2> constexpr void operator+= (const Vector<T2, 2>& v) { x += v.x; y += v.y; }
		template<typename T2> constexpr void operator-= (const Vector<T2, 2>& v) { x -= v.x; y -= v.y; }
		template<typename T2> constexpr void operator*= (const Vector<T2, 2>& v) { x *= v.x; y *= v.y; }
		template<typename T2> constexpr void operator/= (const Vector<T2, 2>& v) { x /= v.x; y /= v.y; }
		template<typename T2> constexpr void operator*= (const T2& v) { x *= v; y *= v; }
		template<typename T2> constexpr void operator/= (const T2& v) { x /= v; y /= v; }

		template<typename T2> constexpr bool operator== (const Vector<T2, 2>& v) const { return x == v.x && y == v.y; }
		template<typename T2> constexpr bool operator!= (const Vector<T2, 2>& v) const { return x != v.x || y != v.y; }
		
		template<typename T2> constexpr void operator=(const Vector<T2, 2>& v) { x = v.x; y = v.y; }

		constexpr inline T Lenght() const { return Math::Sqrt<T>(x * x + y * y); }
		constexpr inline T SqrLenght() const { return x * x + y * y; }
		constexpr inline void Normalise()
		{
			const T lenght = Lenght();
			x /= lenght;
			y /= lenght;
		}
		constexpr inline Vector Normalised()
		{
			const T lenght = Lenght();
			return Vector(x / lenght, y / lenght);
		}
	};

	template<typename T>
	using Vec2 = Vector<T, 2>;
	using Vec2i = Vec2<int>;
	using Vec2u = Vec2<unsigned>;
	using Vec2f = Vec2<float>;
	using Vec2d = Vec2<double>;

	template<typename T>
	struct Vector<T, 3>
	{
		union {
			struct {
				T x, y, z;
			};
			struct {
				Vec2<T> xy;
			};
			T arr[3]{ };
		};

		constexpr Vector()
			: x(0), y(0), z(0)
		{
		}
		constexpr Vector(const T& x, const T& y, const T& z)
			: x(x), y(y), z(z)
		{
		}
		constexpr explicit Vector(const T& v) 
			: x(v), y(v), z(v) 
		{ 
		}
		template<typename T2>
		constexpr Vector(const Vector<T2, 3>& v)
			: x(v.x), y(v.y), z(v.z)
		{			
		}		

		constexpr Vector operator-() const { return Vector(-x, -y, -z); }

		template<typename T2> constexpr Vector operator+(const Vector<T2, 3>& b) const { return Vector(x + b.x, y + b.y, z + b.z); }
		template<typename T2> constexpr Vector operator-(const Vector<T2, 3>& b) const { return Vector(x - b.x, y - b.y, z - b.z); }
		template<typename T2> constexpr Vector operator*(const Vector<T2, 3>& v) const { return Vector(x * v.x, y * v.y, z * v.z); }
		template<typename T2> constexpr Vector operator/(const Vector<T2, 3>& v) const { return Vector(x / v.x, y / v.y, z / v.z); }
		template<typename T2> constexpr Vector operator*(const T2& v) const { return Vector(x * v, y * v, z * v); }
		template<typename T2> constexpr Vector operator/(const T2& v) const { return Vector(x / v, y / v, z / v); }

		template<typename T2> constexpr void operator+= (const Vector<T2, 3>& v) { x += v.x; y += v.y; z += v.z; }
		template<typename T2> constexpr void operator-= (const Vector<T2, 3>& v) { x -= v.x; y -= v.y; z -= v.z; }
		template<typename T2> constexpr void operator*= (const Vector<T2, 3>& v) { x *= v.x; y *= v.y; z *= v.z; }
		template<typename T2> constexpr void operator/= (const Vector<T2, 3>& v) { x /= v.x; y /= v.y; z /= v.z; }
		template<typename T2> constexpr void operator*= (const T2& v) { x *= v; y *= v; z *= v; }
		template<typename T2> constexpr void operator/= (const T2& v) { x /= v; y /= v; z /= v; }

		template<typename T2> constexpr bool operator== (const Vector<T2, 3>& v) const { return x == v.x && y == v.y && z == v.z; }
		template<typename T2> constexpr bool operator!= (const Vector<T2, 3>& v) const { return x != v.x || y != v.y || z != v.z; }
		
		template<typename T2> constexpr void operator=(const Vector<T2, 3>& v) { x = T(v.x); y = T(v.y); z = T(v.z); }

		constexpr inline T Lenght() const { return Math::Sqrt<T>(x * x + y * y + z * z); }
		constexpr inline T SqrLenght() const { return x * x + y * y + z * z; }
		constexpr inline void Normalise()
		{
			const T lenght = Lenght();
			x /= lenght;
			y /= lenght;
			z /= lenght;
		}
		constexpr inline Vector Normalised()
		{
			const T lenght = Lenght();
			return Vector(x / lenght, y / lenght, z / lenght);
		}
	};

	template<typename T>
	using Vec3 = Vector<T, 3>;
	using Vec3i = Vec3<int>;
	using Vec3u = Vec3<unsigned>;
	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;

	template<typename T>
	struct Vector<T, 4>
	{
		union {
			struct {
				T x, y, z, w;
			};
			struct {
				Vec2<T> xy;
			};
			struct {
				Vec3<T> xyz;
			};
			T arr[4]{ };
		};

		constexpr Vector()
		{
		}
		constexpr Vector(const T& x, const T& y, const T& z, const T& w)
			: x(x), y(y), z(z), w(w)
		{
		}
		constexpr explicit Vector(const T& v) 
			: x(v), y(v), z(v), w(v) 
		{ 
		}		
		template<typename T2>
		constexpr Vector(const Vector<T2, 4>& v)
			: x(v.x), y(v.y), z(v.y), w(v.w)
		{			
		}	

		constexpr Vector operator-() const { return Vector(-x, -y, -z, -w); }

		template<typename T2> constexpr Vector operator+(const Vector<T2, 4>& b) const { return Vector(x + b.x, y + b.y, z + b.z, w + b.w); }
		template<typename T2> constexpr Vector operator-(const Vector<T2, 4>& b) const { return Vector(x - b.x, y - b.y, z - b.z, w - b.w); }
		template<typename T2> constexpr Vector operator*(const Vector<T2, 4>& v) const { return Vector(x * v.x, y * v.y, z * v.z, w * v.w); }
		template<typename T2> constexpr Vector operator/(const Vector<T2, 4>& v) const { return Vector(x / v.x, y / v.y, z / v.z, w / v.w); }
		template<typename T2> constexpr Vector operator*(const T2& v) const { return Vector(x * v, y * v, z * v, w * v); }
		template<typename T2> constexpr Vector operator/(const T2& v) const { return Vector(x / v, y / v, z / v, w / v); }

		template<typename T2> constexpr void operator+= (const Vector<T2, 4>& v) { x += v.x; y += v.y; z += v.z; w += v.w; }
		template<typename T2> constexpr void operator-= (const Vector<T2, 4>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
		template<typename T2> constexpr void operator*= (const Vector<T2, 4>& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
		template<typename T2> constexpr void operator/= (const Vector<T2, 4>& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }
		template<typename T2> constexpr void operator*= (const T2& v) { x *= v; y *= v; z *= v; w *= v; }
		template<typename T2> constexpr void operator/= (const T2& v) { x /= v; y /= v; z /= v; w /= v; }

		template<typename T2> constexpr bool operator== (const Vector<T2, 4>& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
		template<typename T2> constexpr bool operator!= (const Vector<T2, 4>& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
		
		template<typename T2> constexpr void operator=(const Vector<T2, 4>& v) { x = v.x; y = v.y; z = v.z; w = v.w; }

		constexpr inline T Lenght() const { return Math::Sqrt<T>(x * x + y * y + z * z + w * w); }
		constexpr inline T SqrLenght() const { return x * x + y * y + z * z + w * w; }
		constexpr inline void Normalise()
		{
			const T lenght = Lenght();
			x /= lenght;
			y /= lenght;
			z /= lenght;
			w /= lenght;
		}
		constexpr inline Vector Normalised()
		{
			const T lenght = Lenght();
			return Vector(x / lenght, y / lenght, z / lenght, w / lenght);
		}
	};

	template<typename T>
	using Vec4 = Vector<T, 4>;
	using Vec4i = Vec4<int>;
	using Vec4u = Vec4<unsigned>;
	using Vec4f = Vec4<float>;
	using Vec4d = Vec4<double>;
}