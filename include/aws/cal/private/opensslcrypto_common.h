#ifndef AWS_C_CAL_OPENSSLCRYPTO_COMMON_H
#define AWS_C_CAL_OPENSSLCRYPTO_COMMON_H

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

typedef HMAC_CTX *(*hmac_ctx_new)(void);
typedef void (*hmac_ctx_free)(HMAC_CTX *);
typedef void (*hmac_ctx_init)(HMAC_CTX *);
typedef void (*hmac_ctx_clean_up)(HMAC_CTX *);
typedef int (*hmac_init_ex)(HMAC_CTX *, const void *, size_t, const EVP_MD *, ENGINE *);
typedef int (*hmac_update)(HMAC_CTX *, const unsigned char *, size_t);
typedef int (*hmac_final)(HMAC_CTX *, unsigned char *, unsigned int *);

/* C standard does not have concept of generic function pointer, but it does
guarantee that function pointer casts will roundtrip when casting to any type and
then back. Use void *(void) as a generic function pointer. */
typedef void (*crypto_generic_fn_ptr)(void);

struct openssl_hmac_ctx_table {
    hmac_ctx_new new_fn;
    hmac_ctx_free free_fn;
    hmac_ctx_init init_fn;
    hmac_ctx_clean_up clean_up_fn;
    hmac_init_ex init_ex_fn;
    hmac_update update_fn;
    hmac_final final_fn;

    /* There is slight variance between the crypto interfaces.
        Note that function pointer casting is undefined behavior.
        To workaround the issue, use generic pointer for crypto and let delegate
        function cast it back to correct type.
        Do not use following fields manually. */
    struct {
        crypto_generic_fn_ptr init_ex_fn;
    } impl;
};

extern struct openssl_hmac_ctx_table *g_aws_openssl_hmac_ctx_table;

typedef EVP_MD_CTX *(*evp_md_ctx_new)(void);
typedef void (*evp_md_ctx_free)(EVP_MD_CTX *);
typedef int (*evp_md_ctx_digest_init_ex)(EVP_MD_CTX *, const EVP_MD *, ENGINE *);
typedef int (*evp_md_ctx_digest_update)(EVP_MD_CTX *, const void *, size_t);
typedef int (*evp_md_ctx_digest_final_ex)(EVP_MD_CTX *, unsigned char *, unsigned int *);

struct openssl_evp_md_ctx_table {
    evp_md_ctx_new new_fn;
    evp_md_ctx_free free_fn;
    evp_md_ctx_digest_init_ex init_ex_fn;
    evp_md_ctx_digest_update update_fn;
    evp_md_ctx_digest_final_ex final_ex_fn;
};

extern struct openssl_evp_md_ctx_table *g_aws_openssl_evp_md_ctx_table;

#endif /* AWS_C_CAL_OPENSSLCRYPTO_COMMON_H */
