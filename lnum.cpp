#pragma once
#include "lnum.h"
#ifndef P_LONG_NUMBER 
#ifndef V_LONG_NUMBER
const lnum::number_s operator+(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n += num; }
const lnum::number_s operator-(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n -= num; }
const lnum::number_s operator*(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n *= num; }
const lnum::number_s operator/(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n /= num; }
const lnum::number_s operator%(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n %= num; }
const lnum::number_s operator&(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n &= num; }
const lnum::number_s operator|(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n |= num; }
const lnum::number_s operator>>(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n >>= num; }
const lnum::number_s operator<<(const lnum::number_s& ths, const lnum::number_s& num) { lnum::number_s n = ths; return n <<= num; }
namespace lnum {
	number_s::number_s(const number_s& num) : number_num(num.number_num.size) {
		unsigned int *ths = this->number_num.number, *nm = num.number_num.number,
			*nmend = num.number_num.number + num.number_num.size;
		__asm {
			mov di_r, nm
			mov si_r, ths
			mov cx_r, nmend
			copyloop :
			mov ax_r, [di_r]
				mov[si_r], ax_r
				add si_r, TMP_INT_SIZE
				add di_r, TMP_INT_SIZE
				cmp di_r, cx_r
				jl copyloop
		}
	}
	number_s::number_s(const unsigned long long& num) : number_num(sizeof(num) / sizeof(unsigned int)) {
		unsigned int i = 0;
		if (!num) this->number_num.number[0] = 0;
		for (; num >> sizeof(unsigned int) * 8 * i; ++i) {
			unsigned long long t1 = num >> sizeof(unsigned int) * 8 * i,
				t2 = (num >> sizeof(unsigned int) * 8 * (i + 1)) << sizeof(unsigned int) * 8;
			this->number_num[i] = t1 - t2;
		}
		if (!i) this->number_num.resize(1);
		else if (i < this->number_num.size) this->number_num.resize(i);
	}
	size_t number_s::size() const { return this->number_num.size; }
	const unsigned int& number_s::operator[](size_t n) const {
		if (n >= this->number_num.size)
			throw std::invalid_argument("Going beyond the bounds of a number."); return this->number_num[n]; }
	number_s::operator bool() const { return !this->is_zero(); }
	inline bool number_s::is_zero() const { return this->number_num.size == 1 && this->number_num[0] == 0; }



	void number_s::number_vector::resize(size_t new_size) {
		number = (unsigned int *)realloc(number, new_size * sizeof(unsigned int));
		if (number == nullptr) throw std::bad_alloc();
		if (this->size < new_size)
			for (size_t i = this->size; i < new_size; ++i)
				this->number[i] = 0;
		this->size = new_size;
	}
	number_s::number_vector::number_vector() : size(1) {
		if ((this->number = (unsigned int*)malloc(sizeof(unsigned int))) == nullptr)
			throw std::bad_alloc();
		this->number[0] = 0;
	}
	number_s::number_vector::number_vector(unsigned int n) : size(n) {
		if ((this->number = (unsigned int*)malloc(n * sizeof(unsigned int))) == nullptr)
			throw std::bad_alloc();
		unsigned int* num_num = this->number, *num_num_end = this->number + n;
		__asm {
			mov si_r, num_num
			mov cx_r, num_num_end
			zeroloop :
			mov[si_r], 0
				add si_r, TMP_INT_SIZE
				cmp si_r, cx_r
				jl zeroloop
		}
	}
	number_s::number_vector::~number_vector() { free(this->number); }






