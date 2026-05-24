#include "string_generator.h"

#include <algorithm>

static const std::string ALPHABET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-.";

const std::string& StringGenerator::alphabet() {
    return ALPHABET;
}

StringGenerator::StringGenerator(unsigned int seed) : rng_(seed) {}

std::string StringGenerator::random_string(int min_len, int max_len) {
    std::uniform_int_distribution<int> len_dist(min_len, max_len);
    std::uniform_int_distribution<int> char_dist(0, (int)ALPHABET.size() - 1);
    int len = len_dist(rng_);
    std::string s(len, ' ');
    for (int i = 0; i < len; ++i) {
        s[i] = ALPHABET[char_dist(rng_)];
    }
    return s;
}

std::vector<std::string> StringGenerator::random(int n, int min_len, int max_len) {
    std::vector<std::string> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = random_string(min_len, max_len);
    }
    return result;
}

std::vector<std::string> StringGenerator::reverse_sorted(int n, int min_len, int max_len) {
    auto result = random(n, min_len, max_len);
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    return result;
}

std::vector<std::string> StringGenerator::almost_sorted(int n, int min_len, int max_len, int swaps) {
    auto result = random(n, min_len, max_len);
    std::sort(result.begin(), result.end());
    if (swaps <= 0) {
        swaps = std::max(1, n / 50);
    }
    std::uniform_int_distribution<int> idx_dist(0, n - 1);
    for (int i = 0; i < swaps; ++i) {
        int a = idx_dist(rng_);
        int b = idx_dist(rng_);
        std::swap(result[a], result[b]);
    }
    return result;
}
