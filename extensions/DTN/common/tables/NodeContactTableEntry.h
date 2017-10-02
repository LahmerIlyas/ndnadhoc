//
// Created by lahmer on 02/09/17.
//

#ifndef SCENARIO_NODECONTACTTABLEENTRY_H
#define SCENARIO_NODECONTACTTABLEENTRY_H

#include <ns3/simulator.h>
#include <map>
#include <ns3/ndnSIM/ndn-cxx/face.hpp>

struct Contact{
    double startTime;
    double endTime;
    uint64_t numberOfSentInterest = 0;
    uint64_t numberOfSentData = 0;
    uint64_t numberOfReceivedInterest = 0;
    uint64_t numberOfReceivedData = 0;
    double   timeOfLastReceivedInterest = -1.0;
    double   timeOfTheLastReceivedData  = -1.0;

};
typedef std::vector<Contact> ContactVector;

class NodeContactTableEntry {
public:

    NodeContactTableEntry(const nfd::Face& face);

    void addContact(const nfd::Face& face);

    void endContact(const nfd::Face& face);

    ContactVector::iterator begin();

    ContactVector::iterator end();

private:
    std::string m_remoteNodeName;
    ContactVector m_contacts;
};


#endif //SCENARIO_NODECONTACTTABLEENTRY_H
