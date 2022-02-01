#include "HEPReference/Reference.hh"

void HEPReference::to_json(json &j, const Reference &ref) {
    j = json{{"id", ref.m_id},
             {"type", ref.m_type},
             {"authors", ref.m_authors},
             {"eprint", ref.m_eprint},
             {"class", ref.m_class},
             {"doi", ref.m_doi},
             {"journal", ref.m_journal},
             {"volume", ref.m_volume},
             {"number", ref.m_number},
             {"pages", ref.m_pages},
             {"month", ref.m_month},
             {"year", ref.m_year}};
}

void HEPReference::from_json(const json &j, Reference &ref) {
    j.at("id").get_to(ref.m_id);
    j.at("type").get_to(ref.m_type);
    j.at("authors").get_to(ref.m_authors);
    j.at("title").get_to(ref.m_title);
    j.at("eprint").get_to(ref.m_eprint);
    j.at("class").get_to(ref.m_class);
    j.at("doi").get_to(ref.m_doi);
    j.at("journal").get_to(ref.m_journal);
    j.at("volume").get_to(ref.m_volume);
    j.at("number").get_to(ref.m_number);
    j.at("pages").get_to(ref.m_pages);
    j.at("month").get_to(ref.m_month);
    j.at("year").get_to(ref.m_year);
}
