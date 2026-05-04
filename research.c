#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#define TIME_SLOT 5

double diff_ms(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * 1000.0 +
           (b.tv_nsec - a.tv_nsec) / 1e6;
}

int main() {
    struct timespec t1, t2;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned int hmac_len;

    const char ID[] = "device009";
    const char secret[] = "K009";

    /* -------- t1 : SHA256(ID) -------- */
    clock_gettime(CLOCK_MONOTONIC, &t1);
    SHA256((unsigned char*)ID, strlen(ID), hash);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    double t_sha = diff_ms(t1, t2);

    /* Convert hash → integer (simplified) */
    uint64_t idint = 0;
    for (int i = 0; i < 8; i++)
        idint = (idint << 8) | hash[i];

    /* -------- t2 : Division Rule -------- */
    clock_gettime(CLOCK_MONOTONIC, &t1);
    uint64_t ts = time(NULL);
    uint64_t Q = idint / ts;
    uint64_t R = idint % ts;
    clock_gettime(CLOCK_MONOTONIC, &t2);
    double t_div = diff_ms(t1, t2);

    /* -------- t3 : HMAC(Q||R||ts) -------- */
    char msg[128];
    snprintf(msg, sizeof(msg), "%lu%lu%lu", Q, R, ts);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    HMAC(EVP_sha256(), secret, strlen(secret),
         (unsigned char*)msg, strlen(msg),
         NULL, &hmac_len);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    double t_hmac = diff_ms(t1, t2);

    printf("SHA256 time      : %.3f ms\n", t_sha);
    printf("Division time    : %.3f ms\n", t_div);
    printf("HMAC time        : %.3f ms\n", t_hmac);
    printf("-------------------------------\n");
    printf("Total Crypto Time: %.3f ms\n",
           t_sha + t_div + t_hmac);

    return 0;
}
