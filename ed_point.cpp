#include "ed_point.h"


Ed_point::Ed_point(const Ec_params &params)
{
    this->params = &params;
    X = 0;
    Y = 1;
    Z = 1;
    T = 0;
}

Ed_point::Ed_point(const cint &x, const cint &y, const Ec_params &curve_params)
{
    this->X = x;
    this->Y = y;
    this->Z = 1;
    this->T = x*y*Z;
    this->params = &curve_params;
}

Ed_point::Ed_point(const Ed_point &pnt)
{
    this->X = pnt.X;
    this->Y = pnt.Y;
    this->Z = pnt.Z;
    this->T = pnt.T;
    this->params = pnt.params;
}
bool Ed_point::operator==(const Ed_point &pnt) const
{

    if(this->params->d == pnt.params->d && this->params->modulus == pnt.params->modulus && this->get_x() == pnt.get_x() && this->get_y() == pnt.get_y())
        return true;
    else return false;
}
bool Ed_point::equal_params(const Ed_point &pnt) const
{
    if (params->d == pnt.params->d && params->modulus == pnt.params->modulus)
        return true;
    else
        return false;
}
Ed_point & Ed_point::operator=(const Ed_point &pnt)
{
    this->X = pnt.X;
    this->Y = pnt.Y;
    this->Z = pnt.Z;
    this->T = pnt.T;
    this->params = pnt.params;
    return *this;
}

cint Ed_point::get_x() const
{
    cint iZ;
    mpz_invert(iZ.get_mpz_t(),Z.get_mpz_t(),params->modulus.get_mpz_t());
    return mod((X*iZ),params->modulus);
}
cint Ed_point::get_y() const
{
    cint iZ;
    mpz_invert(iZ.get_mpz_t(),Z.get_mpz_t(),params->modulus.get_mpz_t());
    return mod((Y*iZ),params->modulus);
}

Ed_point &Ed_point::operator+=(const Ed_point &pnt)
{
    if (equal_params(pnt) == false)
    {
        std::cerr<<"Error: Trying to add 2 points over different curves.\n";
        std::cerr<<"Curve 1:\n\tp = "<<std::dec<<params->modulus<<"\n\td = "<<params->d<<"\n";
        std::cerr<<"Curve 2:\n\tp = "<<std::dec<<pnt.params->modulus<<"\n\td = "<<pnt.params->d<<"\n";
        exit(-1);
    }
    cint A,B,C,D,E,F,G,H;
    const cint &X2 = pnt.X;
    const cint &Y2 = pnt.Y;
    const cint &Z2 = pnt.Z;
    const cint &T2 = pnt.T;
    const cint &p = params->modulus;

    A = mod((Y-X) * (Y2 - X2),p);
    B = mod((Y+X) * (Y2 + X2),p);
    C = mod((T*2*d*T2) , p);
    D = mod((Z * 2 * Z2) ,p);
    E = mod((B - A) , p);
    F = mod((D - C) , p);
    G = mod((D + C) , p);
    H = mod((B + A) , p);
    X = mod((E*F),p);
    Y = mod((G*H),p);
    T = mod((E*H),p);
    Z = mod((F*G),p);
    return *this;
}

Ed_point Ed_point::operator+(const Ed_point &pnt) const
{
    if (equal_params(pnt) == false)
    {
        std::cerr<<"Error: Trying to add 2 points over different curves.\n";
        std::cerr<<"Curve 1:\n\tp = "<<std::dec<<params->modulus<<"\n\td = "<<params->d<<"\n";
        std::cerr<<"Curve 2:\n\tp = "<<std::dec<<pnt.params->modulus<<"\n\td = "<<pnt.params->d<<"\n";
        exit(-1);
    }
    cint A,B,C,D,E,F,G,H;
    Ed_point result(*params);
    const cint &X2 = pnt.X;
    const cint &Y2 = pnt.Y;
    const cint &Z2 = pnt.Z;
    const cint &T2 = pnt.T;
    const cint &p = params->modulus;

    A = mod((Y-X) * (Y2 - X2),p);
    B = mod((Y+X) * (Y2 + X2),p);
    C = mod((T*2*d*T2) , p);
    D = mod((Z * 2 * Z2) ,p);
    E = mod((B - A) , p);
    F = mod((D - C) , p);
    G = mod((D + C) , p);
    H = mod((B + A) , p);
    result.X = mod((E*F),p);
    result.Y = mod((G*H),p);
    result.T = mod((E*H),p);
    result.Z = mod((F*G),p);

    return result;


}

Ed_point Ed_point::operator*(const cint &m) const
{
    cint pr = m;
    Ed_point result = Ed_point(*params);
    size_t bit_num = mpz_sizeinbase(m.get_mpz_t(),2);

    for (size_t i=bit_num-1; i>0; --i)
    {
        if(mpz_tstbit(pr.get_mpz_t(),i) ==1)
        {
            result = result + *this;
        }
        result+=result;
    }
    if(mpz_tstbit(pr.get_mpz_t(),0) ==1)
    {
        result = result + *this;
    }
    return result;
}







































//Ed_point &operator+=(const Ed_point &pnt)

