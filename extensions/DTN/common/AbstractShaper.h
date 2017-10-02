//
// Created by lahmer on 29/09/17.
//

#ifndef SCENARIO_ABSTRACTSHAPER_H
#define SCENARIO_ABSTRACTSHAPER_H


#include <fw/forwarder.hpp>
#include <ns3/wifi-module.h>
#include <daemon/face/face.hpp>
#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"

class AbstractShaper {
public:

    AbstractShaper(ns3::Ptr<ns3::WifiNetDevice> device, nfd::Forwarder& forwarder);

    virtual void Enqueue(std::shared_ptr<ndn::Interest> interest, const nfd::Face& ouFace);

    virtual void Enqueue(std::shared_ptr<ndn::Data> data, const nfd::Face& outFace);

    virtual void onContact(const nfd::Face& face);

    virtual void onContactLost(const nfd::Face& face);

    virtual void startInterestSendingLoop();

    virtual void startDataSendingLoop();

    double getDataRateInBytePerSeconds();

protected:
    ns3::Ptr<ns3::AdhocWifiMac> getWifiMac();

    ns3::Ptr<ns3::WifiMacQueue> getMacQueue();

    ns3::Ptr<ns3::WifiNetDevice> m_device;

    nfd::Forwarder& forwarder;

    ns3::Ptr<ns3::UniformRandomVariable> m_rand;

};


#endif //SCENARIO_ABSTRACTSHAPER_H
