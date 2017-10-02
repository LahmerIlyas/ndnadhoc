cmake_minimum_required(VERSION 3.6)
project(scenario)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(
        /usr/local/include/ns3-dev/ns3/ndnSIM/NFD/
        /usr/local/include/ns3-dev/
        /usr/local/include/ns3-dev/ns3/ndnSIM/NFD/daemon/
        /usr/local/include/ns3-dev/ns3/ndnSIM/NFD/core/
        /usr/local/include/ndn-cxx/
        /usr/local/include/ns3-dev/ns3/ndnSIM/
)


set(SOURCE_FILES
        extensions/DTN/common/tables/cdt.cpp
        extensions/DTN/common/tables/cdt.h
        extensions/DTN/common/tables/cdtEntry.cpp
        extensions/DTN/common/tables/cdtEntry.h
        extensions/DTN/common/tables/cit.cpp
        extensions/DTN/common/tables/cit.h
        extensions/DTN/common/tables/citEntry.cpp
        extensions/DTN/common/tables/citEntry.h
        extensions/DTN/common/tables/NodeContactTable.cpp
        extensions/DTN/common/tables/NodeContactTable.h
        extensions/DTN/common/tables/NodeContactTableEntry.cpp
        extensions/DTN/common/tables/NodeContactTableEntry.h
        extensions/DTN/common/tables/SummaryNameTree.cpp
        extensions/DTN/common/tables/SummaryNameTree.h
        extensions/common/GenericScenario.cpp extensions/common/GenericScenario.h
        extensions/common/WifiStackHelper.cpp extensions/common/WifiStackHelper.h
        extensions/common/AdditionalLayer.cpp extensions/common/AdditionalLayer.h
        extensions/common/WifiNetDeviceTransport.cpp
        extensions/common/WifiNetDeviceTransport.h
        extensions/common/BonnmotionScenario.cpp
        extensions/common/BonnmotionScenario.h
        extensions/DTN/common/AdhocStrategy.cpp
        extensions/DTN/common/AdhocStrategy.h
        "scenarios/test.cpp"
        extensions/DTN/Epidimic/EpidimicNDN.cpp
        extensions/DTN/Epidimic/EpidimicNDN.h
        extensions/DTN/EpidimicSummary/EpidimicSummary.cpp
        extensions/DTN/EpidimicSummary/EpidimicSummary.h
        extensions/DTN/common/AbstractShaper.cpp
        extensions/DTN/common/AbstractShaper.h
        extensions/DTN/common/SimplShaper.cpp
        extensions/DTN/common/SimplShaper.h
        )

add_executable(scenario ${SOURCE_FILES})
