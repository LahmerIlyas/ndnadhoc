//
// Created by lahmer on 31/07/17.
//

#include "cit.h"


cit::cit(NameTree &tree): m_nt(tree) {

}



void cit::insert(const ndn::Interest &interest) {
    auto entry = m_nt.lookupEntry(interest.getName());
    if(entry == nullptr){
        entry = m_nt.insertEntry(interest.getName());
    }
    //we check if there is a cdtEntry
    if(entry->hasCDTEntry())
        return;

    //we create a new citEntry
    auto newCITEntry = std::make_shared<citEntry>(citEntry(interest));
    entry->addCITEntry(newCITEntry);
}

void cit::remove(const ndn::Interest &interest) {
    auto entry = m_nt.lookupEntry(interest.getName());
    if(entry == nullptr){
        return;
    }
    entry->eraseCITEntry();
}

void cit::remove(const ndn::Data &data) {
    auto entry = m_nt.lookupEntry(data.getName());
    if(entry == nullptr){
        return;
    }
    entry->eraseCITEntry();

}

bool cit::contains(const ndn::Name &name) {
    auto entry = m_nt.lookupEntry(name);
    if(entry == nullptr)
        return false;

    return entry->hasCITEntry();
}

void cit::iterateOverElements(std::function<void(citEntry &entry)> func) {
    for(auto& it : m_nt){
        if(it.second->hasCITEntry()){
            func(it.second->getCITEntry());
        }
    }
}

