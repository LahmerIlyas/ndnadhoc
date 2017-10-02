//
// Created by lahmer on 9/26/17.
//

#ifndef SCENARIO_GENERICSCENARIO_H
#define SCENARIO_GENERICSCENARIO_H

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include "ns3/wifi-module.h"
#include <ns3/core-module.h>
#include <ns3/mobility-module.h>
#include <ns3/ndn-all.hpp>
#include <ns3/energy-module.h>


class GenericScenario {
public:


    void printTracersParameters();

protected:
    void installEnergySources();

    void setupTracers();

    void setupWifiParameters();

    void installNDNStack();

    void setup802_11_a();

    void setup802_11_g();
protected:
    //-----------------------------------------------------------------------------------
    //This part is for the tracers callbacks of the wifi station manager
    //-----------------------------------------------------------------------------------
    /**
     *  Triggered when the transmission of RTS by the MAC layer has failed
     */
    void macTxRtsFailed(ns3::Mac48Address address);

    /**
     * Triggered when the transmission of packet is failed
     */
    void macTxPacketFailed(ns3::Mac48Address address);

    /**
     * Triggered whrn the number of retransmission of RTS packet exceeded the limit
     * */
    void rtsTxExceededLimits(ns3::Mac48Address address);

    /**
     * Triggered when the number of retransmission of ns3 packet exceeded the limit
     */
    void packetTxExceededLimits(ns3::Mac48Address address);

    //-----------------------------------------------------------------------------
    //  Tracers related to the wifi phy
    //-----------------------------------------------------------------------------

    /**
     *  Triggered when there is a phy drop during the transmission of a packet
     */
    void phyTxDrop(ns3::Ptr<const ns3::Packet> packet);

    /**
     *  Triggered when there is a phy drop
     */
    void phyRxDrop(ns3::Ptr<const ns3::Packet> packet);

    //-----------------------------------------------------------------------------
    //  Tracers related to the wifi mac
    //-----------------------------------------------------------------------------

    /**
     * Triggered when the mac layer receives a packet from higher layer
     * to be sent and drops it before queuing it for transmission
     */
    void macDropTxPacket(ns3::Ptr<const ns3::Packet> packet);

    /**
     * a packet has been dropped by the mac layer after being received by
     * the physical layer
     */
    void macDropRxPacket(ns3::Ptr<const ns3::Packet> packet);

public:
    ns3::NodeContainer m_nodes;

    ns3::NetDeviceContainer m_devices;

    ns3::ApplicationContainer   m_clients;

    ns3::ApplicationContainer   m_producers;

    ns3::DeviceEnergyModelContainer m_energyModels;

    std::string m_standard = "802.11g";

    std::string m_wifiMode = "ErpOfdmRate54Mbps";                         //each standard has its own modes

    //modes check wifi-phy.cc && yans-wifi-phy.cc
    uint32_t m_initialSeed = 0  ;         //This is helpfull to make results reporoducible

    double m_simulationDuration = 30.0;         //30 seconds by default

    static std::string m_strategy;

    double                      m_initialNodeEnergy = 100000.0;
    //-----------------------------------------------------------------------------
    //wifi related parameters
    //-----------------------------------------------------------------------------

    /**
     * The maximum number of retransmission attempts
     * of an RTS packet
     */
    uint32_t m_maxRtsRetransmissionAttempts = 7;

    /**
     * The maximum number of retransmission attempts
     * of a ns3 packet
     */
    uint32_t m_maxPacketRetransmissionAttempts = 7;

    /**
     * The limit size of the packet + headers to trigger
     * the RTS CTS mechanism
     */
    uint32_t m_rtsCtsThreshold = 2346;

    /**
     * The size of the maximum packet sent
     */
    uint32_t m_fragmentationTreshold = 2346;

    /**
     * See ns3 doc
     */
    bool m_isLowLatency = true;

protected:
    ///This part is for tracers parameters

    uint64_t m_macTxRtsFailed = 0;

    uint64_t m_macTxPacketFailed = 0;

    uint64_t m_rtsExceededLimits = 0;

    uint64_t m_packetRetransmissioExceededLimits = 0;

    uint64_t m_phyTxDrop = 0;

    uint64_t m_phyRxDrop = 0;

    uint64_t m_macTxDrop = 0;

    uint64_t m_macRxDrop = 0;



    const std::vector<std::string> availableWifiMode_802_11_a = {
            "OfdmRate6Mbps",
            "OfdmRate9Mbps",
            "OfdmRate12Mbps",
            "OfdmRate18Mbps",
            "OfdmRate24Mbps",
            "OfdmRate36Mbps",
            "OfdmRate48Mbps",
            "OfdmRate54Mbps"
    };
    const std::vector<std::string> availableWifiMode_802_11_g = {
            "DsssRate1Mbps",
            "DsssRate2Mbps",
            "DsssRate5_5Mbps",
            "ErpOfdmRate6Mbps",
            "ErpOfdmRate9Mbps",
            "DsssRate11Mbps",
            "ErpOfdmRate12Mbps",
            "ErpOfdmRate18Mbps",
            "ErpOfdmRate24Mbps",
            "ErpOfdmRate36Mbps",
            "ErpOfdmRate48Mbps",
            "ErpOfdmRate54Mbps"
    };


};


#endif //SCENARIO_GENERICSCENARIO_H
