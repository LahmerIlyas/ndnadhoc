//
// Created by lahmer on 02/09/17.
//

#include <numeric>
#include "NodeContactTable.h"

std::vector<std::tuple<double,uint64_t, uint64_t, uint64_t, uint64_t, uint64_t >> NodeContactTable::m_contactVector;

void NodeContactTable::addContact(const nfd::Face &face) {
    std::string remoteEndPoint = face.getRemoteUri().toString();
    //check if this is already known face
    if(m_entries.find(remoteEndPoint) != m_entries.end()){
        m_entries.at(remoteEndPoint).addContact(face);
    }else{
        //we create a new entry
        NodeContactTableEntry entry(face);
        m_entries.insert(std::make_pair(remoteEndPoint,entry));
        m_entries.at(remoteEndPoint).addContact(face);
    }
}

void NodeContactTable::endContact(const nfd::Face &face) {
    std::string remoteEndPoint = face.getRemoteUri().toString();
    m_entries.at(remoteEndPoint).endContact(face);
    auto it = (m_entries.at(remoteEndPoint).end() - 1 );
    double contactDuration = it->endTime - it->startTime;
    uint64_t numberOfInBytes = face.getCounters().nInBytes;
    uint64_t numberOfSentInterest= face.getCounters().nOutInterests;
    uint64_t numberOfReceivedInterest = face.getCounters().nInInterests;
    uint64_t numberOfSentData = face.getCounters().nInData;
    uint64_t numberOfReceivedData = face.getCounters().nOutData;
    auto entry = std::make_tuple(contactDuration,numberOfInBytes, numberOfSentInterest, numberOfReceivedInterest, numberOfSentData, numberOfReceivedData);
    m_contactVector.push_back(entry);
}

std::map<std::string, NodeContactTableEntry>::iterator NodeContactTable::begin() {
    return m_entries.begin();
}

std::map<std::string, NodeContactTableEntry>::iterator NodeContactTable::end() {
    return m_entries.end();
}

void NodeContactTable::printDebugInfos() {
    for(auto& it : m_entries){
        std::cout<<it.first<<"------------------------------------------------------"<<std::endl;
        for(auto& e : it.second){
            std::cout<<" duration="<<e.endTime - e.startTime<<"   start="<<e.startTime<<"   end="<<e.endTime
                     <<"   inInterest="<<e.numberOfReceivedInterest<<"  inData="<<e.numberOfReceivedData
                     <<"     outInterest="<<e.numberOfSentInterest<<"    outData="<<e.numberOfSentData<<std::endl;
        }
    }
}

double NodeContactTable::getAverageContactDuration() {
    return getFullContactDuration() / (double) m_contactVector.size();
}

double NodeContactTable::getFullContactDuration() {
    double fullContactDuration = 0.0;
    for(auto& it : m_contactVector){
        fullContactDuration += std::get<0>(it);
    }
    return fullContactDuration;
}

uint64_t NodeContactTable::getFullTransferedBytes() {
    uint64_t fullTransfferedBytes = 0;
    for(auto& it : m_contactVector){
        fullTransfferedBytes += std::get<1>(it);
    }
    return fullTransfferedBytes;
}

double NodeContactTable::getAverageTransferredBytesPerContact() {
    return getFullTransferedBytes() / (double) m_contactVector.size();
}

uint64_t NodeContactTable::getNumberOfAllSentInterests() {
    uint64_t totalNumberOfSentInterest = 0;
    for(auto& it : m_contactVector){
        totalNumberOfSentInterest += std::get<2>(it);
    }
    return totalNumberOfSentInterest;
}

uint64_t NodeContactTable::getNumberOfSuccessfullyTransferedInterests() {
    uint64_t totalNumberOfReceivedInterest = 0;
    for(auto& it : m_contactVector){
        totalNumberOfReceivedInterest += std::get<3>(it);
    }
    return totalNumberOfReceivedInterest;
}

uint64_t NodeContactTable::getNumberOfAllSentData() {
    uint64_t totalNumberOfsentData = 0;
    for (auto &it : m_contactVector) {
        totalNumberOfsentData += std::get<5>(it);
    }
    return totalNumberOfsentData;
}

uint64_t NodeContactTable::getNumberOfSuccessfullyTranssferredData() {
    uint64_t totalNumberOfReceivedData = 0;
    for (auto &it : m_contactVector) {
        totalNumberOfReceivedData += std::get<4>(it);
    }
    return totalNumberOfReceivedData;

}