	inline number_s& __additing_operator_s(number_s& ths, const number_s& num) {
		unsigned int t = 0;
		if (ths.number_num.size < num.number_num.size) ths.number_num.resize(num.number_num.size);
		unsigned int *ths_num = ths.number_num.number, *num_num = num.number_num.number,
			*ths_num_end = ths_num + ths.number_num.size, *num_num_end = num_num + num.number_num.size;
		__asm {
			mov di_r, num_num
			mov si_r, ths_num
			mov cx_r, num_num_end
			mov bx_r, ths_num_end
			xor dx_r, dx_r
			loop_add :
			mov ax_r, [di_r]
				add[si_r], ax_r
				jc g1
				add[si_r], dx_r
				jc nextloop_add
				mov dx_r, 0
				jmp nextloop_add
				g1 :
			add[si_r], dx_r
				mov dx_r, 1
				nextloop_add :
				add si_r, TMP_INT_SIZE
				add di_r, TMP_INT_SIZE
				cmp di_r, cx_r
				jl loop_add

				cmp dx_r, 0
				je end_add
				loop_add_1 :
			cmp si_r, bx_r
				je end_add
				add[si_r], dx_r
				jnc end_loop_add_1
				add si_r, TMP_INT_SIZE
				jmp loop_add_1
				end_loop_add_1 :
			mov dx_r, 0


				end_add :
				mov t, dx_r
		}
		if (t) {
			ths.number_num.resize(ths.number_num.size + 1);
			ths.number_num[ths.number_num.size - 1] = 1;
		}
		return ths;
	}
	inline number_s& __substruct_operator_s(number_s& ths, const number_s& num) {
		if (ths < num)
			throw std::invalid_argument("Too big number are subtracted.");
		int tmp = 0, new_size;
		if (ths.number_num.size < num.number_num.size) ths.number_num.resize(num.number_num.size);
		unsigned int *ths_num = ths.number_num.number, *num_num = num.number_num.number;
		unsigned int *ths_num_end = ths_num + ths.number_num.size,
			*num_num_end = num_num + num.number_num.size, sz = ths.number_num.size;
		_asm {
			mov di_r, num_num
			mov si_r, ths_num
			mov cx_r, num_num_end
			xor dx_r, dx_r

			subloop :
			mov ax_r, [si_r]
				mov bx_r, [di_r]
				cmp ax_r, bx_r
				jb cf1
				sub ax_r, bx_r
				cmp ax_r, dx_r
				jb cf2
				sub ax_r, dx_r
				xor dx_r, dx_r
				jmp nextsubloop

				cf1 :
			sub bx_r, ax_r
				mov ax_r, UINT_MAX
				sub ax_r, bx_r
				inc ax_r
				sub ax_r, dx_r
				mov dx_r, 1
				jmp nextsubloop

				cf2 :
			mov ax_r, UINT_MAX

				nextsubloop :
			mov[si_r], ax_r
				add si_r, TMP_INT_SIZE
				add di_r, TMP_INT_SIZE
				cmp di_r, cx_r
				jl subloop

				mov si_r, ths_num_end
				mov bx_r, sz
				mov cx_r, ths_num
				xor dx_r, dx_r
				endloop :
			sub si_r, TMP_INT_SIZE
				cmp si_r, cx_r
				je fromthis
				mov ax_r, [si_r]
				cmp ax_r, 0
				jne fromthis
				dec bx_r
				jmp endloop


				fromthis :
			mov new_size, bx_r
		}
		if (new_size != sz) ths.number_num.resize(new_size);
		return ths;
	}
	inline number_s& __multiple_operator_s(number_s& ths, const number_s& num) {
		if (ths.is_zero() || num.is_zero()) return ths = 0;
		number_s tmp = ths;
		ths = 0;
		ths.number_num.resize(num.number_num.size + tmp.number_num.size);
		unsigned int *ths_num = ths.number_num.number,
			*num_num = num.number_num.number,
			*tmp_num = tmp.number_num.number, tmp_cf = 0;
		for (unsigned int i = 0; i < tmp.number_num.size; ++i) {
			for (unsigned int j = 0; j < num.number_num.size; ++j) {
				__asm {
					mov si_r, tmp_num
					mov di_r, num_num

					mov bx_r, TMP_INT_SIZE
					mov ax_r, j
					mul bx_r
					push ax_r
					add di_r, ax_r
					mov ax_r, i
					mul bx_r
					push ax_r
					add si_r, ax_r

					mov ax_r, [si_r]
					mov dx_r, [di_r]
					mul dx_r
					add ax_r, tmp_cf
					jnc nxt
					inc dx_r
					nxt :

					mov si_r, ths_num
						pop bx_r
						pop cx_r
						add si_r, bx_r
						add si_r, cx_r

						add[si_r], ax_r
						jnc end
						inc dx_r
						end :
					mov tmp_cf, dx_r
				}
			}
			ths.number_num.number[i + num.number_num.size] += tmp_cf;
			tmp_cf = 0;
		}
		if (!ths.number_num.number[ths.number_num.size - 1]) ths.number_num.resize(ths.number_num.size - 1);
		return ths;
	}
	inline number_s& __division_operator_s(number_s& ths, const number_s& num) {
		if (num.is_zero()) throw std::invalid_argument("Division by zero");
		if (num > ths) { ths.number_num.resize(1); ths.number_num[0] = 0; return ths; }
		unsigned int new_size = ths.number_num.size - num.number_num.size + 1;
		number_s n; n.number_num.resize(new_size);
		for (size_t i = new_size - 1; true; --i) {
			for (unsigned int k = sizeof(unsigned int) * 8 - 1, t = 1U << k; true; t = 1U << (--k)) {
				n.number_num.number[i] += t;
				if (ths < (n*num)) n.number_num.number[i] -= t;
				if (!k) break;
			}
			if (!i) break;
			if (i == new_size - 1 && !n.number_num.number[i]) n.number_num.resize(i);
		}
		return ths = n;
	}
	inline number_s& __mod_operator_s(number_s& ths, const number_s& num) { return ths -= (ths / num)*num; }
	inline number_s& __bit_and_operator_s(number_s& ths, const number_s& num) {
		unsigned int* ths_num = ths.number_num.number, *num_num = num.number_num.number,
			sz = ths.number_num.size, *ths_end = ths_num + sz, new_size;
		__asm {
			mov si_r, ths_num
			mov di_r, num_num
			mov cx_r, ths_end
			loopad :
			mov ax_r, [di_r]
				and [si_r], ax_r
				add si_r, TMP_INT_SIZE
				add di_r, TMP_INT_SIZE
				cmp si_r, cx_r
				jb loopad


				mov cx_r, ths_num
				xor dx_r, dx_r
				zeroloop :
			sub si_r, TMP_INT_SIZE
				cmp[si_r], 0
				jne end
				cmp si_r, cx_r
				jna end
				inc dx_r
				jmp zeroloop
				end :
			mov new_size, dx_r
		}
		if (new_size) ths.number_num.resize(num.number_num.size - new_size);
		else if (sz != num.number_num.size) ths.number_num.resize(num.number_num.size);
		return ths;
	}
	inline number_s& __bit_or_operator_s(number_s& ths, const number_s& num) {
		size_t v = ths.number_num.size, vn = num.number_num.size;
		if (v < vn) ths.number_num.resize(v = vn);
		unsigned int* ths_num = ths.number_num.number,
			*num_num = num.number_num.number, *num_end = num_num + vn;
		__asm {
			mov si_r, ths_num
			mov di_r, num_num
			mov cx_r, num_end
			lp :
			mov ax_r, [di_r]
				or [si_r], ax_r
				add si_r, TMP_INT_SIZE
				add di_r, TMP_INT_SIZE
				cmp di_r, cx_r
				jb lp
		}
		return ths;
	}
	inline number_s& __shift_right_operator_s(number_s& ths, const number_s& num) {
		if (num >= ths.number_num.size*TMP_INT_SIZE * 8) {
			ths.number_num.resize(1);
			ths.number_num.number[0] = 0;
			return ths;
		}
		else if (ths.is_zero()) return ths;
		unsigned int shl = (num / (TMP_INT_SIZE * 8)).number_num.number[0];
		if (shl) {
			for (unsigned int* i = ths.number_num.number;
				i + shl < ths.number_num.number + ths.number_num.size; ++i) *i = *(i + shl);
			ths.number_num.resize(ths.number_num.size - shl);
		}
		shl = (num % (TMP_INT_SIZE * 8)).number_num.number[0];
		if (shl) {
			unsigned int prev = 0, next = 0;
			for (unsigned int* i = ths.number_num.number + ths.number_num.size;
				--i >= ths.number_num.number;) {
				next = *i % (1 << shl);
				*i >>= shl;
				*i += (prev << (TMP_INT_SIZE * 8 - shl));
				prev = next;
			}
		}
		if (!ths.number_num.number[ths.number_num.size - 1])
			ths.number_num.resize(ths.number_num.size - 1);
		return ths;
	}
	inline number_s& __shift_left_operator_s(number_s& ths, const number_s& num) {
		if (ths.is_zero()) return ths;
		unsigned int shl = (num / (TMP_INT_SIZE * 8)).number_num.number[0];
		ths.number_num.resize(ths.number_num.size + shl + 1);
		if (shl) {
			for (unsigned int* i = ths.number_num.number + ths.number_num.size - 1;
				--i - shl >= ths.number_num.number;) {
				*i = *(i - shl);
			}
			for (unsigned int* i = ths.number_num.number + shl;
				--i >= ths.number_num.number;) {
				*i = 0;
			}
		}
		unsigned int shlm = (num % (TMP_INT_SIZE * 8)).number_num.number[0];
		if (shlm) {
			unsigned int prev = 0, next = 0;
			for (unsigned int* i = ths.number_num.number + shl;
				i < ths.number_num.number + ths.number_num.size; ++i) {
				next = *i / (1 << (TMP_INT_SIZE * 8 - shlm));
				*i <<= shlm;
				*i += prev;
				prev = next;
			}
		}
		if (!ths.number_num.number[ths.number_num.size - 1])
			ths.number_num.resize(ths.number_num.size - 1);
		return ths;
	}




