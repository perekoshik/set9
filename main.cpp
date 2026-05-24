#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "sorts.h"
#include "string_generator.h"
#include "string_sort_tester.h"

struct AlgoEntry {
    std::string name;
    StringSortTester::SortFn fn;
};

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(false);

    int n_min = 100;
    int n_max = 3000;
    int n_step = 100;
    int repeats = 3;
    std::string out_path = "results.csv";

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--n-max" && i + 1 < argc) n_max = std::stoi(argv[++i]);
        else if (a == "--n-min" && i + 1 < argc) n_min = std::stoi(argv[++i]);
        else if (a == "--n-step" && i + 1 < argc) n_step = std::stoi(argv[++i]);
        else if (a == "--repeats" && i + 1 < argc) repeats = std::stoi(argv[++i]);
        else if (a == "--out" && i + 1 < argc) out_path = argv[++i];
    }

    std::vector<AlgoEntry> algos = {
        {"quickSort", [](std::vector<std::string> &v){ if (!v.empty()) quickSort(v, 0, (int)v.size() - 1); }},
        {"mergeSort", [](std::vector<std::string> &v){ if (!v.empty()) mergeSort(v, 0, v.size() - 1); }},
        {"TernaryQuickSort", [](std::vector<std::string> &v){ v = TernaryQuickSort(std::move(v), 0); }},
        {"StringMergeSort", [](std::vector<std::string> &v){ v = StringMergeSort(std::move(v)); }},
        {"MSDRadixSort", [](std::vector<std::string> &v){ v = MSDRadixSort(std::move(v), 0); }},
        {"MSBRadixSort", [](std::vector<std::string> &v){ v = MSBRadixSort(std::move(v), 0, 74); }},
    };

    std::vector<std::string> types = {"random", "reverse", "almost"};

    StringSortTester tester(repeats);
    std::ofstream out(out_path);

    out << "algorithm,array_type,n,avg_time_us,avg_char_comparisons\n";

    int total = 0, done = 0;
    for (int n = n_min; n <= n_max; n += n_step) ++total;
    total *= (int)types.size() * (int)algos.size();

    StringGenerator gen(42);
    for (int n = n_min; n <= n_max; n += n_step) {
        std::vector<std::string> random_arr  = gen.random(n);
        std::vector<std::string> reverse_arr = gen.reverse_sorted(n);
        std::vector<std::string> almost_arr  = gen.almost_sorted(n);

        for (const auto& type : types) {
            const std::vector<std::string>* data = nullptr;
            if (type == "random")  data = &random_arr;
            if (type == "reverse") data = &reverse_arr;
            if (type == "almost")  data = &almost_arr;

            for (const auto& a : algos) {
                TestResult r = tester.run(a.name, a.fn, type, *data);
                out << r.algorithm << "," << r.array_type << "," << r.n << ","
                    << r.avg_time_us << "," << r.avg_comparisons << "\n";
                ++done;
                std::fprintf(stderr, "[%d/%d] n=%d type=%s algo=%-18s time=%.1fus cmp=%.0f\n",
                             done, total, r.n, r.array_type.c_str(), r.algorithm.c_str(),
                             r.avg_time_us, r.avg_comparisons);
            }
        }
        out.flush();
    }

    return 0;
}
