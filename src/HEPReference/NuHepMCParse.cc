#include "HEPReference/NuHepMC.hh"

#include <iostream>
#include <fstream>

int main(const int argc, const char *argv[]) {
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    HEPReference::NuHepMCParser parser{argv[1]};
    std::ofstream bibtex{"output.bib"}; 
    bibtex << parser.Bibliography(HEPReference::InspireFormat::bibtex) << std::endl;
    bibtex.close();
    std::ofstream text_blurb{"output.tex"}; 
    text_blurb << parser.TextBlurb() << std::endl;
    text_blurb.close();

    std::cout << "Bibliography written to output.bib" << std::endl;
    std::cout << "Text blurb written to output.tex" << std::endl;

    return 0;
}
