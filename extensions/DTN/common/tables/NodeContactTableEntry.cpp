//
// Created by lahmer on 02/09/17.
//

#include <daemon/face/face.hpp>
#include "NodeContactTableEntry.h"

NodeContactTableEntry::NodeContactTableEntry(const nfd::Face& face) {
    std::string remoteEndPoint = face.getRemoteUri().toString();

}

void NodeContactTableEntry::addContact(const nfd::Face& face) {
    //we verify if the last contact is done or not
    if(m_contacts.size() != 0)
        if(m_contacts.rbegin()->endTime == -1.0){
            std::cout<<" Error the last contact isn't done yet and you're trying to add a new contact "<<std::endl;
            std::terminate();
        }

    Contact contact;
    contact.endTime = -1.0;
    contact.startTime = ns3::Simulator::Now().GetSeconds();
    m_contacts.push_back(contact);

}

void NodeContactTableEntry::endContact(const nfd::Face& face) {
    //verify that there is a pending contact
    if(m_contacts.size() != 0)
        if(m_contacts.rbegin()->endTime != -1.0){
            std::cout<<" Eroor there is no pending contact to finish "<<std::endl;
            std::terminate();
        }
    m_contacts.rbegin()->endTime = ns3::Simulator::Now().GetSeconds();

    m_contacts.rbegin()->numberOfReceivedInterest = face.getCounters().nInInterests;
    m_contacts.rbegin()->numberOfReceivedData = face.getCounters().nInData;

    m_contacts.rbegin()->numberOfSentInterest = face.getCounters().nOutInterests;
    m_contacts.rbegin()->numberOfSentData = face.getCounters().nOutData;
}

ContactVector::iterator NodeContactTableEntry::begin() {
    return m_contacts.begin();
}

ContactVector::iterator NodeContactTableEntry::end() {
    return m_contacts.end();
}

