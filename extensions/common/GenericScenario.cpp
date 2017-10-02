//
// Created by lahmer on 9/26/17.
//

#include "GenericScenario.h"
#include "WifiStackHelper.h"

std::string GenericScenario::m_strategy = "/localhost/nfd/strategy/epidimicNDN";

void GenericScenario::macTxRtsFailed(ns3::Mac48Address address) {
    m_macTxRtsFailed++;
}

void GenericScenario::macTxPacketFailed(ns3::Mac48Address address) {
    m_macTxPacketFailed++;
}

void GenericScenario::rtsTxExceededLimits(ns3::Mac48Address address) {
    m_rtsExceededLimits++;
}

void GenericScenario::packetTxExceededLimits(ns3::Mac48Address address) {
    m_packetRetransmissioExceededLimits++;
}

void GenericScenario::phyTxDrop(ns3::Ptr<const ns3::Packet> packet) {
    m_phyTxDrop++;
}

void GenericScenario::phyRxDrop(ns3::Ptr<const ns3::Packet> packet) {
    m_phyRxDrop++;
}

void GenericScenario::macDropTxPacket(ns3::Ptr<const ns3::Packet> packet) {
    m_macTxDrop++;
}

void GenericScenario::macDropRxPacket(ns3::Ptr<const ns3::Packet> packet) {
    m_macRxDrop++;
}

void GenericScenario::installEnergySources() {
    ns3::BasicEnergySourceHelper basicSourceHelper;
    basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", ns3::DoubleValue (m_initialNodeEnergy));
    ns3::EnergySourceContainer sources = basicSourceHelper.Install (m_nodes);
    ns3::WifiRadioEnergyModelHelper radioEnergyHelper;
    m_energyModels = radioEnergyHelper.Install (m_devices, sources);
}

void GenericScenario::setupTracers() {
    for(auto it = m_devices.Begin(); it != m_devices.End(); it++){
        auto wifiDevice = ns3::DynamicCast<ns3::WifiNetDevice>(*(it));
        auto mac        = wifiDevice->GetMac();
        auto phy        = wifiDevice->GetPhy();
        auto station    = wifiDevice->GetRemoteStationManager();
        //setup mac tracers
        mac->TraceConnectWithoutContext("MacTxDrop", ns3::MakeCallback(&GenericScenario::macDropTxPacket,this));
        mac->TraceConnectWithoutContext("MacRxDrop", ns3::MakeCallback(&GenericScenario::macDropRxPacket,this));
        //setup phy tracers
        phy->TraceConnectWithoutContext("PhyTxDrop", ns3::MakeCallback(&GenericScenario::phyTxDrop,this));
        phy->TraceConnectWithoutContext("PhyRxDrop", ns3::MakeCallback(&GenericScenario::phyRxDrop,this));
        //setup station tracers
        station->TraceConnectWithoutContext("MacTxRtsFailed", ns3::MakeCallback(&GenericScenario::macTxRtsFailed,this));
        station->TraceConnectWithoutContext("MacTxDataFailed", ns3::MakeCallback(&GenericScenario::macTxPacketFailed,this));
        station->TraceConnectWithoutContext("MacTxFinalRtsFailed", ns3::MakeCallback(&GenericScenario::rtsTxExceededLimits,this));
        station->TraceConnectWithoutContext("MacTxFinalDataFailed", ns3::MakeCallback(&GenericScenario::packetTxExceededLimits,this));

    }
}

void GenericScenario::setupWifiParameters() {
    for(auto it = m_devices.Begin(); it != m_devices.End(); it++) {
        auto wifiDevice = ns3::DynamicCast<ns3::WifiNetDevice>(*(it));
        auto mac = wifiDevice->GetMac();
        auto phy = wifiDevice->GetPhy();
        auto station = wifiDevice->GetRemoteStationManager();
        //setting up mac parameters

        //setting the phy parameters

        //setting the station manager parameters
        station->SetAttribute("MaxSsrc",ns3::UintegerValue(m_maxRtsRetransmissionAttempts));
        station->SetAttribute("MaxSlrc",ns3::UintegerValue(m_maxPacketRetransmissionAttempts));
        station->SetAttribute("RtsCtsThreshold",ns3::UintegerValue(m_rtsCtsThreshold));
        station->SetAttribute("FragmentationThreshold",ns3::UintegerValue(m_fragmentationTreshold));
        //station->SetAttribute("IsLowLatency",ns3::BooleanValue(m_isLowLatency));    // what is this ??
        station->SetAttribute("NonUnicastMode",ns3::StringValue(m_wifiMode));
    }

}

void GenericScenario::installNDNStack() {
    ns3::ndn::WifiStackHelper wifiStackHelper;
    std::cout<<" #############################################Installign strategy "<<m_strategy<<std::endl;
    wifiStackHelper.Install(m_nodes);
    ns3::ndn::StrategyChoiceHelper::Install(m_nodes, "/", m_strategy);

}

