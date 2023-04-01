#include "rational.h"
#include "tap.h"

int main() {
    plan(2);
    {
        Rational rat = Rational_New(1, 0); 
        ok(errno);
    }
    {
        Rational rat = Rational_New(1, 1);
        Rational rat2 = Rational_New(2, 2);
        ok(Rational_EqualInteger(Rational_Add(rat, rat2), 2));
    }
    {
        Rational rat = Rational_New(1, 2);
        Rational rat2 = Rational_New(1, 3);
        ok(Rational_Equal(Rational_Add(rat, rat2), Rational_New(5, 6)));
    }
}