#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <openssl/evp.h>
#else
#include <stdio.h>
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
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!EVP_DigestInit(ctx, EVP_sha1())) {
        printf("There was a problem initialising the hasher");
        return NULL;
    }

    // try to open file
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    // loops through the whole file
    unsigned char buffer[CHUNK];
    for (int i = 0; i < size; i += CHUNK) {
        size_t size = fread(buffer, 1, CHUNK, f);
        EVP_DigestUpdate(ctx, buffer, size);
    }

    fclose(f);

    // retrieve digest and free
    EVP_DigestFinal(ctx, digest, NULL);
    free(ctx);

    // parse to hexadecimal
    char *result = malloc(41 * sizeof(char));
    hexdigest(digest, result);
    return result;
}
