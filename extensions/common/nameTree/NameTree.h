//
// Created by lahmer on 05/10/17.
//

#ifndef SCENARIO_NAMETREE_H
#define SCENARIO_NAMETREE_H


#include "Node.h"
#include "LeafNode.h"
/**
 * This implementation is for exact name matching purposes only, it's optimized only for
 * the purpose of making simulation runs faster, nothing else
 * We make the assumptions that there won't be two Interests/Data where one is the prefix of another
 * ex :
 *      /ndn/data1
 *      /ndn/data1/1
 *
 */


class NameTree {
public:
    NameTree();

/**
     * A constructor that takes a serialized name tree and deserilized it to reconstruct it
     * @param serilizedNameTree
     * @return
     */
    NameTree(const std::string& serilizedNameTree);

    /**
     * Checks if there is an entry for this specific name
     * If so, retyrn a pointer to the leaf node
     * Otherwise, return null pointer
     *
     */

    LeafNode* lookupEntry(ndn::Name& name);

    /**
     * This method will search for an entry, If it already exists, it will return a pointer to the leafNode of the
     * entry. If it doesn't exists, a new entry will be created and inserted
     *
     * @param name
     * @return
     */
    LeafNode* insertEntry(ndn::Name& name);

    /**
     * This method will serialize the nametree to be sended to other nodes
     * @return
     */
    std::string serialize();





public:
    Node* m_root;              //this is the root node, it contains no name

    std::unordered_multimap<uint64, LeafNode*> m_leafs;
};


#endif //SCENARIO_NAMETREE_H
