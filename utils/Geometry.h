#ifndef GEOMETRY
#define GEOMETRY

#include <array>
#include <type_traits>
#include <cmath>
#include <cassert>

#define EPS (1e-15)

namespace Geometry {
	template<typename Derived, std::size_t N>
	class VectorBase
	{
		protected:
		std::array<double, N> coord;

		public:
		constexpr VectorBase() = default;
		
		template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == N && (std::is_convertible_v<Args, double> && ...)>>
		constexpr VectorBase (Args... args) noexcept:
			coord{static_cast<double>(args)...}
		{}

		constexpr explicit VectorBase (std::array<double, N> coord) noexcept:
			coord(std::move(coord))
		{}

		constexpr VectorBase (std::initializer_list<double> list) noexcept
		{
			assert(N == list.size());
			std::copy(list.begin(), list.end(), coord.begin());
		}

		constexpr VectorBase (const VectorBase &v) = default;
		constexpr VectorBase (VectorBase &&v) = default;

		constexpr VectorBase & operator= (const VectorBase &v) = default;
		constexpr VectorBase & operator= (VectorBase &&v) = default;

		constexpr double operator[] (std::size_t k) const noexcept
		{
			return coord[k];
		}
	
		constexpr double& operator[] (std::size_t k) noexcept
		{
		return coord[k];
		}
		
		constexpr Derived & derived() noexcept
		{
			return static_cast<Derived &>(*this);
		}

		constexpr const Derived & derived() const noexcept
		{
			return static_cast<const Derived &>(*this);
		}
		
		[[nodiscard]] constexpr Derived operator+ () const noexcept
		{
			return derived();
		}

		[[nodiscard]] constexpr Derived operator- () const noexcept
		{
			Derived v(derived());
			for (std::size_t i{}; i < N; ++i) {
				v[i] = -v[i];
			}
			return v;
		}

		[[nodiscard]] constexpr Derived operator+ (const Derived &v) const noexcept
		{	
			Derived res(derived());
			for (std::size_t i{}; i < N; ++i) {
				res[i] += v[i];
			}
			return res;
		}

		[[nodiscard]] constexpr Derived operator- (const Derived &v) const noexcept
		{	
			Derived res(derived());
			for (std::size_t i{}; i < N; ++i) {
				res[i] -= v[i];
			}
			return res;
		}

		[[nodiscard]] constexpr Derived operator* (double c) const noexcept
		{
			Derived res(derived());
			for (std::size_t i{}; i < N; ++i) {
				res[i] *= c;
			}
			return res;
		}

		[[nodiscard]] constexpr Derived operator/ (double c) const noexcept
		{
			Derived res(derived());
			for (std::size_t i{}; i < N; ++i){
				res[i] /= c;
			}
			return res;
		}

		constexpr Derived & operator+= (const Derived &v) noexcept
		{	
			for (std::size_t i{}; i < N; ++i) {
				coord[i] += v[i];
			}
			return derived();
		}

		constexpr Derived & operator-= (const Derived &v) noexcept
		{
			for (std::size_t i{}; i < N; ++i) {
				coord[i] -= v[i];
			}
			return derived();
		}
	
		constexpr Derived & operator*= (double c) noexcept
		{
			for (std::size_t i{}; i < N; ++i){
				coord[i] *= c;
			}
			return derived();
		}

		constexpr Derived & operator/= (double c) noexcept
		{
			for (std::size_t i{}; i < N; ++i){
				coord[i] /= c;
			}
			return derived();
		}
		
		constexpr bool operator==(const Derived &v) const
		{
			return coord == v.coord;
		}

		constexpr bool operator!=(const Derived &v) const
		{
			return coord != v.coord;
		}

		[[nodiscard]] constexpr double mod () const noexcept
		{
			auto const &self = derived();
			double res{};
			for (std::size_t i{}; i < N; ++i) {
				res += self[i]*self[i];
			}
			return std::sqrt(res);
		}

		[[nodiscard]] constexpr Derived unit() const noexcept
		{
			return derived() / mod();
		}

