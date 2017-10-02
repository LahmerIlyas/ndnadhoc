//
// Created by lahmer on 28/09/17.
//

#include "EpidimicSummary.h"
namespace nfd{
    namespace fw{
        const Name EpidimicSummary::STRATEGY_NAME("ndn:/localhost/nfd/strategy/epidimicNDNSummary/%FD%01");

        NFD_REGISTER_STRATEGY(EpidimicSummary);

        EpidimicSummary::EpidimicSummary(Forwarder &forwarder, const Name &name ) : AdhocStrategy(forwarder, name) {

        }

        void EpidimicSummary::onContact(const Face &face) {
        }

        void EpidimicSummary::onContactLost(const Face &face) {

        }

        void EpidimicSummary::onInterest(const Interest &interest, const Face& face) {
            log( " Summary on Interest is called ");
        }

        void EpidimicSummary::onData(const Data &data, const Face& face) {


        }
    }
}