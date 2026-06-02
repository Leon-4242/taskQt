#ifndef GEOMETRY
#define GEOMETRY

#include <array>
#include <type_traits>
#include <cmath>
#include <cassert>
#include <string>

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
		
		constexpr bool operator==(const Derived &v) const noexcept
		{
			return coord == v.coord;
		}

		constexpr bool operator!=(const Derived &v) const noexcept
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
	
		[[nodiscard]] constexpr double X () const noexcept
		{
			static_assert (1 <= N && N <= 3);
			return coord[0];
		}
	
		[[nodiscard]] constexpr double & X () noexcept
		{
			static_assert (1 <= N && N <= 3);
			return coord[0];
		}
	
		[[nodiscard]] constexpr double Y () const noexcept
		{
			static_assert (2 <= N && N <= 3);
			return coord[1];
		}

		[[nodiscard]] constexpr double & Y () noexcept
		{
			static_assert(2 <= N && N <= 3);
			return coord[1];
		}
		
		[[nodiscard]] constexpr double Z () const noexcept
		{
			static_assert (N == 3);
			return coord[2];
		}

		[[nodiscard]] constexpr double & Z () noexcept
		{
			static_assert(N == 3);
			return coord[2];
		}

		[[nodiscard]] constexpr std::string view () const noexcept
		{
			std::string res{"("};
			for (std::size_t  i{}; i < N; ++i) {
				res += std::to_string(coord[i]) + ", ";
			}
			res += ")";
			return res;
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

	template<std::size_t N, std::size_t M>
	class Matrix : public VectorBase<Matrix<N, M>, N*M>
	{
		public:
		using VectorBase<Matrix<N, M>, N*M>::VectorBase;
		
		[[nodiscard]] constexpr double operator() (std::size_t i, std::size_t j) const noexcept
		{
			return this->coord[i*M+j];
		}

		[[nodiscard]] constexpr double & operator() (std::size_t i, std::size_t j) noexcept
		{
			return this->coord[i*M+j];
		}

		constexpr Vector<N> operator* (const Vector<M> &v) const noexcept
		{
			Vector<N> res;
			for (std::size_t i{}; i < N; ++i) {
					res[i] = 0;
					for (std::size_t k{}; k < M; ++k) {
						res[i] += this->coord[i*M+k]*v[k];
					}
			}
			return res;
		}

//		constexpr double det () const noexcept
//		{
//			static_assert(N == M);
//			return detPrimitive();
//		}

	};
		
	template<std::size_t N, std::size_t M>
	constexpr Matrix<N, M> operator* (double c, const Matrix<N, M> &m) noexcept
	{
		Matrix<N, M> res(m);
		for (std::size_t i{}; i < N*M; ++i) {
			res[i] *= c;
		}
		return res;
	}
	
	template<std::size_t N, std::size_t M, std::size_t K>
	constexpr Matrix<N, K> operator* (const Matrix<N, M> &m1, const Matrix<M, K> &m2) noexcept
	{
		Matrix<N, K> res;
		for (std::size_t i{}; i < N; ++i) {
			for (std::size_t j{}; j < K; ++j) {
				res(i, j) = 0;
				for (std::size_t k{}; k < M; ++k) {
					res(i, j) += m1(i, k)*m2(k, j);
				}
			}
		}
		return res;
	}
	
	template<std::size_t N, std::size_t M>
	constexpr Vector<N> operator* (const Matrix<N, M> &m, const Vector<M> &v) noexcept
	{
		Vector<N> res;
		for (std::size_t i{}; i < N; ++i) {
				res[i] = 0;
				for (std::size_t k{}; k < M; ++k) {
					res[i] += m(i, k)*v[k];
				}
		}
		return res;
	}
	
	template<std::size_t N, std::size_t M>
	constexpr Vector<M> operator* (const Vector<N> &v, const Matrix<N, M> &m) noexcept
	{
		Vector<M> res;
		for (std::size_t j{}; j < M; ++j) {
				res[j] = 0;
				for (std::size_t k{}; k < N; ++k) {
					res[j] += v[k]*m(k, j);
				}
		}
		return res;
	}

	class Quaternion : public Vector<4>
	{
		Matrix<3, 3> m;
	
		public:

		Quaternion (double, double, double, double);
		Quaternion (const Vector<3> &, double);

		void normalize ();

		Quaternion inverse () const;

		Quaternion operator* (const Quaternion &) const;

		Vector<3> rotate (const Vector<3> &) const;
	};
}

#endif
