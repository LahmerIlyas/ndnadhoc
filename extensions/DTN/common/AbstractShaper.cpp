//
// Created by lahmer on 29/09/17.
//

#include <ns3/pointer.h>
#include "AbstractShaper.h"

AbstractShaper::AbstractShaper(ns3::Ptr<ns3::WifiNetDevice> device, nfd::Forwarder& f) :m_device(device),forwarder(f),
                                                                                        m_rand(ns3::CreateObject<ns3::UniformRandomVariable>()){

}

void AbstractShaper::Enqueue(std::shared_ptr<ndn::Interest> interest, const nfd::Face &ouFace) {
    std::cout<<" Enqueue is not supported in the base class"<<std::endl;
    std::terminate();
}

void AbstractShaper::Enqueue(std::shared_ptr<ndn::Data> data, const nfd::Face &outFace) {
    std::cout<<" Enqueue is not supported in the base class"<<std::endl;
    std::terminate();
}

void AbstractShaper::onContact(const nfd::Face &face) {
    std::cout<<" onContact is not supported in the base class"<<std::endl;
    std::terminate();
}

void AbstractShaper::onContactLost(const nfd::Face &face) {
    std::cout<<" onContactLost is not supported in the base class"<<std::endl;
    std::terminate();
}

ns3::Ptr<ns3::AdhocWifiMac> AbstractShaper::getWifiMac() {
    auto mac = ns3::DynamicCast<ns3::AdhocWifiMac>(m_device->GetMac());
    if(mac == nullptr){
        std::cout<<" Eroor while retrieving the adhoc wifi mac"<<std::endl;
        std::terminate();
    }
    return mac;
}

ns3::Ptr<ns3::WifiMacQueue> AbstractShaper::getMacQueue(){
    ns3::PointerValue value;
    getWifiMac()->GetAttribute("DcaTxop", value);
    ns3::Ptr<ns3::DcaTxop> queue = ns3::DynamicCast<ns3::DcaTxop>(value.GetObject());
    auto q = queue->GetQueue();
    if(q == nullptr){
        std::cout<<" Error while retrieving the mac queue"<<std::endl;
        std::terminate();
    }
    return q;
}

void AbstractShaper::startInterestSendingLoop() {
    std::cout<<" Erro interestLoop sending Base class should never be called "<<std::endl;
    std::terminate();
}

void AbstractShaper::startDataSendingLoop() {
    std::cout<<" Error dataLoop sending should never be called "<<std::endl;
    std::terminate();
}

double AbstractShaper::getDataRateInBytePerSeconds() {
    if(m_device == nullptr){
        ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(ns3::Simulator::GetContext());
        m_device = ns3::DynamicCast<ns3::WifiNetDevice>(node->GetDevice(0));
    }
    //Here I suppose that the selected rate is fixed
    auto station = ns3::DynamicCast<ns3::ConstantRateWifiManager>(m_device->GetRemoteStationManager());
    ns3::WifiModeValue mode;
    station->GetAttribute("DataMode",mode);
    auto item = mode.Get();
    return item.GetDataRate() / 8.0 ;
}

