//
// Created by lahmer on 9/14/17.
//

#ifndef SCENARIO_SUMMARYNAMETREE_H
#define SCENARIO_SUMMARYNAMETREE_H


#include <ns3/ndn-all.hpp>
#include <boost/property_tree/ptree.hpp>

class SummaryNameTree {
public:
    /**
     * Construct an empty tree
     * @return
     */
    SummaryNameTree();

    /**
     * Deserialize a tree
     * @param serializedTree
     * @return
     */
    SummaryNameTree(const std::string& serializedTree);

    void pushName(const ndn::Name& name);

    bool lookUpName(const ndn::Name& name);

    std::string serialize();
private:


private:
    boost::property_tree::ptree m_tree;
};


#endif //SCENARIO_SUMMARYNAMETREE_H
