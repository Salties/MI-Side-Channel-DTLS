#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dtls.h"
#include "ecc/ecc.h"
#include "tinycrypto.h"
#include "numeric.h"
#include "prng.h"

#define KEYSIZE 8
#define RANDSIZE 8
#define HEX 16

#define DTLS_SKEXEC_LENGTH (1 + 2 + 1 + 1 + DTLS_EC_KEY_SIZE + DTLS_EC_KEY_SIZE + 1 + 1 + 2 + 70)

unsigned char signkey[DTLS_EC_KEY_SIZE] = { 0 };
unsigned char secretkey[DTLS_EC_KEY_SIZE] = { 0 };

void PrintInt256(const char *title, unsigned char *i256)
{
    int i;

    printf("%s", title);
    for (i = 0; i < DTLS_EC_KEY_SIZE; i++) {
        printf("%02X", i256[i]);
        if ((i + 1) % 4 == 0)
            printf(" ");
    }
    printf("\n");
    return;
}

void str2hex(char *str, unsigned char *hexval, size_t len)
{
    int nvalid = 0;
    size_t count;
    char *cptr;
    char cbuf[3] = { 0 };

    for (cptr = str, count = 0; count < len; count++) {
        //Read two hex characters to cbuf.
        for (nvalid = 0; nvalid < 2; nvalid++, cptr++) {
            //Find the one valid character each loop.
            //Skip redundant characters.
            for (; *cptr == ' ' || *cptr == ','; cptr++);
            //Terminate on '\0', ',' or '\n'.
            if (*cptr == '\0' || *cptr == '\n')
                break;
            //Save to cbuf.
            cbuf[nvalid] = *cptr;
        }
        //Convert to integer.
        hexval[count] = strtoul(cbuf, NULL, HEX);
        //Reset buffer.
        memset(cbuf, 0, sizeof(cbuf));
    }

    return;
}


void PrintSigMsg(const unsigned char *client_random,
                 size_t client_random_size,
                 const unsigned char *server_random,
                 size_t server_random_size,
                 const unsigned char *keyx_params, size_t keyx_params_size)
{

    dtls_hash_ctx data;
    unsigned char sha256hash[DTLS_HMAC_DIGEST_SIZE];

    dtls_hash_init(&data);
    dtls_hash_update(&data, client_random, client_random_size);
    dtls_hash_update(&data, server_random, server_random_size);
    dtls_hash_update(&data, keyx_params, keyx_params_size);
    dtls_hash_finalize(sha256hash, &data);
    //Print the hashed plaintext.
    PrintInt256("#HashedPlaintext: ", sha256hash);

    return;
}

static int PrepareServerKeyExchange(unsigned char *clientrand,
                                    unsigned char *serverrand)
{
    /* The ASN.1 Integer representation of an 32 byte unsigned int could be
     * 33 bytes long add space for that */
    uint8 buf[DTLS_SKEXEC_LENGTH + 2];
    uint8 *p;
    uint8 *key_params;
    uint8 *ephemeral_pub_x;
    uint8 *ephemeral_pub_y;
    uint32_t point_r[9];
    uint32_t point_s[9];

    /* ServerKeyExchange 
     *
     * Start message construction at beginning of buffer. */
    p = buf;

    key_params = p;
    /* ECCurveType curve_type: named_curve */
    dtls_int_to_uint8(p, 3);
    p += sizeof(uint8);

    /* NamedCurve namedcurve: secp256r1 */
    dtls_int_to_uint16(p, TLS_EXT_ELLIPTIC_CURVES_SECP256R1);
    p += sizeof(uint16);

    dtls_int_to_uint8(p, 1 + 2 * DTLS_EC_KEY_SIZE);
    p += sizeof(uint8);

    /* This should be an uncompressed point, but I do not have access to the spec. */
    dtls_int_to_uint8(p, 4);
    p += sizeof(uint8);

    /* store the pointer to the x component of the pub key and make space */
    ephemeral_pub_x = p;
    p += DTLS_EC_KEY_SIZE;

    /* store the pointer to the y component of the pub key and make space */
    ephemeral_pub_y = p;
    p += DTLS_EC_KEY_SIZE;

    dtls_ecdsa_generate_key(secretkey,
                            ephemeral_pub_x, ephemeral_pub_y,
                            DTLS_EC_KEY_SIZE);

    /* sign the ephemeral and its paramaters */
    dtls_ecdsa_create_sig(signkey, DTLS_EC_KEY_SIZE,
                          clientrand, DTLS_RANDOM_LENGTH,
                          serverrand, DTLS_RANDOM_LENGTH,
                          key_params, p - key_params, point_r, point_s);

    p = dtls_add_ecdsa_signature_elem_wrapper(p, point_r, point_s);
   
    //Print variables.
    PrintInt256("#SecretKey: ", secretkey);
    PrintInt256("#PublicKeyX: ", ephemeral_pub_x);
    PrintInt256("#PublicKeyY: ", ephemeral_pub_y);
    PrintSigMsg(clientrand, DTLS_RANDOM_LENGTH,
                serverrand, DTLS_RANDOM_LENGTH,
                key_params, p - key_params);

    return 0;
}

int main()
{
    char *input = NULL;
    size_t len = 0;
    unsigned char clientrand[DTLS_RANDOM_LENGTH] = { 0 };
    unsigned char serverrand[DTLS_RANDOM_LENGTH] = { 0 };

    //Read in signing key.
    printf("#Enter signing key: ");
    getline(&input, &len, stdin);
    str2hex(input, signkey, DTLS_EC_KEY_SIZE);
    free(input);
    input = NULL;
    len = 0;
    PrintInt256("#SigningKey: ", signkey);

    for (;;) {
        dtls_prng(clientrand, DTLS_RANDOM_LENGTH);
        dtls_prng(serverrand, DTLS_RANDOM_LENGTH);
        PrintInt256("#ClientRandomness: ", clientrand);
        PrintInt256("#ServerRandomness: ", serverrand);
        PrepareServerKeyExchange(clientrand, serverrand);
        printf("\n");
        sleep(1);
    }
    return 0;
}
