#include "HEPReference/Library.hh"

using HEPReference::Reference;
using HEPReference::Library;

void HEPReference::to_json(json &j, const Library &lib) {
    for(const auto &entry : lib.m_contents) {
        to_json(j, entry.second);
    }
}

void HEPReference::from_json(const json &j, Library &lib) {

}

Reference Library::Lookup(const std::string &id) const {
    if(m_contents.find(id) == m_contents.end()) {
        std::string msg = fmt::format("HEPReference: Unknown reference {}", id);
        std::cerr << msg << std::endl;
        Reference ref;
        return ref;
    }

    return m_contents.at(id);
}
