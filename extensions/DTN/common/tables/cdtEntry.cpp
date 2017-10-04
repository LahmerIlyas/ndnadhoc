//
// Created by lahmer on 01/08/17.
//
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include <ns3/ndnSIM/ndn-cxx/lp/tags.hpp>
#include <stdint.h>
#include "cdtEntry.h"

cdtEntry::cdtEntry(const ndn::Data &data) {
    auto dt = make_shared<ndn::Data>();
    dt->setName(data.getName());
    dt->setFreshnessPeriod(data.getFreshnessPeriod());
    dt->setContent(data.getContent().value(), data.getContent().size());
    dt->setSignature(data.getSignature());
    m_hops          = data.getTag<nfd::lp::HopCountTag>()->get();
    dt->setTag(std::make_shared<nfd::lp::HopCountTag>(m_hops));

    m_data          = dt;
    m_reveivedAt    = ns3::Simulator::Now().GetSeconds();
}

uint64_t cdtEntry::getSize() {
    return m_data->getContent().size();
}

bool cdtEntry::operator==(const ndn::Data &data) {
    return m_data->getName().compare(data.getName()) == 0;
}

double cdtEntry::getReveivedAt() const {
    return m_reveivedAt;
}

const uint32_t &cdtEntry::getHops() const {
    return m_hops;
}

const shared_ptr<ndn::Data> &cdtEntry::getData() const {
    return m_data;
}
