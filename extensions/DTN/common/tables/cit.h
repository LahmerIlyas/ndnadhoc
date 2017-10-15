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
#include "../../../common/nameTree/NameTree.h"
#include <boost/iterator/transform_iterator.hpp>

class cit {
public:

    cit(NameTree& tree);

    void insert(const ndn::Interest& interest);

    void remove(const ndn::Interest& interest);

    void remove(const ndn::Data& data);

    bool contains(const ndn::Name& name);

    void iterateOverElements(std::function<void (citEntry& entry)> func);

    std::string getJsonSummary();

    size_t size();
private:
    NameTree& m_nt;
};


#endif //SCENARIO_CIT_H
