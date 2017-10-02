//
// Created by lahmer on 02/09/17.
//

#include <numeric>
#include "NodeContactTable.h"

std::vector<std::tuple<double,uint64_t>> NodeContactTable::m_contactVector;

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
    auto entry = std::make_tuple(contactDuration,numberOfInBytes);
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
    auto acc =  std::accumulate(m_contactVector.begin(),m_contactVector.end(),std::make_tuple(0.0,(uint64_t)0),[](std::tuple<double,uint64_t>a, std::tuple<double,uint64_t> b){
        return std::make_tuple(std::get<0>(a)+std::get<0>(b),std::get<1>(a)+std::get<1>(b));
    });
    return std::get<0>(acc);
}

uint64_t NodeContactTable::getFullTransferedBytes() {
    auto acc = std::accumulate(m_contactVector.begin(), m_contactVector.end(), std::make_tuple(0.0,(uint64_t)0), [](std::tuple<double,uint64_t>a, std::tuple<double,uint64_t> b){
            return std::make_tuple(std::get<0>(a)+std::get<0>(b),std::get<1>(a)+std::get<1>(b));
        });

    return std::get<1>(acc);
}

double NodeContactTable::getAverageTransferredBytesPerContact() {
    return getFullTransferedBytes() / (double) m_contactVector.size();
}

