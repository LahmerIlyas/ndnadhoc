//
// Created by lahmer on 31/07/17.
//

#include "cit.h"

uint32_t cit::getLimit() const {
    return m_limit;
}

void cit::setLimit(uint32_t limit) {
    m_limit = limit;
}

void cit::insert(const ndn::Interest &interest) {

    if(m_entries.find(interest.getName()) != m_entries.end())
        return;

    citEntry entry(interest);
    m_entries.insert(std::make_pair<>(interest.getName(),entry));

}

cit::cit() {

}

unsigned long cit::size() {
    return m_entries.size();
}

void cit::remove(const ndn::Interest &interest) {
    m_entries.erase(interest.getName());
}

void cit::remove(const ndn::Data &data) {
    m_entries.erase(data.getName());
}
