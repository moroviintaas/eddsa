#ifndef CRYPTO_OPS_H
#define CRYPTO_OPS_H
#include "conversions.h"
#include "ed_point.h"
#include "ed25519_params.h"
#include <vector>
#include <stdint.h>

ar512 hash_sha512(const std::vector<uint8_t> &m);
ar512 expand_secret_ed25519(const ar256 &secret);
ar256 compress_point_ed25519(const Ed_point &pt);
Ed_point decompress_point_ed25519(const ar256 &compressed,  bool &success,const Ec_params &params =ed25519_params);

cint calculate_a_from_sk(const ar512 &esk);
ar256 calculate_pk_ed25519_from_expanded_sk(const ar512 &esk, const Ed_point &generator =ed25519_generator);
ar256 calculate_pk_ed25519(const ar256 &sk, const Ed_point &generator =ed25519_generator);
ar512 sign_ed15519(const ar256 &sk, const std::vector<uint8_t> &msg, const Ed_point &generator =ed25519_generator);
bool verify_ed12519(const ar256 &pk, const std::vector<uint8_t> &msg, const ar512 &signature, const Ed_point &generator =ed25519_generator);
#endif // CRYPTO_OPS_H
