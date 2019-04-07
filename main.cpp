#include <iostream>
#include "ed25519_params.h"
#include <iomanip>
#include "ed_point.h"
#include "conversions.h"
#include "crypto_ops.h"
#include "boost/program_options.hpp"
#include <random>

#ifndef CINT
#define CINT
typedef mpz_class cint;
#endif //CINT


namespace  po = boost::program_options;
//ar512 (*int_to_ar512)(const cint& n) = &int_to_ar<ar512>;

void test1()
{
    std::cout<<"===================================\nTest 1:\n";
    cint n = cint("0x607fae1c03ac3b701969327b69c54944c42cec92f44a84ba605afdef9db1619d");
    Ed_point g(PX,PY);
    ar512 signature;
    std::vector<uint8_t> msg = {};
    ar256 sk = int_to_ar<ar256>(n);
    //std::cout<<"a\n";
    ar256 pk = calculate_pk_ed25519(sk,g);

    signature = sign_ed15519(sk,msg);
    std::cout<<"signature:\t"<<signature<<"\n";
    std::cout<<"pk:\t"<<pk<<"\n";

    std::cout<<verify_ed12519(pk,msg,signature)<<"\n";


}
void test1_1()
{
    std::cout<<"===================================\nTest 1:\n";
    cint n = cint("0xfba6b84fedf68cda24a6ab359f318a5b0f4e11ec46c3b69dda96ff289b08cd4c");
    Ed_point g(PX,PY);
    ar512 signature;
    std::vector<uint8_t> msg = {0x72};
    ar256 sk = int_to_ar<ar256>(n);
    //std::cout<<"a\n";
    ar256 pk = calculate_pk_ed25519(sk,g);

    signature = sign_ed15519(sk,msg);
    std::cout<<"signature:\t"<<signature<<"\n";
    std::cout<<"pk:\t"<<pk<<"\n";

    std::cout<<verify_ed12519(pk,msg,signature)<<"\n";
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
    tp2 = decompress_point_ed25519(csd,success,ed25519_params);
    std::cout<<success<<"\t"<<(tp==tp2)<<"\n"<<tp2.get_x()<<"\n"<<tp2.get_y()<<"\n";


}

void show_help(po::options_description &o)
{
    std::cout<<o<<"\n";
}

cint import_cint_from_file(std::string filename, bool &success){
    std::ifstream ifs;
    std::string buffer;
    ifs.open(filename);
    cint result;
    if (ifs.good()){
        ifs>>buffer;
        result = cint(buffer);
        ifs.close();
        success = true;
    }
    else success = false;
    return result;
}

int vectorize_binary_file(const std::string &filename, std::vector<uint8_t> &result)
{
    std::ifstream ifs;
    result.clear();
    char c;
    ifs.open(filename, std::ios_base::in | std::ios_base::binary);
    while( ifs.good()){
        ifs.get(c);
        result.push_back(uint8_t(c));
    }
    result.pop_back();


    ifs.close();
    return 0;
}
void string_to_vec(std::vector<uint8_t> vec, const std::string &s){
    vec.clear();
    vec = std::vector<uint8_t>(s.size());
    for (size_t i =0; i<s.size();++i) {
        vec[i] = uint8_t(s[i]);
    }
}