	number_s& number_s::operator+=(const number_s& num) { return __additing_operator_s(*this, num); }
	number_s& number_s::operator-=(const number_s& num) { return __substruct_operator_s(*this, num); }
	number_s& number_s::operator*=(const number_s& num) { return __multiple_operator_s(*this, num); }
	number_s& number_s::operator/=(const number_s& num) { return __division_operator_s(*this, num); }
	number_s& number_s::operator%=(const number_s& num) { return __mod_operator_s(*this, num); }
	number_s& number_s::operator&=(const number_s& num) { return __bit_and_operator_s(*this, num); }
	number_s& number_s::operator|=(const number_s& num) { return __bit_or_operator_s(*this, num); }
	number_s& number_s::operator>>=(const number_s& num) { return __shift_right_operator_s(*this, num); }
	number_s& number_s::operator<<=(const number_s& num) { return __shift_left_operator_s(*this, num); }



	inline const bool __equal_operator_s(const number_s& ths, const number_s& num) {
		size_t n = ths.number_num.size, nv = num.number_num.size;
		if (n != nv) return false;
		unsigned int *ths_num = ths.number_num.number, *num_num = num.number_num.number;
		unsigned int *ths_num_end = ths_num + n,
			*num_num_end = num_num + nv;
		bool t = 0;
		_asm {
			mov di_r, num_num_end
			mov si_r, ths_num_end
			mov cx_r, ths_num
			equalloop :
			sub di_r, TMP_INT_SIZE
				sub si_r, TMP_INT_SIZE

				mov ax_r, [si_r]
				mov bx_r, [di_r]
				cmp ax_r, bx_r
				jne end
				cmp si_r, cx_r
				ja equalloop
				mov t, 1
				end:
		}
		return t;
	}
	inline const bool __less_equal_operator_s(const number_s& ths, const number_s& num) {
		size_t n = ths.number_num.size, nv = num.number_num.size;
		if (n > nv) return false;
		else if (n < nv) return true;
		unsigned int *ths_num = ths.number_num.number, *num_num = num.number_num.number;
		unsigned int *ths_num_end = ths_num + n,
			*num_num_end = num_num + nv;
		bool t = 0;
		_asm {
			mov di_r, num_num_end
			mov si_r, ths_num_end
			mov cx_r, ths_num
			equalloop :
			sub di_r, TMP_INT_SIZE
				sub si_r, TMP_INT_SIZE

				mov ax_r, [si_r]
				mov bx_r, [di_r]
				cmp ax_r, bx_r
				ja end
				cmp ax_r, bx_r
				jne next
				cmp si_r, cx_r
				jae equalloop
				next :
			mov t, 1
				end :
		}
		return t;
	}
	inline const bool __greater_equal_operator_s(const number_s& ths, const number_s& num) {
		size_t n = ths.number_num.size, nv = num.number_num.size;
		if (n > nv) return true;
		else if (n < nv) return false;
		unsigned int *ths_num = ths.number_num.number, *num_num = num.number_num.number;
		unsigned int *ths_num_end = ths_num + n,
			*num_num_end = num_num + nv;
		bool t = 0;
		_asm {
			mov di_r, num_num_end
			mov si_r, ths_num_end
			mov cx_r, ths_num
			equalloop :
			sub di_r, TMP_INT_SIZE
				sub si_r, TMP_INT_SIZE

				mov ax_r, [si_r]
				mov bx_r, [di_r]
				cmp ax_r, bx_r
				jb end
				cmp ax_r, bx_r
				jne next
				cmp si_r, cx_r
				jae equalloop
				next :
			mov t, 1
				end :
		}
		return t;
	}

