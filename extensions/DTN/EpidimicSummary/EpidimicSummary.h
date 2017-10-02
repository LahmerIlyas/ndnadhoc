//
// Created by lahmer on 28/09/17.
//

#ifndef SCENARIO_EPIDIMICSUMMARY_H
#define SCENARIO_EPIDIMICSUMMARY_H
#include "../common/AdhocStrategy.h"

namespace nfd
{
    namespace fw {


        class EpidimicSummary : public AdhocStrategy{

        public:
            EpidimicSummary(Forwarder &forwarder, const Name &name = EpidimicSummary::STRATEGY_NAME);

            virtual void onContact(const Face &face);

            virtual void onContactLost(const Face &face);

            virtual void onInterest(const Interest &interest, const Face& face);

            virtual void onData(const Data &data, const Face& face);



        public:
            static const Name STRATEGY_NAME;

        };
    }
}

#endif //SCENARIO_EPIDIMICSUMMARY_H
