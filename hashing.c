#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef _WIN32
#include <openssl/evp.h>
#else
#include <windows.h>
#include <wincrypt.h>
#endif

int hash_init(void *ctx) {
#ifndef _WIN32
    *ctx = EVP_MD_CTX_new();
    return !EVP_DigestInit(*ctx, EVP_sha1());
#else
    HCRYPTPROV context;
    CryptAcquireContext(&context, NULL, NULL, PROV_RSA_FULL,
                        CRYPT_VERIFYCONTEXT);
    return !CryptCreateHash(context, CALG_SHA, 0, 0, (HCRYPTHASH *)ctx);
#endif
}

void hash_append(void *ctx, unsigned char *buffer, size_t read_size) {
#ifndef _WIN32
    EVP_DigestUpdate(ctx, buffer, read_size);
#else
    CryptHashData((HCRYPTHASH)ctx, buffer, read_size, 0);
#endif
}

void end_hash(void *ctx, void *digest) {
#ifndef _WIN32
    EVP_DigestFinal(ctx, digest, NULL);
    free(ctx);
#else
    unsigned long d_size = 21;
    CryptGetHashParam((HCRYPTHASH)ctx, HP_HASHVAL, digest, &d_size, 0);
    CryptDestroyHash((HCRYPTHASH)ctx);
    CryptReleaseContext((HCRYPTHASH)ctx, 0);
#endif
}

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
char *hash(char *path, unsigned long long size) {
    // SHA struct init
    unsigned char digest[20];
    void *ctx;
    if (hash_init(&ctx)) {
        printf("There was a problem initialising the hasher");
        return NULL;
    }

    // try to open file
    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;

    // loops through the whole file
    unsigned char buffer[CHUNK];
    for (unsigned long long i = 0; i < size; i += CHUNK) {
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
