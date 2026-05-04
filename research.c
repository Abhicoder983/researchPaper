#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#define TIME_SLOT 5

double diff_ms(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER freq) {
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
}

int main() {
    LARGE_INTEGER t1, t2, freq;
    QueryPerformanceFrequency(&freq);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned int hmac_len;

    const char ID[] = "device009";
    const char secret[] = "K009";

    /* -------- t1 : SHA256(ID) -------- */
    QueryPerformanceCounter(&t1);
    SHA256((unsigned char*)ID, strlen(ID), hash);
    QueryPerformanceCounter(&t2);
    double t_sha = diff_ms(t1, t2, freq);

    /* Convert hash → integer (simplified) */
    uint64_t idint = 0;
    for (int i = 0; i < 8; i++)
        idint = (idint << 8) | hash[i];

    /* -------- t2 : Division Rule -------- */
    QueryPerformanceCounter(&t1);
    uint64_t ts = (uint64_t)time(NULL);
    uint64_t Q = idint / ts;
    uint64_t R = idint % ts;
    QueryPerformanceCounter(&t2);
    double t_div = diff_ms(t1, t2, freq);

    /* -------- t3 : HMAC(Q||R||ts) -------- */
    char msg[128];
    snprintf(msg, sizeof(msg), "%llu%llu%llu",
             (unsigned long long)Q,
             (unsigned long long)R,
             (unsigned long long)ts);

    QueryPerformanceCounter(&t1);
    HMAC(EVP_sha256(), secret, strlen(secret),
         (unsigned char*)msg, strlen(msg),
         NULL, &hmac_len);
    QueryPerformanceCounter(&t2);
    double t_hmac = diff_ms(t1, t2, freq);

    printf("SHA256 time      : %.3f ms\n", t_sha);
    printf("Division time    : %.3f ms\n", t_div);
    printf("HMAC time        : %.3f ms\n", t_hmac);
    printf("-------------------------------\n");
    printf("Total Crypto Time: %.3f ms\n",
           t_sha + t_div + t_hmac);

    return 0;
}