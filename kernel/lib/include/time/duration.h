#pragma once
#include<stdint.h>
#include<math/gcd.h>
namespace util
{
    template<uintmax_t N, uintmax_t D>
	struct ratio
	{
		static_assert(D != 0, "zero denominator");

		static constexpr uintmax_t num = N / math::gcd(N, D);
		static constexpr uintmax_t den = D / math::gcd(N, D);

		typedef ratio<num, den> type;
	};

	typedef ratio<1, 1000000000> nano;
	typedef ratio<1, 1000000> micro;
	typedef ratio<1, 1000> milli;
	typedef ratio<1, 100> centi;
	typedef ratio<1, 10> deci;
	typedef ratio<10, 1> deca;
	typedef ratio<100, 1> hecto;
	typedef ratio<1000, 1> kilo;
	typedef ratio<1000000, 1> mega;
	typedef ratio<1000000000, 1> giga;


	template<typename T>
	struct is_ratio
	{
		constexpr static bool value = false;
	};
	template<uintmax_t n, uintmax_t d>
	struct is_ratio< ratio<n, d> >
	{
		constexpr static bool value = true;
	};

	template<typename Rep, typename Period>
    class duration
    {
	public:
	static_assert(is_ratio<Period>::value == true, "period must be ratio.");
	typedef duration & reference;

	constexpr duration() = default;
	constexpr duration(const duration & t) :rep(t.rep) {}
	reference operator=(const reference t)
	{
		rep = t.rep;
		return *this;
	}
	reference operator--()
	{
		rep--;
		return *this;
	}
	reference operator--(int)
	{
		return operator--();
	}
	reference operator++()
	{
		rep++;
		return *this;
	}
	reference operator++(int)
	{
		return operator++();
	}
	reference operator+=(const reference rhs)
	{
		rep += rhs.rep;
		return *this;
	}
	reference operator-=(const reference rhs)
	{
		rep -= rhs.rep;
		return *this;
	}
	constexpr Rep count() const
	{
		return rep;
	}
	/*template<typename frep, typename fperiod>
	static constexpr duration<Rep, Period> operator frep(duration<frep, fperiod> t)
	{
		return duration<Rep, Period>(
			(t.count()*fperiod::num*Period::den)
			/ (fperiod::den*Period::num));
	}*/
	template<typename Rep2>
	explicit constexpr duration(Rep2 t)
		:rep(static_cast<Rep>(t))
	{}
	explicit constexpr duration(Rep t)
		:rep(t){}
	
	static constexpr Rep zero()
	{
		return Rep{ 0 };
	}
	private:
		Rep rep;
    };
	typedef duration<uint64_t, milli> millisecond_t;
} // namespace util

inline namespace literal
{
	inline constexpr util::duration<uint64_t, util::milli>
		operator""_ms(unsigned long long int t)
	{
		return util::duration<uint64_t, util::milli>(t);
	}
}