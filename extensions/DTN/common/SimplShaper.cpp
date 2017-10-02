//
// Created by lahmer on 29/09/17.
//

#include "SimplShaper.h"
namespace SimpleShaper {
    SimpleShaper::SimpleShaper(ns3::Ptr<ns3::WifiNetDevice> device, ::nfd::Forwarder& f) : AbstractShaper(device, f) {

    }

    void SimpleShaper::Enqueue(std::shared_ptr<ndn::Interest> interest, const nfd::Face &ouFace) {
        interestQueue.push_back(std::make_pair(ouFace.getId(),interest));
    }

    void SimpleShaper::Enqueue(std::shared_ptr<ndn::Data> data, const nfd::Face &outFace) {
        dataQueue.push_back(std::make_pair(outFace.getId(),data));
    }

    void SimpleShaper::onContact(const nfd::Face &face) {
        //nothing is needed
        rate = getDataRateInBytePerSeconds();
    }

    void SimpleShaper::onContactLost(const nfd::Face &face) {
        int id = face.getId();

        interestQueue.erase(std::remove_if(interestQueue.begin(), interestQueue.end(), [id](InterestQueueEntry e ){
            return e.first == id;
        }),interestQueue.end());


        dataQueue.erase(std::remove_if(dataQueue.begin(), dataQueue.end(), [id](DataQueueEntry e ){
            return e.first == id;
        }),dataQueue.end());

    }

    void SimpleShaper::startInterestSendingLoop() {
        if(interestQueue.size() != 0){
            auto& entry = interestQueue.front();
            double size = entry.second->wireEncode().size();
            auto face = forwarder.getFace(entry.first);
            if(face != nullptr){
                face->sendInterest(*entry.second);
            }
            interestQueue.erase(interestQueue.begin());

            double delay = size/(rate * interestRateFraction) + m_rand->GetValue(0,0.001);

            ns3::Simulator::Schedule(ns3::Seconds(delay),&SimpleShaper::startInterestSendingLoop, this);
            return;
        }
        ns3::Simulator::Schedule(ns3::Seconds(0.1),&SimpleShaper::startInterestSendingLoop, this);
    }

    void SimpleShaper::startDataSendingLoop() {
        if(dataQueue.size() != 0){
            auto& entry = dataQueue.front();
            double size = entry.second->wireEncode().size();
            auto face = forwarder.getFace(entry.first);
            if(face != nullptr){
                face->sendData(*entry.second);
            }else{
                std::cout<<" Error, this should never happen a null face  "<<entry.first <<std::endl;
                std::terminate();
            }
            dataQueue.erase(dataQueue.begin());
            //std::cout<<" Scheduling Data "<<size<<"    sending after "<<size/(rate * (dataRateFraction))<<"    queue "<<dataQueue.size()<<"   mac queue esize "<<getMacQueue()->GetSize()<<std::endl;
            double delay = size/(rate * (dataRateFraction)) + m_rand->GetValue(0,0.001);
            ns3::Simulator::Schedule(ns3::Seconds(delay),&SimpleShaper::startDataSendingLoop, this);
            return;
        }
        ns3::Simulator::Schedule(ns3::Seconds(0.1),&SimpleShaper::startDataSendingLoop, this);
    }
}
