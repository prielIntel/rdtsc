#include <stdio.h>
#include <stdlib.h>
#include <papi.h>

int main() {
    int retval;
    long long cycles = 0;
    volatile int dummy = 0; // Prevent compiler optimization

    // Initialize the PAPI library
    if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT) {
        fprintf(stderr, "PAPI library initialization error: %d\n", retval);
        exit(1);
    }

    // Start counting cycles
    if (PAPI_start_counters((int[]){PAPI_TOT_CYC}, 1) != PAPI_OK) {
        fprintf(stderr, "Failed to start PAPI counters.\n");
        exit(1);
    }

    // Target assembly instruction or code to measure
    __asm__ volatile (
        "addl $1, %[dummy]\n\t" // Example instruction
        : [dummy] "+r"(dummy)    // Output constraint to prevent optimization
    );

    // Stop counters and read the cycles
    if (PAPI_stop_counters(&cycles, 1) != PAPI_OK) {
        fprintf(stderr, "Failed to stop PAPI counters.\n");
        exit(1);
    }

    printf("Cycles taken: %lld\n", cycles);

    return 0;
}