		constexpr Derived & norm() noexcept
		{
			return derived() = unit();
		}
	
		constexpr double X () const noexcept
		{
			static_assert (N >= 1);
			return coord[0];
		}
	
		constexpr double & X () noexcept
		{
			static_assert (N >= 1);
			return coord[0];
		}
	
		constexpr double Y () const noexcept
		{
			static_assert (N >= 2);
			return coord[1];
		}
		constexpr double & Y () noexcept
		{
			static_assert(N >= 2);
			return coord[1];
		}
		
		constexpr double Z () const noexcept
		{
			static_assert (N >= 3);
			return coord[2];
		}
		constexpr double & Z () noexcept
		{
			static_assert(N >= 3);
			return coord[2];
		}

	};

	template<typename Derived, std::size_t N>
	constexpr Derived operator* (double c, const VectorBase<Derived, N> &v) noexcept
	{
		Derived res(v.derived());
		for (std::size_t i{}; i < N; ++i) {
			res[i] *= c;
		}
		return res;
	}

	template<std::size_t N>
	class Vector;
	template<std::size_t N>
	class Point;

	template<std::size_t N>
	class Vector: public VectorBase<Vector<N>, N>
	{
		friend Point<N>;
		public:
		using VectorBase<Vector<N>, N>::VectorBase;
		
		[[nodiscard]] constexpr Vector(const Point<N> &p) noexcept:
			VectorBase<Vector<N>, N>(p.coord)
		{}

		[[nodiscard]] inline constexpr Vector vect (const Vector &v) const 
		{
			static_assert(N == 3);
			return Vector(this->Y()*v.Z()-this->Z()*v.Y(), this->Z()*v.X() - this->X()*v.Z(), this->X()*v.Y() - this->Y()*v.X());
		}
	
		[[nodiscard]] constexpr double operator* (const Vector &v) const noexcept
		{
			double res{};
			for (std::size_t i{}; i < N; ++i) {
				res += this->coord[i]*v[i];
			}
			return res;
		}
	};

	template<std::size_t N>
	constexpr Vector<N> operator* (double c, const Vector<N> &v) noexcept
	{
		Vector<N> res(v);
		for (std::size_t i{}; i < N; ++i) {
			res[i] *= c;
		}
		return res;
	}

	template<std::size_t N>
	class Point: public VectorBase<Point<N>, N>
	{
		friend Vector<N>;
		public:
		using VectorBase<Point<N>, N>::VectorBase;
		[[nodiscard]] constexpr Point(const Vector<N> &v) noexcept:
			VectorBase<Point<N>, N>(v.coord)
		{}
	
		[[nodiscard]] constexpr Vector<N> operator- (const Point &p) const noexcept
		{	
			Vector<N> res(this->coord);
			for (std::size_t i{}; i < N; ++i) {
				res[i] -= p[i];
			}
			return res;
		}
	};

	template<std::size_t N>
	constexpr Point<N> operator* (double c, const Point<N> &p) noexcept
	{
		Point<N> res(p);
		for (std::size_t i{}; i < N; ++i) {
			res[i] *= c;
		}
		return res;
	}

}

class Quaternion {
	
	private:
	using Mat = struct Matrix3
	{
		double m[3][3];
		Geometry::Vector<3> operator* (const Geometry::Vector<3> &v) const noexcept
		{
			return {
				m[0][0]*v.X() + m[0][1]*v.Y() + m[0][2]*v.Z(),
				m[1][0]*v.X() + m[1][1]*v.Y() + m[1][2]*v.Z(),
				m[2][0]*v.X() + m[2][1]*v.Y() + m[2][2]*v.Z()
			};
		}
	};

	Mat m;
	public:
	double w, x, y, z;

	Quaternion (double w, double x, double y, double z);
	Quaternion (const Geometry::Vector<3> &axis, double angle);

	void normalize ();

	Quaternion conjugate () const;
	Quaternion inverse () const;

	Quaternion operator* (const Quaternion &rhs) const;

	Geometry::Vector<3> rotate (const Geometry::Vector<3> &v) const;
};

#endif
