//
// Created by lahmer on 01/08/17.
//

#include "cdt.h"

cdt::cdt(double size){
    m_limitSize = size;

    m_entries.reserve(1024*100);
}

cdtIterator cdt::begin() {
    return m_entries.begin();
}

cdtIterator cdt::end() {
    return m_entries.end();
}

void cdt::insert(const ndn::Data &data) {
    if(m_entries.find(data.getName()) != m_entries.end()){
        return;
    }
    cdtEntry entry(data);
    m_entries.insert(std::make_pair(data.getName(),entry));

    m_tableSizeInKo += data.getContent().size();
    if(m_tableSizeInKo >= m_limitSize){
        onFullBuffer();
        return;
    }

}

void cdt::remove(const ndn::Data &data) {
    m_entries.erase(data.getName());
    m_tableSizeInKo -= data.getContent().size();
}

double cdt::size() {
    return m_entries.size();
}

void cdt::setKoLimitSize(double limitSize) {
    m_limitSize = limitSize;
}

bool cdt::isHere(const ndn::Name name ) {
    return m_entries.find(name) != m_entries.end();
}

cdtIterator cdt::getEntryByName(const ndn::Name &name) {
    auto it = m_entries.find(name);
    return it;
}

void cdt::remove(const ndn::Name &name) {
    if(m_entries.find(name) != m_entries.end()){
        m_tableSizeInKo -= m_entries[name].getData()->getContent().size();
    }
    m_entries.erase(name);
}

void cdt::removeOldest() {
    auto it = std::max_element(begin(),end(),[](const std::pair<ndn::Name,cdtEntry>& a, const std::pair<ndn::Name,cdtEntry>& b){
        return a.second.getReveivedAt() < b.second.getReveivedAt();
    });
    if(it != m_entries.end()){
        m_tableSizeInKo -= it->second.getData()->getContent().size();
        m_entries.erase(it);
    }else{
        std::cout<<" Error while removing the oldest entry in the cdt table "<<std::endl;
        std::terminate();
    }
}

double cdt::getSizeKo() {
    return m_tableSizeInKo;
}

void cdt::logBufferSize() {
    double time = ns3::Simulator::Now().GetSeconds();
    double bufferSize = getSizeKo();
    m_bufferSizeEvolution.push_back(std::make_pair(time, bufferSize));
}
