//
// Created by lahmer on 28/09/17.
//

#include "EpidimicNDN.h"
#include "../../common/nameTree/Node.h"

namespace nfd{
    namespace fw{
        const Name EpidimicNDN::STRATEGY_NAME("ndn:/localhost/nfd/strategy/epidimicNDN/%FD%01");

        NFD_REGISTER_STRATEGY(EpidimicNDN);

        EpidimicNDN::EpidimicNDN(nfd::Forwarder &forwarder, const ndn::Name &name) : AdhocStrategy(forwarder, name), m_cit(m_nt), m_cdt(m_nt) {


        }

        void EpidimicNDN::onContact(const Face &face) {
            m_nct.addContact(face);
            log(" ############################################################################"" ");
            //m_nct.printDebugInfos();

            m_cit.iterateOverElements([this, &face](citEntry& entry){
                m_shaper->Enqueue(entry.getInterest(), face);
            });

            m_cdt.iterateOverElements([this, &face](cdtEntry& entry){
               m_shaper->Enqueue(entry.getData(),face);
            });

        }

        void EpidimicNDN::onContactLost(const Face &face) {
            m_nct.endContact(face);
        }

        void EpidimicNDN::onInterest(const Interest &interest, const Face& face) {

            //we first check the local CDT
            if(m_cdt.isHere(interest.getName()))
                return;

            //we check if the Interest can be satisfied by a local app
            auto can = canTheInterestBeSatisfiedByLocalApp(interest);
            if(can.first){
                can.second->sendInterest(interest);
                return;
            }
            //we insert in the cit
            m_cit.insert(interest);
        }

        void EpidimicNDN::onData(const Data &data, const Face& face) {
            //log("node received data packet ");
            //This will create a new entry in the nametree and delete any CIT entry available
            m_cdt.insert(data);

            //we don't need to check for an application requesting the interest because it's the goal of the PIT


        }

    }
}
