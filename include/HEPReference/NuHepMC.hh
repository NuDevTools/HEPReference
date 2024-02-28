#pragma once

#include "HEPReference/InspireRequest.hh"

namespace HEPReference {

class NuHepMCParser {
    public:
        NuHepMCParser(const std::string &filename);
        std::string Bibliography(InspireFormat) const;
        std::string TextBlurb() const;

    private:

};

}
