//
// Created by lahmer on 31/07/17.
//
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include <ns3/ndnSIM/ndn-cxx/lp/tags.hpp>
#include "citEntry.h"

const shared_ptr<ndn::Interest> &citEntry::getInterest() const {
    return m_interest;
}

double citEntry::getReceivedAt() const {
    return m_receivedAt;
}

uint32_t citEntry::getHops() const {
    return m_hops;
}

ndn::Name citEntry::getName() {
    return m_interest->getName();
}

bool citEntry::operator==(const ndn::Interest &interest) {
    return m_interest->getName().compare(interest.getName()) == 0;
}

citEntry::citEntry(const ndn::Interest &interest) {
    //make a copy of the interest
    m_interest = std::make_shared<ndn::Interest>();
    m_interest->setName(interest.getName());
    m_interest->setNonce(interest.getNonce());
    m_interest->setInterestLifetime(interest.getInterestLifetime());
    m_hops       = interest.getTag<nfd::lp::HopCountTag>()->get();
    m_interest->setTag(std::make_shared<nfd::lp::HopCountTag>(m_hops));

    m_receivedAt = ns3::Simulator::Now().GetSeconds();
}

bool citEntry::operator==(const ndn::Data &data) {
    return data.getName().compare(m_interest->getName()) == 0;
}

citEntry::citEntry() {
    std::cout<<" The constructor of the cit entry should never be called "<<std::endl;
    std::terminate();
}

