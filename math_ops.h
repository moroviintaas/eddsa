#ifndef MATH_OPS_H
#define MATH_OPS_H
#include <gmpxx.h>
#ifndef CINT
#define CINT
typedef mpz_class cint;
#endif //CINT
cint mod(const cint &a, const cint &n);
cint inv(const cint &a, const cint &n);
cint powm(const cint &base, const cint& exp, const cint &n);

#endif // MATH_OPS_H
