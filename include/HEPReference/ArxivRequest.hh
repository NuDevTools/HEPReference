#ifndef HEPREFERENCE_ARXIV
#define HEPREFERENCE_ARXIV

#include "HEPReference/Request.hh"
#include "fmt/format.h"
#include "pugixml.hpp"

#include <string>
#include <string_view>

namespace HEPReference {

class ArxivRequest : Request {
    public:
        ArxivRequest();

    private:
        bool ValidateID(const std::string &id) const;

};

}

#endif
