//
// Created by lahmer on 05/10/17.
//

#ifndef SCENARIO_LEAFNODE_H
#define SCENARIO_LEAFNODE_H


#include "Node.h"
#include "../../DTN/common/tables/citEntry.h"
#include "../../DTN/common/tables/cdtEntry.h"

/**
 * Only leaf nodes are allowed to contain entries of CDT and CIT
 */

class LeafNode : public Node{
public:
    LeafNode(Node *parent, const ndn::Name::Component &component);

    ndn::Name getEntryName();

    bool hasCITEntry();

    bool hasCDTEntry();

    cdtEntry& getCDTEntry();

    citEntry& getCITEntry();

    void eraseCITEntry();

    void eraseCDTEntry();

    void addCITEntry(std::shared_ptr<citEntry> entry);

    void addCDTEntry(std::shared_ptr<cdtEntry> entry);
private:
    std::shared_ptr<cdtEntry> m_cdtEntry;
    std::shared_ptr<citEntry> m_citEntry;
};


#endif //SCENARIO_LEAFNODE_H
