//
// Created by lahmer on 31/07/17.
//

#ifndef SCENARIO_CITENTRY_H
#define SCENARIO_CITENTRY_H

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <ns3/core-module.h>
#include <ns3/ndn-all.hpp>

class citEntry {
public:
    citEntry();

    citEntry(const ndn::Interest& interest);

    bool operator == (const ndn::Interest& interest);

    bool operator == (const ndn::Data& data);

    ndn::Name getName();

    const shared_ptr<ndn::Interest> &getInterest() const;

    double getReceivedAt() const;

    uint32_t getHops() const;

private:
    std::shared_ptr<ndn::Interest> m_interest;
    double m_receivedAt;
    uint32_t m_hops;
};


#endif //SCENARIO_CITENTRY_H
