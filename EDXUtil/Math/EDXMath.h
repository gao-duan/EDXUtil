#pragma once

#include "../EDXPrerequisites.h"
#include "Constants.h"

#include <utility>

namespace EDX
{
	namespace Math
	{
		template <typename T>
		inline bool NumericValid(const T& num) { return !_isnan(num) && _finite(num); }

		template <class T1, class T2, class T3>
		inline T1 Clamp(const T1& tVal, const T2& tMin, const T3& max)
		{
			if (tVal < tMin) return tMin;
			if (tVal > max) return max;
			return tVal;
		}
		inline float Saturate(float val) { return Clamp(val, 0.0f, 1.0f); }
		inline float Pow(float val, float fPow) { return powf(val, fPow); }
		template <class T> inline T Abs(T tVal) { return tVal >= 0 ? tVal : -tVal; }
		inline float Sqrt(float val) { return sqrtf(val); }
		inline float ToRadians(float fDeg) { return (fDeg / 180.0f) * float(Math::EDX_PI); }
		inline float ToDegrees(float fRad) { return (fRad / float(Math::EDX_PI)) * 180.0f; }
		inline float Sin(float val) { return sinf(val); }
		inline float Cos(float val) { return cosf(val); }
		inline void SinCos(float val, float& fSin, float& fCos) { fSin = sinf(val); fCos = cosf(val); }
		inline float Tan(float val) { return tanf(val); }
		inline float Exp(float val) { return expf(val); }
		inline float Atan2(float val1, float val2) { return atan2f(val1, val2); }

		inline int FloorToInt(float val) { return (int)val; }
		inline int CeilToInt(float val)
		{
			int ceil = (int)val;
			if (val - ceil > 1e-5f)
			{
				return ceil + 1;
			}
			else
			{
				return ceil;
			}
		}
		inline int RoundToInt(float val) { return FloorToInt(val + 0.5f); }

		template <class T1, class T2>
		inline float LinStep(const T1& tVal, const T2& min, const T2& max)
		{
			if (min == max)
			{
				return 0.0f;
			}
			return Saturate((tVal - min) / (max - min));
		}
		template<class T1, class T2> inline auto Lerp(const T1& min, const T2& max, const float val) -> decltype(min + max) { return min * (1 - val) + max * val; }
		template<class T1, class T2> inline auto Max(const T1& e1, const T2& e2) -> decltype(e1 + e2)  { return e1 > e2 ? e1 : e2; }
		template<class T1, class T2> inline auto AbsMax(const T1& e1, const T2& e2) -> decltype(e1 + e2)  { return Abs(e1) > Abs(e2) ? e1 : e2; }
		template<class T1, class T2> inline auto Min(const T1& e1, const T2& e2) -> decltype(e1 + e2)  { return e1 < e2 ? e1 : e2; }
		template<class T1, class T2> inline auto AbsMin(const T1& e1, const T2& e2) -> decltype(e1 + e2)  { return Abs(e1) < Abs(e2) ? e1 : e2; }

		template<class T1, class T2, class T3, class T4>
		inline auto BiLerp(const T1& e00, const T2& e01, const T3& e10, const T4& e11, const float val1, const float val2) -> decltype(e00 + e01 + e10 + e11)
		{
			return Lerp(Lerp(e00, e01, val1), Lerp(e10, e11, val1), val2);
		}
		template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		inline auto TriLerp(const T1& e000, const T2& e001, const T3& e010, const T4& e011, const T5& e100, const T6& e101, const T7& e110, const T8& e111, const float val1, const float val2, const float val3) -> decltype(e000 + e001 + e010 + e011 + e100 + e101 + e110 + e111)
		{
			return Lerp(BiLerp(e000, e001, e010, e011, val1, val2), BiLerp(e100, e101, e110, e111, val1, val2), val3);
		}

		inline bool IsPowOfTwo(const int val)
		{
			return (val & (val - 1)) == 0;
		}
		inline int RoundUpPowTwo(uint val)
		{
			val--;
			val |= val >> 1;
			val |= val >> 2;
			val |= val >> 4;
			val |= val >> 8;
			val |= val >> 16;
			return val + 1;
		}
		inline int RoundUpTo(const uint val, const uint round)
		{
			uint iRet = val + round - (val % round);
			return iRet;
		}
		inline int CeilLog2(int val)
		{
			int ret = 0;
			while (val)
			{
				val >>= 1;
				ret++;
			}
			return ret;
		}

		template<uint Dim> class Pow2
		{
		public:
			static const int Value = 1 << Dim;
		};
		template<uint Dim> class Pow4
		{
		public:
			static const int Value = 1 << 2 * Dim;
		};

		inline float Xor(const float f1, const float f2)
		{
			return _mm_cvtss_f32(_mm_xor_ps(_mm_set_ss(f1), _mm_set_ss(f2)));
		}

		inline float SignMask(const float f)
		{
			return _mm_cvtss_f32(_mm_and_ps(_mm_set_ss(f), _mm_castsi128_ps(_mm_set1_epi32(0x80000000))));
		}

		template<class T>
		inline T MonoCubicLerp(const T& e0, const T& e1, const T& e2, const T& e3, const float fLerp)
		{
			assert(abs(e2) < 1e8f);
			T eVal = Math::Lerp(e1, e2, fLerp);

			T emnusCur = fLerp;
			T emnusCurSqr = emnusCur * emnusCur;
			T emnusCurCub = emnusCurSqr * emnusCur;

			T eK = e2 - e1;
			T eD1 = (e2 - e0) * 0.5f;
			T eD2 = (e3 - e1) * 0.5f;
			if (eK * eD1 < 0)
			{
				eD1 = 0;
			}
			if (eK * eD2 < 0)
			{
				eD2 = 0;
			}
			if (eK == 0)
			{
				eD1 = eD2 = 0;
			}

			T eA0 = e1;
			T eA1 = eD1;
			T eA2 = 3 * eK - 2 * eD1 - eD2;
			T eA3 = eD1 + eD2 - 2 * eK;
			T eRet = eA3 * emnusCurCub + eA2 * emnusCurSqr + eA1 * emnusCur + eA0;
			//eRet = eA0 * emnusCurCub + eA1 * emnusCurSqr + eA2 * emnusCur + eA3;
			assert(NumericValid(eRet));

			return eRet;
		}
	}
}