//
// Created by lahmer on 01/08/17.
//

#ifndef SCENARIO_CDT_H
#define SCENARIO_CDT_H

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <ns3/core-module.h>
#include <ns3/ndn-all.hpp>
#include "cdtEntry.h"
#include "../../../common/nameTree/NameTree.h"
#include <boost/signal.hpp>
#include <boost/bind.hpp>


typedef std::unordered_map<ndn::Name,cdtEntry> cdtTable;
typedef cdtTable::iterator    cdtIterator;

class cdt {
public:
    cdt(NameTree& nt, double size = 1000);

    void insert(const ndn::Data& data);

    void remove(const ndn::Data& data);

    boost::signal<void ()>  onFullBuffer;

    bool isHere(const ndn::Name name);

    void iterateOverElements(std::function<void (cdtEntry& entry)> func);
private:
    uint64                  m_limitSizeInBytes;

    uint64                  m_currentSizeInBytes = 0;

    NameTree&               m_nt;
};


#endif
