//
// Created by lahmer on 28/09/17.
//

#ifndef SCENARIO_EPIDIMICSUMMARY_H
#define SCENARIO_EPIDIMICSUMMARY_H
#include "../common/AdhocStrategy.h"
#include "../common/tables/NodeContactTable.h"
#include "../common/tables/cdt.h"
#include "../common/tables/cit.h"

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

            void sendRequestForSummaryVector(int faceId);

            void sendSummaryVector(const Face& face);

            void onSummaryVectorReceived(const Face& face, const ndn::Interest& interest);

        public:
            static const Name STRATEGY_NAME;

            NodeContactTable m_nct;

            ::cit m_cit;

            ::cdt m_cdt;

            ::NameTree m_nt;

            std::map<int, bool> m_summaryVectorState;           //holds a state that denote if a summary vector is received or not yet
        };
    }
}

#endif //SCENARIO_EPIDIMICSUMMARY_H
