#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <random>
#include <string>
#include <vector>

class StringGenerator {
public:
    StringGenerator(unsigned int seed = 42);

    std::vector<std::string> random(int n, int min_len = 10, int max_len = 200);
    std::vector<std::string> reverse_sorted(int n, int min_len = 10, int max_len = 200);
    std::vector<std::string> almost_sorted(int n, int min_len = 10, int max_len = 200, int swaps = 0);

    static const std::string& alphabet();

private:
    std::mt19937 rng_;

    std::string random_string(int min_len, int max_len);
};

#endif
