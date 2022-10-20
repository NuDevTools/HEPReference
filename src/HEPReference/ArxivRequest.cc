#include "HEPReference/ArxivRequest.hh"
#include <regex>

bool HEPReference::ArxivRequest::ValidateID(const std::string &id) const {
    static std::regex valid(R"(\d{4}\.\d+|(?:hep-(?:ph|th|ex)|nucl-(?:th|ex))\/\d+)");
    return std::regex_search(id, valid);
}
