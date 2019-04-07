#include "crypto_ops.h"
#include <openssl/evp.h>

ar512 hash_sha512(const std::vector<uint8_t> &m)
{
    ar512 result;
    //bool success = false;
    uint8_t size_of_hash = 512/8;

    const EVP_MD *evp_sha = EVP_sha512();
    EVP_MD_CTX *context = EVP_MD_CTX_new();

    if(context != nullptr)
    {
        if(EVP_DigestInit_ex(context, evp_sha, nullptr))
        {
            if(EVP_DigestUpdate(context, m.data(), m.size()))
            {
                //rstd::cout<<"buff:\t\t";
                //rfor (size_t i=0; i<m.size(); ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<int(m.data()[i]);
                //rstd::cout<<"\n";
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
                        //success = true;

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

}

ar512 expand_secret_ed25519(const ar256 &secret)//, bool &success)
{
    ar512 result;
    //success = false;
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
                        //success = true;

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

Ed_point decompress_point_ed25519(const ar256 &compressed,bool &success,const Ec_params &params)
{
    uint8_t x_0 = compressed[31]>>7;
    cint x,y,u,v,x2,t,e;
    //Ed_point result(ed25519_params);
    Ed_point result;
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
        success = true;
        //result =  Ed_point(x,y,ed25519_params);
    }
    else if(t == p-u)
    {
        success =true;
        e = (p-1)/4;
        x =mod(x*powm(cint(2),e,p),p);
        //result =  Ed_point(x,y,ed25519_params);
    }
    else {
        success  =false;
        std::cerr<<"Błąd nie można znaleźć pierwiastka kwadratowego.\n";
        return Ed_point();

    }
    if (success)
    {
        if(x == 0 && x_0 != 0){
            success = false;
            return Ed_point();
        }
        else if(x_0%2 != x%2){
            x = p-x;

        }
         return  Ed_point(x,y,ed25519_params);
    }
    std::cerr<<"Błąd przy dekompresji punktu, nieznany - moim zdaniem nie powinien się pojawić.\n";
    return Ed_point();




}

cint calculate_a_from_sk(const ar512 &esk)
{
    ar256 pk;
    cint s;
    //cint x,y;
    //Ed_point pk_point;
    for (size_t i=0; i<pk.max_size(); i++)
    {
        pk[i] = esk[i];
    }

    pk[0] =  pk[0]  & 0xf8;
    pk[31] = pk[31] & 0x7f;
    pk[31] = pk[31] | 0x40;

    s = ar_to_int(pk);
    return s;
}

ar256 calculate_pk_ed25519_from_expanded_sk(const ar512 &esk, const Ed_point &generator)
{
    /*ar256 pk;
    cint s;
    //cint x,y;
    Ed_point pk_point;
    for (size_t i=0; i<pk.max_size(); i++)
    {
        pk[i] = esk[i];
    }

    pk[0] =  pk[0]  & 0xf8;
    pk[31] = pk[31] & 0x7f;
    pk[31] = pk[31] | 0x40;

    s = ar_to_int(pk);


    pk_point = generator*s;
    pk = compress_point_ed25519(pk_point);
    return pk;
    */
    return compress_point_ed25519(generator*calculate_a_from_sk(esk));
}


ar256 calculate_pk_ed25519(const ar256 &sk, const Ed_point &generator)
{
    //bool hash_success;
    ar512 hsh = expand_secret_ed25519(sk);
    /*
    ar256 pk;
    cint s;
    //cint x,y;
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
    //std::cout<<pk<<"\n";
    return pk;
    */
    return calculate_pk_ed25519_from_expanded_sk(hsh);

}

ar512 sign_ed15519(const ar256 &sk, const std::vector<uint8_t> &msg, const Ed_point &generator)
{
    ar512 buff512 = expand_secret_ed25519(sk);
    cint a,r,h,s;
    ar256 prefix, A, Rs, buff256;
    std::vector<uint8_t> buffer;
    Ed_point R, dbg, hA, t;;
    bool success;

    buff256 = calculate_pk_ed25519_from_expanded_sk(buff512);
    //std::cout<<"buff256:\t"<<buff256<<"\n";
    //a = ar_to_int<ar256>(calculate_pk_ed25519_from_expanded_sk(buff512));
    a = calculate_a_from_sk(buff512);
    //set prefix:
    for(size_t i=0; i<prefix.max_size(); ++i){
        prefix[i] = buff512[32+i];
    }
    A = compress_point_ed25519(generator * a);


    buffer.clear();
    for (size_t i=0; i<prefix.max_size();i++){
        buffer.push_back(prefix[i]);
    }
    for (size_t i=0; i<msg.size(); ++i){
        buffer.push_back(msg[i]);
    }

    r = mod(ar_to_int<ar512>(hash_sha512(buffer)),generator.get_ec_order());

    R = generator *r;
    //std::cout<<"R:\n"<<R<<"\n\n";
    Rs = compress_point_ed25519(R);
    //std::cout<<"Rs:\t"<<Rs<<"\n";
    dbg = decompress_point_ed25519(Rs,success);
    if(!(dbg == R)){
        std::cerr<<"Failure: decompresing R.\n";
        exit(-1);

    }
    //std::cout<<"Rs s:\t"<<Rs<<"\n";

    buffer.clear();
    //std::cout<<"A max size:\t"<<A.max_size()<<"\n";
    for (size_t i=0; i<Rs.max_size();i++){
        buffer.push_back(Rs[i]);
    }
    for (size_t i=0; i<A.max_size(); ++i){
        buffer.push_back(A[i]);
    }
    for (size_t i=0; i<msg.size(); ++i){
        buffer.push_back(msg[i]);
    }
    //std::cout<<std::dec<<buffer.size()<<"\n";
    h = mod(ar_to_int<ar512>(hash_sha512(buffer)),generator.get_ec_order());

    //std::cout<<std::dec<<"order:\t"<<generator.get_ec_order()<<"\n";
    s = mod( r + (h * a), generator.get_ec_order());
    //std::cout<<"s:\t"<<std::hex<<s<<"\n";
    buff256 = int_to_ar<ar256>(s);
    //std::cout<<"p2 s\t"<<buff256<<"\n";
    for (size_t i=0; i<Rs.max_size(); ++i){
        buff512[i] = Rs[i];
        buff512[i+Rs.max_size()] = buff256[i];
    }

    hA = (decompress_point_ed25519(A,success)*h);
    t = hA + R;
    //std::cout<<"hA_x:\t"<<hA.get_x()<<"\n";
    //std::cout<<"sB_x:\t"<<(generator*s).get_x()<<"\n";
    //std::cout<<"R_x:\t"<<R.get_x()<<"\n";
    //std::cout<<"t_x:\t"<<t.get_x()<<"\n";
    return buff512;


}






bool verify_ed12519(const ar256 &pk, const std::vector<uint8_t> &msg, const ar512 &signature, const Ed_point &generator)
{
    Ed_point A, R, sB, hA, t;
    ar256 buff256, Rs;
    //ar512 buff512;
    cint a,h,s;
    std::vector<uint8_t> buffer;
    bool success;

    A = decompress_point_ed25519(pk,success);
    //std::cout<<success<<"\n";

    for (size_t i= 0 ; i<Rs.max_size(); i++){
        Rs[i] = signature[i];
    }
   // std::cout<<"Rs v:\t"<<Rs<<"\n";
    R = decompress_point_ed25519(Rs,success);
    //std::cout<<success<<"\n";
    for (size_t i= Rs.max_size() ; i<Rs.max_size() + buff256.max_size(); i++){
        buff256[i - Rs.max_size()] = signature[i];
    }
    //std::cout<<"p2 v\t"<<buff256<<"\n";
    s = ar_to_int<ar256>(buff256);

    buffer.clear();
    for (size_t i=0; i<Rs.max_size();i++){
        buffer.push_back(Rs[i]);
    }
    for (size_t i=0; i<pk.max_size(); ++i){
        buffer.push_back(pk[i]);
    }
    for (size_t i=0; i<msg.size(); ++i){
        buffer.push_back(msg[i]);
    }

    h = mod(ar_to_int<ar512>(hash_sha512(buffer)),generator.get_ec_order());

    sB = generator * s;
    hA = A * h;

    //std::cout<<"sB:\n"<<sB<<"\n\n";
    t = hA + R;
    //std::cout<<"tt:\n"<<t<<"\n\n";
    //
    //std::cout<<"hA_x:\t"<<(hA).get_x()<<"\n";
    //std::cout<<"sB_x:\t"<<sB.get_x()<<"\n";
    //std::cout<<"R_x:\t"<<R.get_x()<<"\n";
    //std::cout<<"t_x:\t"<<t.get_x()<<"\n";

    /*std::cout<<"hA_y:\t"<<(hA).get_y()<<"\n";
    std::cout<<"sB_y:\t"<<sB.get_y()<<"\n";
    std::cout<<"R_y:\t"<<R.get_y()<<"\n";
    std::cout<<"t_y:\t"<<t.get_y()<<"\n";*/

    return (sB == t);


}
