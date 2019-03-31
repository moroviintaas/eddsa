#ifndef ED_POINT_H
#define ED_POINT_H
#include <gmpxx.h>
#include <stdint.h>
#include "ed25519_params.h"
#include <iostream>
#ifndef CINT
#define CINT
typedef mpz_class cint;
#endif //CINT

struct Ec_params{
    cint modulus;
    cint d;
};
const Ec_params ed25519_params ={p, d};

class Ed_point
{
protected:
    cint X;
    cint Y;
    cint Z;
    cint T;

    Ec_params const * params;
public:
    Ed_point(const Ec_params &params = ed25519_params);
    Ed_point(const cint &x, const cint &y, const Ec_params &curve_params = ed25519_params);
    Ed_point(const Ed_point &pnt);
    bool operator==(const Ed_point &pnt) const;
    bool equal_params(const Ed_point &pnt) const;
    Ed_point &operator=(const Ed_point &pnt);


    cint get_x() const;
    cint get_y() const;


    Ed_point &operator+=(const Ed_point &pnt);
    Ed_point operator+(const Ed_point &pnt) const;
    Ed_point operator*(const cint &m) const;

    static cint mod(const cint &a, const cint &n);

};

#endif // ED_POINT_H
