#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <array>
#include <gmpxx.h>
#include <tuple>
#include <type_traits>
#include <array>
#include <iostream>
#include <iomanip>
#ifndef CINT
#define CINT
typedef mpz_class cint;
#endif //CINT
typedef std::array<uint8_t,64> ar512;
typedef std::array<uint8_t,32> ar256;

/*
template <class ar>
ar int_to_ar(const cint& n);

template <class ar>
cint ar_to_int(const ar& a);
*/

template<class ar>
ar int_to_ar(const cint &n)
{
    ar result;
    size_t sz;
    mpz_export(result.data(), &sz, -1, result.max_size(), -1, 0, n.get_mpz_t());
    return result;
}

template<class ar>
cint ar_to_int(const ar &a)
{
    cint result;
    mpz_import(result.get_mpz_t(), a.size(), -1, sizeof (uint8_t)/* sizeof(std::tuple_element<0, decltype(a)>::type)*/, -1, 0, a.data());
    std::cout<<std::dec<<a.size()<<"\n";
    return result;
}



template ar512 int_to_ar<ar512>(const cint &n);
template ar256 int_to_ar<ar256>(const cint &n);
template cint ar_to_int<ar512>(const ar512 &a);



#endif // CONVERSIONS_H