void GenericScenario::printTracersParameters() {
    std::cout<<" m_macTxRtsFailed  = "<<m_macTxRtsFailed<<std::endl;
    std::cout<<" m_macTxPacketFailed  = "<<m_macTxPacketFailed<<std::endl;
    std::cout<<" m_rtsExceededLimits  = "<<m_rtsExceededLimits<<std::endl;
    std::cout<<" m_packetRetransmissioExceededLimits  = "<<m_packetRetransmissioExceededLimits<<std::endl;
    std::cout<<" m_phyTxDrop  = "<<m_phyTxDrop<<std::endl;
    std::cout<<" m_phyRxDrop  = "<<m_phyRxDrop<<std::endl;
    std::cout<<" m_macTxDrop  = "<<m_macTxDrop<<std::endl;
    std::cout<<" m_macRxDrop  = "<<m_macRxDrop<<std::endl;

}

void GenericScenario::setup802_11_a() {
    ns3::YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel");
    //wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");
    /**
     * I don't know what the fuck these parameters means, the most important is that they do the job
     * I copied them from https://github.com/sergiosvieira/ns3-dtn/blob/master/scratch/aodv-test.cc
     */
    ns3::YansWifiPhyHelper wifiPhyHelper = ns3::YansWifiPhyHelper::Default();
    wifiPhyHelper.Set("TxPowerStart", ns3::DoubleValue(16.0));
    wifiPhyHelper.Set("TxPowerEnd", ns3::DoubleValue(16.0));
    wifiPhyHelper.Set("TxPowerLevels", ns3::UintegerValue(1));
    double threshold = -69.6779;
    //100m threshold = -69.6779;
    /// 200m threshold = -75.6985;
    // 300m threshold = -79.2203;
    // 500m threshold = -83.6573;
    // 800m threshold = -87.7397
    // 1000m threshold = -89.6779
    wifiPhyHelper.Set("EnergyDetectionThreshold", ns3::DoubleValue(threshold));
    wifiPhyHelper.Set("CcaMode1Threshold", ns3::DoubleValue(threshold + 3.0));
    wifiPhyHelper.Set("RxNoiseFigure", ns3::DoubleValue(4.0));
    wifiPhyHelper.SetChannel(wifiChannel.Create());


    ns3::NqosWifiMacHelper wifiMacHelper = ns3::NqosWifiMacHelper::Default();
    wifiMacHelper.SetType("ns3::AdhocWifiMac");

    ns3::WifiHelper wifi = ns3::WifiHelper::Default();
    wifi.SetStandard(ns3::WIFI_PHY_STANDARD_80211a);

    //we test if the wifi mode is correct for the standard
    if(std::find(availableWifiMode_802_11_a.begin(),availableWifiMode_802_11_a.end(),m_wifiMode) == availableWifiMode_802_11_a.end()){
        std::cout<<" the wifi mode "<<m_wifiMode<<" is not available for 802.11a standard"<<std::endl;
        std::terminate();
    }

    wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager",
                                  "DataMode", ns3::StringValue (m_wifiMode));

    m_devices = wifi.Install(wifiPhyHelper, wifiMacHelper, m_nodes);

}

void GenericScenario::setup802_11_g() {
    ns3::YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel");
    //wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");

    /**
     * I don't know what the f**k these parameters mean, the most important is that they do the job
     * I copied them from https://github.com/sergiosvieira/ns3-dtn/blob/master/scratch/aodv-test.cc
     */
    ns3::YansWifiPhyHelper wifiPhyHelper = ns3::YansWifiPhyHelper::Default();
    wifiPhyHelper.Set("TxPowerStart", ns3::DoubleValue(16.0));
    wifiPhyHelper.Set("TxPowerEnd", ns3::DoubleValue(16.0));
    wifiPhyHelper.Set("TxPowerLevels", ns3::UintegerValue(1));
    double threshold = -69.6779;
    //100m threshold = -69.6779;
    // 200m threshold = -75.6985;
    // 300m threshold = -79.2203;
    // 500m threshold = -83.6573;
    // 800m threshold = -87.7397
    // 1000m threshold = -89.6779
    wifiPhyHelper.Set("EnergyDetectionThreshold", ns3::DoubleValue(threshold));
    wifiPhyHelper.Set("CcaMode1Threshold", ns3::DoubleValue(threshold + 3.0));
    wifiPhyHelper.Set("RxNoiseFigure", ns3::DoubleValue(4.0));
    wifiPhyHelper.SetChannel(wifiChannel.Create());


    ns3::NqosWifiMacHelper wifiMacHelper = ns3::NqosWifiMacHelper::Default();
    wifiMacHelper.SetType("ns3::AdhocWifiMac");

    ns3::WifiHelper wifi = ns3::WifiHelper::Default();
    wifi.SetStandard(ns3::WIFI_PHY_STANDARD_80211g);

    //we test if the wifi mode is correct for the standard
    if(std::find(availableWifiMode_802_11_g.begin(),availableWifiMode_802_11_g.end(),m_wifiMode) == availableWifiMode_802_11_g.end()){
        std::cout<<" the wifi mode "<<m_wifiMode<<" is not available for 802.11g standard"<<std::endl;
        std::terminate();
    }

    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode", ns3::StringValue (m_wifiMode));

    m_devices = wifi.Install(wifiPhyHelper, wifiMacHelper, m_nodes);

}