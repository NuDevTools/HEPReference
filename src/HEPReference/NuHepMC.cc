#include "HEPReference/NuHepMC.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#include "HepMC3/ReaderFactory.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Print.h"
#include "NuHepMC/ReaderUtils.hxx"
#pragma GCC diagnostic pop

#include "range/v3/all.hpp"
#include "range/v3/range/conversion.hpp"

#include <iostream>

using HEPReference::NuHepMCParser;
using HEPReference::InspireFormat;

NuHepMCParser::NuHepMCParser(const std::string &filename) : m_filename{filename} {
    ExtractEntries();
}

void NuHepMCParser::ExtractEntries() {
    auto reader = HepMC3::deduce_reader(m_filename);
    if(!reader) {
        throw std::runtime_error("Could not deduce reader for file: " + m_filename);
    }

    HepMC3::GenEvent evt;
    reader->read_event(evt);
    if(reader->failed()) {
        throw std::runtime_error("Could not read event from file: " + m_filename);
    }

    auto run_info = reader->run_info();
    if(!run_info) {
        throw std::runtime_error("Could not get run info from file: " + m_filename);
    }

    HepMC3::Print::listing(std::cout, (*run_info));

    if(!NuHepMC::GC1::SignalsConvention(run_info, "G.C.6")) {
        throw std::runtime_error("File does not contain citations flag");
    }

    auto tools = run_info->tools();
    m_generator = {tools[0].name, tools[0].version, tools[0].description};

    auto citations = NuHepMC::GC6::ReadAllCitations(run_info);

    for(const auto &citation : citations) {
        std::string identifier = citation.first;
        for(const auto &line : citation.second) {
            auto split_entries = line.second | ranges::views::split(' ') | ranges::to<std::vector<std::string>>();
            for(const auto &entry : split_entries) {
                m_entries_map[identifier].push_back(entry);
                m_entries.insert(entry);
            }
        } 
    }

    m_descriptions = NuHepMC::GR4::ReadProcessIdDefinitions(run_info);

    std::cout << "Found " << m_entries.size() << " citations" << std::endl;
}

int NuHepMCParser::ExtractProcessID(const std::string &entry) const {
    int process_id = -1;
    static std::regex process_id_regex(R"(Process\[(\d+)\])");
    std::smatch match;
    if(std::regex_search(entry, match, process_id_regex)) {
        process_id = std::stoi(match[1]);
    } else {
        throw std::runtime_error("Could not extract process_id from entry: " + entry);
    }

    return process_id;
}

std::string NuHepMCParser::ExtractInspireHEPKey(const std::string &entry) const {
    std::string key;
    static std::regex key_regex(R"(@\w+\{([^,]+),)");
    std::smatch match;
    if(std::regex_search(entry, match, key_regex)) {
        key = match[1];
    } else {
        throw std::runtime_error("Could not extract key from entry: " + entry);
    }

    return key;
}

std::string NuHepMCParser::Bibliography(InspireFormat format) {
    std::string bibliography;
    for(const auto &entry : m_entries) {
        auto response = m_request.BibtexEntry(entry, format);
        auto key = ExtractInspireHEPKey(response);
        m_key_map[entry] = key;

        bibliography += response;
    }
    return bibliography;
}

std::string NuHepMCParser::TextBlurb() {
    std::string blurb;
    for(const auto &entry : m_entries_map) {
        if(entry.first == "Generator") {
            blurb += "Events were generated using " + m_generator.name + " version " + m_generator.version + ".";
        } else {
            try {
                int process_id = ExtractProcessID(entry.first);
                auto description = m_descriptions.at(process_id);
                blurb += "The " + description.first + " process is defined as " + description.second + ".";
            } catch(const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
        blurb += "~\\cite{";
        blurb += entry.second | ranges::views::transform([&](const auto &e) { return m_key_map.at(e); })
            | ranges::views::join(',') | ranges::to<std::string>();
        blurb += "}\n";
    }
    return blurb;
}
