#pragma once
#include <bitset>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#if UINT_MAX == 0xFFFF
#define X_16
#define WIDTH_INT 4
#define TMP_INT_SIZE 2
#define ax_r ax
#define bx_r bx
#define cx_r cx
#define dx_r dx
#define si_r si
#define di_r di
#elif UINT_MAX == 0xFFFFFFFF
#define X_32
#define WIDTH_INT 8
#define TMP_INT_SIZE 4
#define ax_r eax
#define bx_r ebx
#define cx_r ecx
#define dx_r edx
#define si_r esi
#define di_r edi
#elif UINT_MAX == 0xFFFFFFFFFFFFFFFF
#define X_64
#define WIDTH_INT 16
#define TMP_INT_SIZE 8
#define ax_r rax
#define bx_r rbx
#define cx_r rcx
#define dx_r rdx
#define si_r esi
#define di_r edi
#else
#error "Unknown OS bit depth"
#endif
#define S_LONG_NUMBER
#ifndef _MSC_VER
#error "Only for Visual C++"
#endif
#ifdef S_LONG_NUMBER
#ifdef P_LONG_NUMBER
#ifdef V_LONG_NUMBER
#error "DEFINED: S_LONG_NUMBER, P_LONG_NUMBER, V_LONG_NUMBER"
#else
#error "DEFINED: S_LONG_NUMBER, P_LONG_NUMBER"
#endif
#elif V_LONG_NUMBER
#error "DEFINED: S_LONG_NUMBER, V_LONG_NUMBER"
#endif
#elif P_LONG_NUMBER
#ifdef V_LONG_NUMBER
#error "DEFINED: P_LONG_NUMBER, V_LONG_NUMBER"
#endif
#endif
namespace lnum {
#ifndef P_LONG_NUMBER
#ifndef V_LONG_NUMBER

	struct number_s final {
		size_t size() const;
		number_s() {}
		number_s(const number_s& num);
		number_s(const unsigned long long& num);

		const unsigned int& operator[](size_t n) const;


		number_s& operator+=(const number_s&);
		number_s& operator-=(const number_s&);
		number_s& operator*=(const number_s&);
		number_s& operator/=(const number_s&);
		number_s& operator%=(const number_s&);
		number_s& operator&=(const number_s&);
		number_s& operator|=(const number_s&);
		number_s& operator>>=(const number_s&);
		number_s& operator<<=(const number_s&);
		number_s& operator=(const number_s& num);


		const bool operator==(const number_s&) const;
		const bool operator!=(const number_s&) const;
		const bool operator>(const number_s&) const;
		const bool operator<(const number_s&) const;
		const bool operator<=(const number_s&) const;
		const bool operator>=(const number_s&) const;

		const number_s& operator+() const;
		const bool operator!() const;
		number_s& operator--();
		const number_s operator--(int);
		number_s& operator++();
		const number_s operator++(int);

		explicit operator bool() const;
	protected:
		struct number_vector {
			mutable size_t size;
			mutable unsigned int * number;
			inline unsigned int& operator[](size_t n) {
				if (n >= this->size)
					throw std::invalid_argument("Going beyond the bounds of a number."); return number[n]; }
			inline unsigned int operator[](size_t n) const {
				if (n >= this->size)
					throw std::invalid_argument("Going beyond the bounds of a number."); return number[n]; }
			void resize(size_t new_size);
			number_vector();
			number_vector(unsigned int n);
			~number_vector();
		} number_num;
		inline bool is_zero() const;
		friend inline number_s& __additing_operator_s(number_s&, const number_s&);
		friend inline number_s& __substruct_operator_s(number_s&, const number_s&);
		friend inline number_s& __multiple_operator_s(number_s&, const number_s&);
		friend inline number_s& __division_operator_s(number_s&, const number_s&);
		friend inline const bool __equal_operator_s(const number_s& ths, const number_s& num);
		friend inline const bool __less_equal_operator_s(const number_s& ths, const number_s& num);
		friend inline const bool __greater_equal_operator_s(const number_s& ths, const number_s& num);
		friend inline number_s& __bit_and_operator_s(number_s& ths, const number_s& num);
		friend inline number_s& __bit_or_operator_s(number_s& ths, const number_s& num);
		friend inline number_s& __mod_operator_s(number_s& ths, const number_s& num);
		friend inline number_s& __shift_right_operator_s(number_s& ths, const number_s& num);
		friend inline number_s& __shift_left_operator_s(number_s& ths, const number_s& num);
		friend std::ostream& operator<<(std::ostream&, const number_s& num);
	};
#ifdef S_LONG_NUMBER
	typedef number_s number;
#endif
}
const lnum::number_s operator+(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator-(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator*(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator/(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator%(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator&(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator|(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator>>(const lnum::number_s& ths, const lnum::number_s& num);
const lnum::number_s operator<<(const lnum::number_s& ths, const lnum::number_s& num);
#endif
#endif
