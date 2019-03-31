#ifndef CRYPTO_OPS_H
#define CRYPTO_OPS_H
#include "conversions.h"
#include "ed_point.h"
#include "ed25519_params.h"

ar512 expand_secret_ed25519(const ar256 &secret, bool& success);
ar256 compress_point_ed25519(const Ed_point &pt);
Ed_point decompress_point_ed25519(const ar256 &compressed, const Ec_params &params, bool &success);

ar256 calculate_pk_ed25519(const ar256 &sk, const Ed_point &generator);
#endif // CRYPTO_OPS_H
