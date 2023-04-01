#include <stdlib.h>
#include <errno.h>

typedef int64_t integer;

/* Errnos */
extern int errno;
#define RATIONAL_DENOMINATOR_IS_ZERO 100000

typedef struct {
    integer num;
    integer den;
} Rational;

#define NIL(rat) rat.num = rat.den = 0;

Rational Rational_New(integer num, integer den) {
    Rational rat;
    if (den == 0) {
        errno = RATIONAL_DENOMINATOR_IS_ZERO;
        NIL(rat); return rat;
    }
    rat.num = num; rat.den = den;
    return rat;
}

integer gcd(integer x, integer y) {
    while (y != 0) {
        int tmp = x;
        x = y;
        y = tmp % y;
    }
    return x;
}

/* NOTE: all functions with `This' in name requires 
    the operand to be passed as a pointer!
*/
void Rational_SimplifyThis(Rational *this) {
    integer g = gcd(this->num, this->den);
    if (g == 1) return;
    this->num /= g;
    this->den /= g;
}

#define RATIONAL_ISINT(rat)  ((rat).den == 1)

Rational Rational_Add(Rational this, Rational that) {
    Rational rat;
	if (RATIONAL_ISINT(this) && RATIONAL_ISINT(that)) {
	    rat.num = this.num + that.num;
        rat.den = 1; return rat;
	}
    rat.den = this.den * that.den;
    rat.num = this.den * that.num + this.num * that.den;
    Rational_SimplifyThis(&rat);
    return rat;
}

void Rational_NegateThis(Rational* this) {
    this->num *= -1;
}

#define RATIONAL_ISZERO(this) ((this).num == 0)

Rational Rational_Subtract(Rational this, Rational that) {
    Rational rat;
    Rational_NegateThis(&that);
    return Rational_Add(this, that);
}

void Rational_InvertThis(Rational* this) {
    if (RATIONAL_ISZERO(*this)) {
        errno = RATIONAL_DENOMINATOR_IS_ZERO;
        return;
    }
    integer tmp = this->num;
    this->num = this->den;
    this->den = tmp;
}

Rational Rational_Multiply(Rational this, Rational that) {
    Rational rat;
    if (RATIONAL_ISINT(this) && RATIONAL_ISINT(that)) {
	    rat.num = this.num * that.num;
        rat.den = 1; return rat;
    }
    rat.num = this.num * that.num;
    rat.den = this.den * that.den;
    Rational_SimplifyThis(&rat);
    return rat;
}

Rational Rational_Divide(Rational this, Rational that) {
    Rational rat;
    Rational_InvertThis(&that);
    if (errno == RATIONAL_DENOMINATOR_IS_ZERO) {
        NIL(rat); return rat;
    }
    return Rational_Multiply(this, that);
}

int Rational_Equal(Rational this, Rational that) {
    Rational_SimplifyThis(&this);
    Rational_SimplifyThis(&that);
    return (this.num == that.num && this.den == that.den);
}

int Rational_EqualInteger(Rational this, integer that) {
    Rational_SimplifyThis(&this);
    if (this.den != 1) return 0;
    return this.num == that;
}