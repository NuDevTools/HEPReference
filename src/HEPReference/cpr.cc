#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "HEPReference/InspireRequest.hh"

int main() {
    HEPReference::InspireRequest inspire;
    // auto response = inspire.Search(HEPReference::InspireIdentifier::authors, "j.isaacson");
    // std::cout << response.text << std::endl;
    // auto entry = inspire.BibtexEntry("2110.15319", HEPReference::InspireFormat::json);
    // std::cout << nlohmann::json::parse(entry).at("metadata").at("documents") << std::endl;
    // entry = inspire.BibtexEntry("10.1103/PhysRevD.105.096006");
    // std::cout << entry << std::endl;
    // entry = inspire.BibtexEntry("Isaacson:2021xty");
    // std::cout << entry << std::endl;

    // auto bib = inspire.GenerateCitationsFromTex("1_intro.tex");
    // std::cout << bib << std::endl;
    
    auto cv = inspire.GenerateCV("Joshua.Isaacson.1");
    std::cout << cv << std::endl;

    // response = cpr::Get(cpr::Url{"http://export.arxiv.org/api/query?search_query=co:scipost&sortBy=lastUpdatedDate&sortOrder=descending&max_results=50"});
    // std::cout << response.text << std::endl;
    // std::cout << response.header["content-type"] << std::endl;
    // response = cpr::Get(cpr::Url{"https://inspirehep.net/api/literature/4328?format=bibtex"});
    // std::cout << response.text << std::endl;
}
