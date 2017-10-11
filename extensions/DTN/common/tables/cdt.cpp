//
// Created by lahmer on 01/08/17.
//

#include "cdt.h"



cdt::cdt(NameTree &nt, double size) : m_nt(nt){

}


void cdt::insert(const ndn::Data &data) {
    //we lookup to see if there is an entry
    auto entry = m_nt.lookupEntry(data.getName());
    if(entry == nullptr){
        entry = m_nt.insertEntry(data.getName());
    }
    //if there is cit entry we delete it
    if(entry->hasCITEntry()){
        entry->eraseCITEntry();
    }
    //we check if there is cdt Entry
    if(entry->hasCDTEntry())
        return;

    //we create a new cdtEntry
    auto newEntry = std::make_shared<cdtEntry>(cdtEntry(data));
    entry->addCDTEntry(newEntry);
}

void cdt::remove(const ndn::Data &data) {
    auto entry = m_nt.lookupEntry(data.getName());
    if(entry == nullptr)
        return;

    entry->eraseCDTEntry();
}


bool cdt::isHere(const ndn::Name name) {
    auto entry = m_nt.lookupEntry(name);
    if(entry == nullptr)
        return false;
    return entry->hasCDTEntry();
}

void cdt::iterateOverElements(std::function<void(cdtEntry &entry)> func) {
    for(auto& it : m_nt){
        if(it.second->hasCDTEntry()){
            func(it.second->getCDTEntry());
        }
    }
}

