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
#include <boost/signal.hpp>
#include <boost/bind.hpp>


typedef std::unordered_map<ndn::Name,cdtEntry> cdtTable;
typedef cdtTable::iterator    cdtIterator;

class cdt {
public:
    cdt(double size = 1000);

    void insert(const ndn::Data& data);

    void remove(const ndn::Data& data);

    cdtIterator begin();

    cdtIterator end();

    double size();

    double getSizeKo();

    void setKoLimitSize(double limitSize);

    boost::signal<void ()>  onFullBuffer;

    bool isHere(const ndn::Name name);

    cdtIterator getEntryByName(const ndn::Name& name);

    void remove(const ndn::Name& name);

    void removeOldest();

private:
    void logBufferSize();
private:
    double                  m_limitSize;
    //cdtTable                m_table;
    double                  m_tableSizeInKo = 0;
    std::unordered_map<ndn::Name,cdtEntry> m_entries;

    std::vector<std::pair<double, double>> m_bufferSizeEvolution;       //<time, buffer size>
};


#endif
