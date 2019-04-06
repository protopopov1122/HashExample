#include "hash.h"
#include <cmath>

#ifndef USE_FNV

static constexpr std::size_t PRIME_COUNT = 256;
static bool PrimesInitialised = false;
static uint64_t PRIMES[PRIME_COUNT];

static bool IsPrime(uint64_t number) {
	uint64_t sq_root = static_cast<uint64_t>(ceil(sqrt(number)));
	for (uint64_t i = 2; i <= sq_root; i++) {
		if (number % i == 0) {
			return false;
		}
	}
	return true;
}

void InitPrimes() {
	constexpr uint64_t FIRST_PRIME = 19;
	uint64_t number = FIRST_PRIME;
	std::size_t counter = 0;
	while (counter < PRIME_COUNT) {
		if (IsPrime(number)) {
			PRIMES[counter++] = number;
		}
		number++;
	}
	PrimesInitialised = true;
}

#endif

uint64_t HashString(const std::string &str) {
#ifndef USE_FNV
	if (!PrimesInitialised) {
		InitPrimes();
	}
	constexpr uint64_t PRIME_FACTOR = 17;
	uint64_t acc = 0;
	std::size_t len = 0;
	for (const unsigned char chr : str) {
		acc += chr;
		acc *= PRIMES[(++len) % PRIME_COUNT];
	}
	return acc * PRIME_FACTOR;
#else
	uint64_t hash = 0xcbf29ce484222325;
	for (const unsigned char chr : str) {
		hash *= 1099511628211;
		hash ^= chr;
	}
	return hash;
#endif
}
