#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    std::string url = "https://inspirehep.net/api/arxiv/2110.15319?format=bibtex";
    cpr::Response r = cpr::Get(cpr::Url{url},
                               cpr::VerifySsl{false});
    std::cout << r.status_code << std::endl;
    std::cout << r.text << std::endl;
}