int main(int argc, const char *argv[])
{
    std::string key_filename, pk_filename, msg_filename, signature_filename, cfg_file_name =  "settings.cfg";
    std::string mode;
    std::string key, pk, signature, msg;
    std::ifstream ifs;
    std::ofstream ofs;

    std::random_device rd;
    std::uniform_int_distribution<uint32_t> d(0, UINT32_MAX);
    //std::ifstream rin("/dev/urandom");

    size_t key_size_bits = 256;
    bool quiet = false;

    Ed_point generator(PX,PY);

    po::options_description generic("Opcje wiersza poleceń");
    generic.add_options()
            ("help,h", "Wyświetl ten ekran pomocy.")
            ("mode", po::value<std::string>(&mode), "Tryb pracy, dostępne to:\n\n\t"
                                                    "keygen - generowanie pseudolosowego klucza publicznego\n\n\t"
                                                    "public - oblczanie klucza publicznego na podstawie sk, należy podać key lub key_file)\n\n\t"
                                                    "sign - podpis wiadomości, należy podać (sk lub sk_file) oraz (message lub message_file)\n\n\t"
                                                    "ver - weryfikacja podpisu, należy podać (pk lub pk_file), (msg lub msg_file oraz (signature lub signature_file)\n\n")
            ("config_file,c", po::value<std::string>(&cfg_file_name)->default_value(std::string("settings.cfg")),"Nazwa pliku konfiguracyjnego\n")
            ("key,k", po::value<std::string>(&key), "Klucz prywatny w postaci liczby w systemie szesnastkowym (0xabcd)")
            ("pk,p", po::value<std::string>(&pk), "Klucz publiczny w postaci liczby w systemie szesnastkowym (0xabcd)")
            ("msg,m", po::value<std::string>(&msg), "Wiadomość w postaci liczby w systemie szesnastkowym (0xabcd)")
            ("signature,s", po::value<std::string>(&signature), "Podpis w postaci liczby w systemie szesnastkowym (0xabcd)");





    po::options_description config("Opcje ");
    config.add_options()
            ("key_file,K", po::value<std::string>(&key_filename), "Nazwa pliku do odczytania/zapisania klucza prywatnego (priorytetowa jest opcja key)")
            ("pk_file,P", po::value<std::string>(&pk_filename), "Nazwa pliku do odczytania/zapisania klucza publicznego (priorytetowa jest opcja pk)")
            ("msg_file,F", po::value<std::string>(&msg_filename), "Nazwa pliku do odczytania/zapisania wiadomości (priorytetowa jest opcja msg)")
            ("signature_file,S", po::value<std::string>(&signature_filename), "Nazwa pliku do odczytania/zapisania podpisu (priorytetowa jest opcja signature)")
            ("quiet,q", "Bez wypisywania na ekran, wtedy trzeba podać wszelkie wymagane wejścia");


    po::positional_options_description pos;
    pos.add("mode", 1);

    po::variables_map vm;
    po::options_description all_options;
    all_options.add(generic).add(config);

    po::store(po::command_line_parser(argc,argv).options(all_options).positional(pos).run(), vm);
    po::notify(vm);

    ifs.open((cfg_file_name));
    po::store(po::parse_config_file(ifs, config), vm);
    po::notify(vm);
    ifs.close();

    if(vm.count("help")){
        show_help(all_options);
    }
    if(vm.count("quiet")){
        quiet = true;
    }

    if(vm.count("mode")){
        if(mode == "keygen"){
            if(vm.count("quiet") && !vm.count("key_file"))
            {
                std::cout<<"Brak wyjścia!\n";
                return 0;
            }
            else{
                cint key = 0;
                for (size_t i=0; i<key_size_bits/sizeof (uint32_t)/8; ++i){
                    key<<=32;
                    key^=(d(rd)&0xffffffff);
                }
                if(!quiet) std::cout<<std::hex<<key<<"\n";
                if(vm.count("key_file")){
                    ofs.open(key_filename);
                    ofs.setf(std::ios_base::hex);
                    ofs<<"0x"<<std::hex<<key<<"\n";
                    ofs.close();

                }
                return 0;

            }

        }
        else if(mode == "public"){
            cint pub_key, key;
            std::string buffer;
            if(vm.count("quiet") && !vm.count("pk_file"))
            {
                std::cout<<"Brak wyjścia!\n";
                return 0;
            }

            if(vm.count("key")){
                pub_key = cint(pk);
            }
            else{
                if(vm.count("key_file")){
                    ifs.open(key_filename);
                    if (ifs.good()){
                        ifs>>buffer;
                        key = cint(buffer);
                        ifs.close();
                    }
                    else{
                        std::cerr<<"Błąd otwarcia key_file\n";
                        return 0;
                    }

                }
                else{
                    std::cerr<<"Błąd, potrzebny klucz prywatny";
                    return 0;
                }
            }
            pub_key = ar_to_int<ar256>(calculate_pk_ed25519(int_to_ar<ar256>(key), generator ));
            if(!quiet) std::cout<<std::hex<<pub_key<<"\n";
            if(vm.count("pk_file")){
                ofs.open(pk_filename);
                ofs.setf(std::ios_base::hex);
                ofs<<"0x"<<std::hex<<pub_key<<"\n";
                ofs.close();

            }
            return 0;

        }
        else if(mode =="sign"){
            if(vm.count("quiet") && !vm.count("signature_file"))
            {
                std::cout<<"Brak wyjścia!\n";
                return 0;
            }
            std::string buffer;
            cint c_sk, c_msg;
            std::vector<uint8_t> msg_v;
            bool success;
            bool message_present = false;
            bool key_present = false;

            if(vm.count("key")){
                c_sk = cint(key);
                key_present  =true;
            }
            else{
                if(vm.count("key_file"))
                {
                    c_sk = import_cint_from_file(key_filename, success);
                    if(!success){
                        std::cerr<<"Nie udało się oddtworzyć klucza prywatnego z pliku\n";
                        return 0;
                    }
                    else{
                        key_present = true;
                    }


                }
                else{
                    std::cerr<<"Błąd, potrzebny klucz prywatny!\n";
                    return 0;
                }

            }

            //TODO message parse

            if(vm.count("msg")){
                string_to_vec(msg_v, msg);
                message_present = true;
            }
            else if(vm.count("msg_file")){

                vectorize_binary_file(msg_filename, msg_v);
                message_present = true;


            }
            else{
                std::cerr<<"Błąd, potrzebna wiadomość!\n";
                return 0;
            }

            if(message_present && key_present){
                ar512 signature;
                signature = sign_ed15519(int_to_ar<ar256>(c_sk),msg_v);
                if(!quiet) std::cout<<std::hex<<ar_to_int<ar512>(signature)<<"\n";
                if(vm.count("signature_file")){
                    ofs.open(signature_filename, std::ios_base::out | std::ios_base::binary);
                    drop_ar(signature, ofs);
                    ofs.close();
                }


            }



        }
        else if(mode == "ver"){
            if(vm.count("quiet"))
            {
                std::cout<<"Weryfikacja na ekran!\n";
                return 0;
            }
            //TODO
        }

    }




    /*Ed_point a(ed25519_params),b(ed25519_params),c;
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
    hsh = expand_secret_ed25519(secret);
    std::cout<<hsh<<"\n";

    pk = calculate_pk_ed25519(secret, g);

    test1();
    test1_1();
    //std::cout<<std::hex<<(cint(1)<<255)<<"\n";
    //test2();
    //std::string tt = "a\0n";

    */




    return 0;
}
