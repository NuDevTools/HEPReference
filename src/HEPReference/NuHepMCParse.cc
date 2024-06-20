#include "HEPReference/NuHepMC.hh"

#include <iostream>

int main(const int argc, const char *argv[]) {
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    HEPReference::NuHepMCParser parser{argv[1]};
    std::cout << parser.Bibliography(HEPReference::InspireFormat::bibtex) << std::endl;
    std::cout << parser.TextBlurb() << std::endl;
}
