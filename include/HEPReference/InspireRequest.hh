#ifndef HEPREFERNCE_INSPIRE_REQUEST
#define HEPREFERNCE_INSPIRE_REQUEST

#include "HEPReference/Request.hh"
#include "HEPReference/ArxivRegex.hh"
#include "fmt/format.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#include "range/v3/all.hpp"
#include "spdmon/spdmon.hh"
#pragma GCC diagnostic pop

#include <set>
#include <string>
#include <string_view>
#include <regex>

namespace HEPReference {

enum class InspireIdentifier {
    uid,
    literature,
    authors,
    conferences,
    seminars,
    journals,
    jobs,
    experiments,
    data,
    // External sources
    doi = 101,
    arxiv = 102,
    orcid = 103
};

enum class InspireFormat {
    json,
    bibtex,
    latex_eu,
    latex_us,
    cv
};

class InspireRequest : Request {
    public:
        InspireRequest() {
            SetOption(cpr::VerifySsl{false});
        }

        cpr::Response Search(InspireIdentifier id, const std::string &query) { 
            if(static_cast<int>(id) > 100) {
                return cpr::Response{};
            }
            std::string tail = ConvertIdentifier(id) + "?q=" + query;
            std::string url = fmt::format(base_url, tail);
            spdlog::trace("Request url: {}", url);
            return Get(url);
        }

        std::string GenerateCitations(const std::string &articles, InspireFormat format=InspireFormat::bibtex) {
            return GenerateCitations({articles}, format);
        }

        std::string GenerateCitations(const std::vector<std::string> &varticles, InspireFormat format=InspireFormat::bibtex) {
            auto split_strs = [](auto &&pattern) {
                return ranges::views::split(pattern) | ranges::views::transform([](auto p) {
                        auto c = p | ranges::views::common;
                        auto tmp = std::string(c.begin(), c.end());
                        tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
                        return tmp;
                        });
            };
            std::set<std::string> unique_articles;
            for(auto articles : varticles) {
                auto article = articles | split_strs(',');
                unique_articles.insert(article.begin(), article.end());
            }
            spdlog::info("Found {} unique articles", unique_articles.size());
            std::string result = "";
            for(const auto &[logger, article] : spdmon::LogProgress(unique_articles)) {
                try {
                    result += BibtexEntry(article, format);
                } catch(std::runtime_error &e) {
                    spdlog::error("{}", e.what());
                }
            }
            return result;
        }

        std::string GenerateCitationsFromTex(const std::string &texfile, InspireFormat format=InspireFormat::bibtex) {
            auto contents = FileContents(texfile);
            static std::regex citation(R"((?:\\cite\{([^\}]+)\}))");
            spdlog::info("input = {}", texfile);
            std::vector<std::string> matches(std::sregex_token_iterator(
                        contents.begin(), contents.end(), citation, 1),
                    std::sregex_token_iterator());
            return GenerateCitations(matches, format);
        }

        std::string GenerateCV(const std::string &author,[[maybe_unused]] InspireFormat format=InspireFormat::cv) {
            std::string tail = "literature?q=a%20" + author ;
            std::string url = fmt::format(base_url, tail);
            spdlog::info("Request url: {}", url);
            // SetOption(cpr::Header{{"accept", "text/vnd+inspire.html+html"}});
            SetOption(cpr::Header{{"accept", "application/json"}});
            return Get(url).text;
        }

        std::string BibtexEntry(const std::string &query, InspireFormat format=InspireFormat::bibtex) {
            static std::regex doi_id(R"(^10.\d{4,9}/[-._;()/:A-Za-z0-9]+$)");
            static std::regex inspire_id(R"([-A-Za-z]+:\d{4}[a-z]{2,3})");

            InspireIdentifier id;
            if(HepReference::arxiv::Validate(query)) id = InspireIdentifier::arxiv;
            else if(std::regex_search(query, doi_id)) id = InspireIdentifier::doi;
            else if(std::regex_search(query, inspire_id)) id = InspireIdentifier::uid;
            else {
                throw std::runtime_error(fmt::format("HEPReference::InspireRequest: Invalid ID {}", query));
            }

            spdlog::debug("Getting bibtex entry for {} with id {}", query, ConvertIdentifier(id));
            return SingleRecord(id, query, format).text;
        }

    private:
        std::string FileContents(const std::string &filename) const {
            std::ifstream in(filename, std::ios::in | std::ios::binary);
            if(!in) {
                spdlog::error("InspireRequest::FileContents: Could not open file: {}",
                              filename);
                return "";
            }
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(static_cast<size_t>(in.tellg()));
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], static_cast<long>(contents.size()));
            in.close();
            return contents;
        }

        cpr::Response SingleRecord(InspireIdentifier id, const std::string &query, InspireFormat format) {
            std::string tail;
            if(id == InspireIdentifier::uid) {
                tail = ConvertIdentifier(id) + "?" + FormatToString(format) + "&q=texkey:" + query;
            } else {
                tail = ConvertIdentifier(id) + query + "?" + FormatToString(format);
            }
            std::string url = fmt::format(base_url, tail);
            return Get(url);
        }

        std::string ConvertIdentifier(InspireIdentifier id) {
            switch(id) {
                case InspireIdentifier::uid:
                    return "literature/";
                case InspireIdentifier::literature:
                    return "literature/";
                case InspireIdentifier::authors:
                    return "authors/";
                case InspireIdentifier::conferences:
                    return "conferences/";
                case InspireIdentifier::seminars:
                    return "seminars/";
                case InspireIdentifier::journals:
                    return "journals/";
                case InspireIdentifier::jobs:
                    return "jobs/";
                case InspireIdentifier::experiments:
                    return "experiments/";
                case InspireIdentifier::data:
                    return "data/";
                case InspireIdentifier::doi:
                    return "doi/";
                case InspireIdentifier::arxiv:
                    return "arxiv/";
                case InspireIdentifier::orcid:
                    return "orcid/";
            }
            return "Unknown";
        }
        std::string FormatToString(InspireFormat format) {
            switch(format) {
                case InspireFormat::json:
                    return "format=json";
                case InspireFormat::bibtex:
                    return "format=bibtex";
                case InspireFormat::latex_eu:
                    return "format=latex-eu";
                case InspireFormat::latex_us:
                    return "format=latex-us";
                case InspireFormat::cv:
                    return "format=cv";
            };
            return "Unknown";
        }
        constexpr static std::string_view base_url = "https://inspirehep.net/api/{}";
};

}

#endif