	const bool number_s::operator==(const number_s& num) const { return __equal_operator_s(*this, num); }
	const bool number_s::operator!=(const number_s& num) const { return !__equal_operator_s(*this, num); }
	const bool number_s::operator> (const number_s& num) const { return !__less_equal_operator_s(*this, num); }
	const bool number_s::operator< (const number_s& num) const { return !__greater_equal_operator_s(*this, num); }
	const bool number_s::operator<=(const number_s& num) const { return __less_equal_operator_s(*this, num); }
	const bool number_s::operator>=(const number_s& num) const { return __greater_equal_operator_s(*this, num); }







	const number_s& number_s::operator+() const { return *this; }

	inline const bool number_s::operator!() const { return this->is_zero(); }
	const number_s number_s::operator--(int) { number_s num = *this; --*this;  return num; }
	number_s& number_s::operator--() {
		if (this->is_zero())
			throw std::invalid_argument("Too big number are subtracted.");
		unsigned int sz = this->number_num.size, *ths = this->number_num.number,
			*ths_end = this->number_num.number + sz, new_size;
		;
		__asm {
			mov si_r, ths
			pos :
			mov ax_r, [si_r]
				sub ax_r, 1
				jnc endpositive
				mov[si_r], ax_r
				add si_r, TMP_INT_SIZE
				jmp pos


				endpositive :
			mov[si_r], ax_r

				mov si_r, ths_end
				mov bx_r, sz
				mov cx_r, ths
				xor dx_r, dx_r
				endloop :
			sub si_r, TMP_INT_SIZE
				cmp si_r, cx_r
				je fromthis
				mov ax_r, [si_r]
				cmp ax_r, 0
				jne fromthis
				dec bx_r
				jmp endloop

				fromthis :
			mov new_size, bx_r
		}
		if (new_size != sz) this->number_num.resize(new_size);
		return *this;
	}
	const number_s number_s::operator++(int) { number_s num = *this; ++*this; return num; }
	number_s& number_s::operator++() {
		unsigned int t = 0, sz = this->number_num.size, *ths = this->number_num.number,
			*ths_end = this->number_num.number + sz;
		__asm {
			mov si_r, ths
			mov di_r, ths_end
			pos :
			mov ax_r, [si_r]
				add ax_r, 1
				jnc endpos
				mov[si_r], ax_r
				add si_r, TMP_INT_SIZE
				cmp si_r, di_r
				jne pos
				mov t, 1
				jmp end
				endpos :
			mov[si_r], ax_r
				end :
		}
		if (t) { this->number_num.resize(sz + 1); this->number_num[sz] = 1; }
		return *this;
	}

	number_s& number_s::operator=(const number&num) {
		this->number_num.resize(num.number_num.size);
		unsigned int *this_num = this->number_num.number,
			*this_end = this_num + this->number_num.size,
			*num_num = num.number_num.number;
		__asm {
			mov si_r, this_num
			mov di_r, num_num
			mov cx_r, this_end

			g :
			mov ax_r, [di_r]
				mov[si_r], ax_r

				add si_r, TMP_INT_SIZE
				add di_r, TMP_INT_SIZE
				cmp si_r, cx_r
				jb g
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& str, const number_s& num) {
		if (num.number_num.size != 1) {
			unsigned int t = num.number_num[num.number_num.size - 1], c = WIDTH_INT - 1;
			while (!(t >> (c * 4))) --c;
			while (true) {
				str << std::hex << (t >> c * 4) % 0x10;
				if (!c--) break;
			}
			char fl = str.fill();
			str.fill('0');
			for (unsigned int i = num.number_num.size - 2; true; --i) {
				str << std::hex << std::setw(WIDTH_INT) << num.number_num[i];
				if (!i) break;
			}
			str.fill(fl);
			return str;
		}
		else str << std::hex << num.number_num[0];
	}
}
#endif
#endif




