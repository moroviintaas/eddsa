#include <iostream>
#include "ed25519_params.h"
#include <iomanip>
#include "ed_point.h"
#include "conversions.h"

//ar512 (*int_to_ar512)(const cint& n) = &int_to_ar<ar512>;

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

    for(int i=0; i<64; i++) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)ga[i];
    std::cout<<"\n\n";
    tmp = ar_to_int<ar512>(ga);
    std::cout<<std::hex<<tmp<<"\n";


    return 0;
}
