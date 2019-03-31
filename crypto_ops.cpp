#include "crypto_ops.h"
#include <openssl/evp.h>

ar512 expand_secret_ed25519(const ar256 &secret, bool &success)
{
    ar512 result;
    success = false;
    uint32_t size_of_hash = 512/8;

    const EVP_MD *evp_sha = EVP_sha512();
    EVP_MD_CTX *context = EVP_MD_CTX_new();

    if(context != nullptr)
    {
        if(EVP_DigestInit_ex(context, evp_sha, nullptr))
        {
            if(EVP_DigestUpdate(context, secret.data(), secret.size()))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    //std::stringstream ss;
                    if(lengthOfHash <= size_of_hash)
                    {
                        for(unsigned int i = 0; i < lengthOfHash; ++i)
                        {
                            result[i] = hash[i];
                        }
                        size_of_hash = lengthOfHash;
                        success = true;

                    }
                    else
                    {
                        std::cout<<"API length:"<<std::dec<<lengthOfHash<<"\n";
                        std::cout<<"Demanded length:"<<size_of_hash<<"\n";
                        std::cout<<"hash fails at 1\n";
                    }

                }
                else std::cout<<"hash fails at 2\n";
            }
            else std::cout<<"hash fails at 3\n";
        }
        else std::cout<<"hash fails at 4\n";

        EVP_MD_CTX_free(context);
    }
    return result;

    //delete evp_sha;
}
ar256 compress_point_ed25519(const Ed_point &pt)
{
    ar256 compressed = int_to_ar<ar256>(pt.get_y());
    compressed[31] |= (uint8_t(mpz_tstbit(pt.get_x().get_mpz_t(),0))<<7);
    return compressed;
}

Ed_point decompress_point_ed25519(const ar256 &compressed,const Ec_params &params, bool &success)
{
    uint8_t x_0 = compressed[31]>>7;
    cint x,y,u,v,x2,t,e;
    //Ed_point result(ed25519_params);
    const cint &p = params.modulus;

    if(x_0 != 0){
        y = ar_to_int(compressed)-(cint(1)<<255);
    }
    else {
        y = ar_to_int(compressed);

    }

    if(y>p){
        std::cerr<<"Error decoding point, y > p\n";
        success  =false;
        return Ed_point();
    }
    u = mod(((y*y)-1),p);
    v = mod(((params.d*y*y)+1),p);
    t = u* inv(v,p);
    e = (p+3)/8;
    //mpz_powm_sec(x.get_mpz_t(),t.get_mpz_t(),e.get_mpz_t(), p.get_mpz_t());
    x = powm(t,e,p);
    x2 = mod(x*x,p);
    t = mod(v*x2,p);
    if(t == u)
    {
        success = 1;
        return Ed_point(x,y,ed25519_params);
    }
    else if(t == p-u)
    {
        success =1;
        e = (p-1)/4;
        x =mod(x*powm(cint(2),e,p),p);
        return Ed_point(x,y,ed25519_params);
    }
    else {
        success  =false;
        std::cerr<<"Error: cannot find x to be square root.\n";
        return Ed_point();

    }



}

ar256 calculate_pk_ed25519(const ar256 &sk, const Ed_point &generator)
{
    bool hash_success;
    ar512 hsh = expand_secret_ed25519(sk,hash_success);
    ar256 pk;
    cint s;
    cint x,y;
    Ed_point pk_point;
    for (size_t i=0; i<pk.max_size(); i++)
    {
        pk[i] = hsh[i];
    }

    //std::cout<<pk<<"\n";
    pk[0] =  pk[0]  & 0xf8;
    pk[31] = pk[31] & 0x7f;
    pk[31] = pk[31] | 0x40;

    s = ar_to_int(pk);
    //std::cout<<std::hex<<s<<"\n";
    pk_point = generator*s;
    //pk = int_to_ar<ar256>(pk_point.get_y());
    //pk[31] = pk[31] | uint8_t(mpz_tstbit(pk_point.get_x().get_mpz_t(),0));
    pk = compress_point_ed25519(pk_point);
    //std::cout<<hsh<<"\n";
    std::cout<<pk<<"\n";

}
