//
// Created by lahmer on 9/27/17.
//

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/energy-module.h"
#include "ns3/ndnSIM-module.h"
#include "../extensions/common/BonnmotionScenario.h"


using namespace std;
namespace ns3 {

    int main(int argc, char* argv[])
    {
        //test this
        Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("2200"));
        Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("2200"));
        BonnmotionScenario scenario(argc, argv);
        scenario.setup();
        return 0;
    }

}

int
main(int argc, char* argv[])
{
    return ns3::main(argc, argv);
}