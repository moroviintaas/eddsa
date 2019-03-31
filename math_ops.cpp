#include "math_ops.h"
cint mod(const cint &a, const cint &n)
{
    cint result;
    mpz_mod(result.get_mpz_t(), a.get_mpz_t(), n.get_mpz_t() );
    //std::cout<<a<<"\n"<<n<<"\n"<<result<<"\n\n";
    return result;
}

cint inv(const cint &a, const cint &n)
{
    cint result;
    mpz_invert(result.get_mpz_t(), a.get_mpz_t(), n.get_mpz_t());
    return result;
}
cint powm(const cint &base, const cint& exp, const cint &n){
    cint result;
    mpz_powm_sec(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), n.get_mpz_t());
    return result;

}
