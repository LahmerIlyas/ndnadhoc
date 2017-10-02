//
// Created by lahmer on 31/07/17.
//

#ifndef SCENARIO_CIT_H
#define SCENARIO_CIT_H


#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <ns3/core-module.h>
#include <ns3/ndn-all.hpp>
#include "citEntry.h"
#include <boost/iterator/transform_iterator.hpp>

typedef std::unordered_map<ndn::Name, citEntry> citTable;
typedef citTable::iterator citIterator;

class cit {
public:

    cit();

    void insert(const ndn::Interest& interest);

    uint32_t getLimit() const;

    void setLimit(uint32_t limit);

    citIterator begin() {
        return m_entries.begin();
    }

    citIterator end() {
        return m_entries.end();
    }

    void remove(const ndn::Interest& interest);

    void remove(const ndn::Data& data);

    unsigned long size();

private:
    uint32_t m_limit;
    citTable m_entries;

};


#endif //SCENARIO_CIT_H
