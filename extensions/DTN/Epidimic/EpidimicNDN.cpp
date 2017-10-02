//
// Created by lahmer on 28/09/17.
//

#include "EpidimicNDN.h"

namespace nfd{
    namespace fw{
        const Name EpidimicNDN::STRATEGY_NAME("ndn:/localhost/nfd/strategy/epidimicNDN/%FD%01");

        NFD_REGISTER_STRATEGY(EpidimicNDN);

        EpidimicNDN::EpidimicNDN(nfd::Forwarder &forwarder, const ndn::Name &name) : AdhocStrategy(forwarder, name) {


        }

        void EpidimicNDN::onContact(const Face &face) {
            m_nct.addContact(face);
            //we send all Interests in the CIT
            log( " On contact is called ");
            for(auto& it : m_cit){
                m_shaper->Enqueue(it.second.getInterest(), face);
            }
            //we send all data packets
            for(auto& it : m_cdt){
                m_shaper->Enqueue(it.second.getData(), face);
            }
        }

        void EpidimicNDN::onContactLost(const Face &face) {
            m_nct.endContact(face);
        }

        void EpidimicNDN::onInterest(const Interest &interest, const Face& face) {
            //is there a carried data packet in the local cdt
            if(m_cdt.isHere(interest.getName()))
                return;

            //first we check if the Interest can be satisfied by a local app
            auto can = canTheInterestBeSatisfiedByLocalApp(interest);
            if(can.first){
                can.second->sendInterest(interest);
                return;
            }


            //we insert in the cit
            m_cit.insert(interest);
        }

        void EpidimicNDN::onData(const Data &data, const Face& face) {
            log("node received data packet ");
            //we first remove any pending Interest with the same name as the received Data packet
            m_cit.remove(data);

            m_cdt.insert(data);



        }

    }
}
