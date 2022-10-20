#pragma once

#include <map>
#include <iostream>

#include "HEPReference/Reference.hh"

using json = nlohmann::json;

namespace HEPReference {

class Library;

void to_json(json&, const Library&);
void from_json(const json&, Library&);

class Library {
    public:
        Library() = default;

        static Library& Global() {
            static Library library;
            return library;
        }

        template<typename OStream>
        friend OStream& operator<<(OStream &os, const Library &lib) {
            for(const auto &entry : lib.m_contents) {
                os << entry.second << "\n";
            }
        }

        Reference Lookup(const std::string &id) const;
        template<typename OStream>
        bool WriteDescription(OStream &os) const {
            for(const auto &entry : m_contents) {
                os << entry.second.Description();
            }
            return true;
        }
        template<typename OStream>
        bool WriteBibtex(OStream &os) const {
            os << *this;
            return true;
        }
        bool AddReference(const Reference &ref) {
            if(m_contents.find(ref.Id()) != m_contents.end()) {
                std::string msg = fmt::format("HEPReference: entry {} is already defined", ref.Id());
                std::cerr << msg << std::endl;
                return false;
            }
            m_contents[ref.Id()] = ref;
            return true;
        }

        friend void to_json(json&, const Library&);
        friend void from_json(const json&, Library&);

    private:
        std::map<std::string, Reference> m_contents;
};

}
