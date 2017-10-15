//
// Created by lahmer on 02/09/17.
//

#ifndef SCENARIO_NODECONTACTTABLE_H
#define SCENARIO_NODECONTACTTABLE_H


#include <daemon/face/face.hpp>
#include "NodeContactTableEntry.h"
struct ContactStatistics{
    double averageContatactDuration;
    double averageTransfferedBytesByContact;
    double totalTransfferedBytes;
};

class NodeContactTable {
public:

    void addContact(const nfd::Face& face);

    void endContact(const nfd::Face& face);

    std::map<std::string, NodeContactTableEntry>::iterator begin();

    std::map<std::string, NodeContactTableEntry>::iterator end();

    void printDebugInfos();

    static double getAverageContactDuration();

    static double getFullContactDuration();

    static uint64_t getFullTransferedBytes();

    static double getAverageTransferredBytesPerContact();

    static uint64_t getNumberOfSuccessfullyTransferedInterests();

    static uint64_t getNumberOfAllSentInterests();

    static uint64_t getNumberOfSuccessfullyTranssferredData();

    static uint64_t getNumberOfAllSentData();


private:
    std::map<std::string, NodeContactTableEntry> m_entries;

    static std::vector<std::tuple<double,uint64_t, uint64_t, uint64_t, uint64_t, uint64_t >> m_contactVector;                         //this is used to log the contact duration for metrics
};


#endif //SCENARIO_NODECONTACTTABLE_H
