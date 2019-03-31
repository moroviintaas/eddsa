#include <iostream>
#include "ed25519_params.h"
#include <iomanip>
#include "ed_point.h"
#include "conversions.h"
#include "crypto_ops.h"

//ar512 (*int_to_ar512)(const cint& n) = &int_to_ar<ar512>;

void test1()
{
    std::cout<<"===================================\nTest 1:\n";
    cint n = cint("0x607fae1c03ac3b701969327b69c54944c42cec92f44a84ba605afdef9db1619d");
    Ed_point g(PX,PY);
    ar256 sk = int_to_ar<ar256>(n);
    //std::cout<<"a\n";
    ar256 pk = calculate_pk_ed25519(sk,g);

}

void test2()
{
    std::cout<<"===================================\nTest 2:\n";
    cint n = cint("0x607fae1c03ac3b701969327b69c54944c42cec92f44a84ba605afdef9db1619d");
    Ed_point g(PX,PY);
    ar256 csd;

    Ed_point tp, tp2;

    tp = g*n;
    std::cout<<tp.get_x()<<"\n"<<tp.get_y()<<"\n";
    csd = compress_point_ed25519(tp);
    bool success;
    tp2 = decompress_point_ed25519(csd,ed25519_params,success);
    std::cout<<success<<"\t"<<(tp==tp2)<<"\n"<<tp2.get_x()<<"\n"<<tp2.get_y()<<"\n";
}

int main()
{


    Ed_point a(ed25519_params),b(ed25519_params),c;
    Ed_point g(PX,PY);
    cint tmp;
    c= a+b;
    c = a+g;

    a = b = g;
    for (int i=0; i<100010; i++)
    {
        g=g +c;
    }

    cint m = 0xb;

    //a = b = g;
    a = Ed_point();


    for(int i =0; i<m; ++i)
    {
        a = a +g;
        //std::cout<<i+1""
    }
    b = g*m;

    std::cout<<a.get_x()<<"\n"<<a.get_y()<<"\n\n";
    std::cout<<b.get_x()<<"\n"<<b.get_y()<<"\n\n";

    std::cout<<(a==b)<<"\n";
    //std::cout<<a.get_x()<<"\n"<<a.get_y()<<"\n"<<c.get_x()<<"\n"<<c.get_y()<<"\n"<<b.get_x()<<"\n"<<b.get_y()<<"\n";

    //std::cout <<std::hex<< p << "\n";

    std::cout<<std::hex<<g.get_x()<<"\n";
    ar512 ga;
    ga = int_to_ar<ar512>(g.get_x());

    for(size_t i=0; i<64; i++) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<int(ga[i]);
    std::cout<<"\n\n";
    tmp = ar_to_int<ar512>(ga);
    std::cout<<std::hex<<tmp<<"\n";

    ar256 secret;
    ar256 pk;
    ar512 hsh;
    bool success_flag;
    clr(secret);
    std::string  s = "Repent, for tomorow you die!";
    for (size_t i=0; i<s.size(); ++i)
    {
        secret[i] = uint8_t(s[i]);
    }
    std::cout<<secret<<"\n";
    hsh = expand_secret_ed25519(secret,success_flag);
    std::cout<<hsh<<"\n";

    pk = calculate_pk_ed25519(secret, g);

    test1();
    std::cout<<std::hex<<(cint(1)<<255)<<"\n";
    test2();



    return 0;
}
