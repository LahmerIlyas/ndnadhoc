//
// Created by lahmer on 28/09/17.
//

#include "EpidimicSummary.h"
#include "../common/tables/SummaryNameTree.h"

namespace nfd{
    namespace fw{
        const Name EpidimicSummary::STRATEGY_NAME("ndn:/localhost/nfd/strategy/epidimicNDNSummary/%FD%01");

        NFD_REGISTER_STRATEGY(EpidimicSummary);

        EpidimicSummary::EpidimicSummary(Forwarder &forwarder, const Name &name ) : AdhocStrategy(forwarder, name) , m_cit(m_nt), m_cdt(m_nt){

        }

        void EpidimicSummary::onContact(const Face &face) {
            m_summaryVectorState[face.getId()] = false;
            m_nct.addContact(face);
            sendRequestForSummaryVector(face.getId());
        }

        void EpidimicSummary::onContactLost(const Face &face) {
            m_summaryVectorState.erase(face.getId());
            m_nct.endContact(face);
        }

        void EpidimicSummary::onInterest(const Interest &interest, const Face& face) {
            if(interest.getName().compare("/adhoc/request/summary") == 0){
                sendSummaryVector(face);
                return;
            }else if(interest.getName().compare("/adhoc/response/summary") == 0){
                //log(" The response of the summary is received " + interest.getAdditionalData());
                m_summaryVectorState[face.getId()] = true;
                onSummaryVectorReceived(face, interest);
                return;
            }

            //we first check the local CDT
            if(m_cdt.isHere(interest.getName()))
                return;

            //we check if the Interest can be satisfied by a local app
            auto can = canTheInterestBeSatisfiedByLocalApp(interest);
            if(can.first){
                can.second->sendInterest(interest);
                return;
            }
            m_cit.insert(interest);
        }

        void EpidimicSummary::onData(const Data &data, const Face& face) {
            m_cdt.insert(data);

        }

        void EpidimicSummary::sendRequestForSummaryVector(int faceId) {
            if(m_summaryVectorState.find(faceId) != m_summaryVectorState.end()){
                if(m_summaryVectorState.find(faceId)->second)
                    return;
            }else{
                return;
            }

            auto summaryRequestInterest = createInterest("/adhoc/request/summary", "");
            m_shaper->Enqueue(summaryRequestInterest, *getForwarder().getFace(faceId));
            ns3::Simulator::Schedule(ns3::Seconds(1.0),&EpidimicSummary::sendRequestForSummaryVector,this,faceId);
        }

        void EpidimicSummary::sendSummaryVector(const Face &face) {
            auto summaryInterest = createInterest("/adhoc/response/summary",m_cdt.getJsonSummary());
            m_shaper->Enqueue(summaryInterest,face);
        }

        void EpidimicSummary::onSummaryVectorReceived(const Face &face, const ndn::Interest& interest) {
            //we send all carried data packets
            m_cit.iterateOverElements([&face, this](citEntry& entry){
               m_shaper->Enqueue(entry.getInterest(),face);
            });

            //we select which data packet to send
            int n = 0;
            SummaryNameTree tree(interest.getAdditionalData());
            m_cdt.iterateOverElements([&n, &tree, &face, this](cdtEntry &entry) {
                if (!tree.lookUpName(entry.getData()->getName())) {
                    m_shaper->Enqueue(entry.getData(), face);
                    n++;
                }
            });

            log(" node sent " + std::to_string(n) + " data  instead of " + std::to_string(m_cdt.size()) +"    cit size is "+ std::to_string(m_cit.size()));

        }
    }
}