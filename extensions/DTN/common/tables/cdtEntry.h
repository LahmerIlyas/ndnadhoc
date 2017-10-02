//
// Created by lahmer on 01/08/17.
//

#ifndef SCENARIO_CDTENTRY_H
#define SCENARIO_CDTENTRY_H

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <ns3/core-module.h>
#include <ns3/ndn-all.hpp>

class cdtEntry {
public:
    cdtEntry(){
        std::cout<<" Error this constructor should never be called "<<std::endl;
        std::terminate();
    }

    cdtEntry(const ndn::Data& data);

    uint64_t getSize();

    bool operator==(const ndn::Data& data);

    double getReveivedAt() const;

    const uint32_t &getHops() const;

    const shared_ptr<ndn::Data> &getData() const;

private:
    std::shared_ptr<ndn::Data>  m_data;
    double                      m_reveivedAt;
    uint32_t                    m_hops;





};


#endif //SCENARIO_CDTENTRY_H
