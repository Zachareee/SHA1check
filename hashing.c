#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#include "datatypes.h"
#define CHUNK 65536

char byte_to_hex(char b) {
    switch (b) {
        case 10:
            return 'a';
        case 11:
            return 'b';
        case 12:
            return 'c';
        case 13:
            return 'd';
        case 14:
            return 'e';
        case 15:
            return 'f';
        default:
            return b + '0';
    }
}

void hexdigest(unsigned char *digest, char *result) {
    // loop through all chars of digest
    for (int i = 0; i < 20; i++) {
        unsigned char c = digest[i];

        // mask the top 4 and bottom 4 bits and
        // convert to hex
        char top = (c & 0xf0) >> 4;
        char bot = c & 0x0f;
        result[2 * i] = byte_to_hex(top);
        result[2 * i + 1] = byte_to_hex(bot);
    }
}

// returns the hash value of the file
// returns NULL if run into an error
char *hash(file_struct file) {
    // SHA struct init
    unsigned char digest[20];
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!EVP_DigestInit(ctx, EVP_sha1())) {
        printf("There was a problem initialising the hasher");
        return NULL;
    }

    // try to open file
    FILE *f = fopen(file.name, "r");
    if (!f) return NULL;

    // loops through the whole file
    unsigned char buffer[CHUNK];
    for (int i = 0; i < file.size; i += CHUNK) {
        size_t size = fread(buffer, 1, CHUNK, f);
        EVP_DigestUpdate(ctx, buffer, size);
    }

    // close file
    fclose(f);

    // retrieve digest and parse to hexadecimal
    EVP_DigestFinal(ctx, digest, NULL);
    char *result = malloc(41 * sizeof(char));
    hexdigest(digest, result);
    result[40] = '\0';
    return result;
}
