#ifndef ARXIV_REGEX
#define ARXIV_REGEX

#include "fmt/format.h"
#include <array>
#include <regex>
#include <string_view>

namespace HepReference {

namespace arxiv {

static constexpr std::array<std::string_view, 38> categories {
    "acc-phys", "adap-org", "alg-geom", "ao-sci",   "astro-ph",
    "atom-ph",  "bayes-an", "chao-dyn", "chem-ph",  "cmp-lg", 
    "comp-gas", "cond-mat", "cs",       "dg-ga",    "econ",
    "eess",     "funct-an", "gr-qc",    "hep-ex",   "hep-lat",
    "hep-ph",   "hep-th",   "math",     "math-ph",  "mtrl-th",
    "nlin",     "nucl-ex",  "nucl-th",  "patt-sol", "physics",
    "plasm-ph", "q-alg",    "q-bio",    "q-fin",    "quant-ph",
    "solv-int", "stat",     "supr-con"
};

inline bool Validate(const std::string &id) {
    static const std::string all_categories = fmt::format("{}", fmt::join(categories.begin(),
                                                                          categories.end(), "|"));
    static std::regex valid_reg(R"(^(ar[xX]iv:)?((?:(?:(?:)" 
                                + all_categories 
                                + R"()(?:[.][A-Z]{2})?/[0-9]{2}(?:0[1-9]|1[0-2])\d{3}(?:[vV]\d+)?))|(?:(?:[0-9]{2}(?:0[1-9]|1[0-2])[.]\d{4,5}(?:[vV]\d+)?)))$)");
    return std::regex_search(id, valid_reg);
}

}

}

#endif
