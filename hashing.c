#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <openssl/evp.h>
#define hash_init(ctx, stat)                                                   \
    *(ctx) = EVP_MD_CTX_new();                                                 \
    stat = !EVP_DigestInit(*(ctx), EVP_sha1())
#define hash_append(ctx, buffer, read_size)                                    \
    EVP_DigestUpdate((ctx), (buffer), (read_size));
#define end_hash(ctx, digest) EVP_DigestFinal(ctx, digest, NULL); free(ctx);

#else
#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#define hash_init(ctx, stat)                                                   \
    HCRYPTPROV context;                                                        \
    CryptAcquireContext(&context, NULL, NULL, PROV_RSA_FULL,                   \
            CRYPT_VERIFYCONTEXT);                                              \
    stat = CryptCreateHash(context, CALG_SHA, 0, 0, (HCRYPTHASH *) (ctx))
#define hash_append(ctx, buffer, read_size)                                    \
    CryptHashData((HCRYPTHASH) (ctx), (buffer), (read_size), 0)
#define end_hash(ctx, digest)                                                  \
    CryptGetHashParam((HCRYPTHASH) (ctx), HP_HASHVAL, (digest), &stat, 0);     \
    CryptDestroyHash((HCRYPTHASH) (ctx));                                      \
    CryptReleaseContext((HCRYPTHASH) (ctx), 0)
#endif

#include "datatypes.h"
#define CHUNK 131072

void hexdigest(unsigned char *digest, char *result) {
    // loop through all chars of digest
    for (int i = 0; i < 20; i++) {
        snprintf(result + 2 * i, 3, "%02x", digest[i]);
    }

    result[40] = '\0';
}

// returns the hash value of the file
// returns NULL if run into an error
char *hash(char *path, size_t size) {
    // SHA struct init
    unsigned char digest[20];
    void *ctx;
    unsigned long stat;
    hash_init(&ctx, stat);
    if (stat) {
        printf("There was a problem initialising the hasher");
        return NULL;
    }

    // try to open file
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    // loops through the whole file
    unsigned char buffer[CHUNK];
    for (int i = 0; i < size; i += CHUNK) {
        size_t read_size = fread(buffer, 1, CHUNK, f);
        hash_append(ctx, buffer, read_size);
    }

    fclose(f);

    // retrieve digest and free
    end_hash(ctx, digest);

    // parse to hexadecimal
    char *result = malloc(41 * sizeof(char));
    hexdigest(digest, result);
    return result;
}
