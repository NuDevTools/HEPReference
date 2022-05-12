#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "fmt/format.h"
#include "yaml-cpp/yaml.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace HEPReference {

class Reference;

void to_json(json&, const Reference&);
void from_json(const json&, Reference&);

enum class RefType {
    Unknown = -1,
    Article,
    Book,
    Conference,
    Inproceedings,
    Manual,
};

inline std::string RefTypeToString(const RefType &type) {
    std::string result;
    switch(type) {
        case RefType::Unknown:
            throw std::runtime_error("Unknown reference type");
        case RefType::Article:
            result = "article";
            break;
        case RefType::Book:
            result = "book";
            break;
        case RefType::Conference:
        case RefType::Inproceedings:
            result = "conference";
            break;
        case RefType::Manual:
            result = "manual";
            break;
    }
    return result;
}

inline RefType StringToRefType(const std::string &name) {
    if(name == "article")
        return RefType::Article;
    else if(name == "book")
        return RefType::Book;
    else if(name == "conference")
        return RefType::Conference;
    else if(name == "manual")
        return RefType::Manual;
    else 
        return RefType::Unknown;
}

inline void to_json(json &j, const RefType &type) {
    j = json{"type", RefTypeToString(type)};
}

inline void from_json(const json &j, RefType &type) {
    auto name = j.at("type").get<std::string>();
    type = StringToRefType(name);
}


class Reference {
    public:
        Reference() = default;
        Reference(std::string description) : m_description{description} {}

        template<typename OStream>
        friend OStream& operator<<(OStream &os, const Reference &ref) {
            const std::string in = "    ";
            os << fmt::format("@{}{{{},\n", RefTypeToString(ref.m_type), ref.m_id);
            os << in << fmt::format("author = {},\n", ref.m_authors);
            os << in << fmt::format("title = {},\n", ref.m_title);
            if(ref.m_eprint != "N/A") {
                os << in << fmt::format("eprint = {},\n", ref.m_eprint);
                os << in << fmt::format("archivePrefix = \"arXiv\",\n");
                os << in << fmt::format("primaryClass = {},\n", ref.m_class);
            }
            if(ref.m_doi != "N/A") {
                os << in << fmt::format("doi = {},\n", ref.m_doi);
                os << in << fmt::format("journal = {},\n", ref.m_journal);
                os << in << fmt::format("volume = {},\n", ref.m_volume);
                os << in << fmt::format("number = {},\n", ref.m_number);
                os << in << fmt::format("pages = {},\n", ref.m_pages);
            }
            if(ref.m_month != 0)
                os << in << fmt::format("month = {},\n", ref.m_month);
            os << in << fmt::format("year = {},\n", ref.m_year);
            return os;
        }

        const std::string& Description() const { return m_description; }
        std::string& Description() { return m_description; }

        friend struct YAML::convert<HEPReference::Reference>;
        friend void to_json(json&, const Reference&);
        friend void from_json(const json&, Reference&);
        std::string Id() const { return m_id; }
        
    private:
        std::string m_id{}, m_description{};
        RefType m_type{RefType::Unknown};
        std::vector<std::string> m_authors{};
        std::string m_title{};
        std::string m_eprint{"N/A"};
        std::string m_class{};
        std::string m_doi{"N/A"};
        std::string m_journal{};
        std::string m_volume{};
        std::string m_number{};
        std::string m_pages{};
        int m_month{}, m_year{};
};

}

namespace YAML {

template<>
struct convert<HEPReference::Reference> {
    static Node encode(const HEPReference::Reference &ref) {
        Node node;
        node["id"] = ref.m_id;
        node["type"] = HEPReference::RefTypeToString(ref.m_type);
        node["authors"] = ref.m_authors;
        node["title"] = ref.m_title;
        node["eprint"] = ref.m_eprint;
        if(ref.m_eprint != "N/A")
            node["class"] = ref.m_class;
        node["doi"] = ref.m_doi;
        if(ref.m_doi != "N/A") {
            node["journal"] = ref.m_journal;
            node["volume"] = ref.m_volume;
            node["number"] = ref.m_number;
            node["pages"] = ref.m_pages;
        }
        node["month"] = ref.m_month;
        node["year"] = ref.m_year;
        return node;
    }

    static bool decode(const Node &node, HEPReference::Reference &ref) {
        ref.m_id = node["id"].as<std::string>();
        ref.m_type = HEPReference::StringToRefType(node["type"].as<std::string>());
        ref.m_authors = node["authors"].as<std::vector<std::string>>();
        ref.m_title = node["title"].as<std::string>();
        ref.m_eprint = node["eprint"].as<std::string>();
        if(ref.m_eprint != "N/A")
            ref.m_class = node["class"].as<std::string>();
        ref.m_doi = node["doi"].as<std::string>();
        if(ref.m_doi != "N/A") {
            ref.m_journal = node["journal"].as<std::string>();
            ref.m_volume = node["volume"].as<std::string>();
            ref.m_number = node["number"].as<std::string>();
            ref.m_pages = node["pages"].as<std::string>();
        }
        ref.m_month = node["month"].as<int>();
        ref.m_year = node["year"].as<int>();
        return true;
    }
};

}
