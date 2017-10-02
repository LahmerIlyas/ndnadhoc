//
// Created by lahmer on 28/09/17.
//

#ifndef SCENARIO_EPIDIMICNDN_H
#define SCENARIO_EPIDIMICNDN_H

#include "../common/AdhocStrategy.h"
#include "../common/tables/NodeContactTable.h"
#include "../common/tables/cit.h"
#include "../common/tables/cdt.h"



namespace nfd
{
    namespace fw {



        class EpidimicNDN : public AdhocStrategy{
        public:
            EpidimicNDN(Forwarder &forwarder, const Name &name = EpidimicNDN::STRATEGY_NAME);

            virtual void onContact(const Face &face);

            virtual void onContactLost(const Face &face);

            virtual void onInterest(const Interest &interest, const Face& face);

            virtual void onData(const Data &data, const Face& face);






        public:
            static const Name STRATEGY_NAME;

            NodeContactTable m_nct;

            ::cit m_cit;

            ::cdt m_cdt;


        };
    }
}

#endif //SCENARIO_EPIDIMICNDN_H
