#pragma once

#include "HEPReference/InspireRequest.hh"

namespace HEPReference {

struct Generator {
    std::string name;
    std::string version;
    std::string description;
};

struct NuHepMCEntry {
    std::string record_id;
    std::string description;
    std::vector<std::string> keys;
};

class NuHepMCParser {
    public:
        NuHepMCParser(const std::string &filename);
        std::string Bibliography(InspireFormat);
        std::string TextBlurb();

    private:
        void ExtractEntries();
        int ExtractProcessID(const std::string &entry) const;
        std::string ExtractInspireHEPKey(const std::string &entry) const;

        InspireRequest m_request;
        std::string m_filename;
        Generator m_generator;
        std::set<std::string> m_entries;
        std::map<std::string, std::vector<std::string>> m_entries_map;
        std::map<int, std::pair<std::string, std::string>> m_descriptions;
        std::map<std::string, std::string> m_key_map;
};

}
