from subprocess import call
from enum import Enum

class WifiMode_802_11_A(Enum):
    OfdmRate6Mbps   = "OfdmRate6Mbps"
    OfdmRate9Mbps   = "OfdmRate9Mbps"
    OfdmRate12Mbps  = "OfdmRate12Mbps"
    OfdmRate18Mbps  = "OfdmRate18Mbps"
    OfdmRate24Mbps  = "OfdmRate24Mbps"
    OfdmRate36Mbps  = "OfdmRate36Mbps"
    OfdmRate48Mbps  = "OfdmRate48Mbps"
    OfdmRate54Mbps  = "OfdmRate54Mbps"

class WifiMode_802_11_G(Enum):
    DsssRate1Mbps       = "DsssRate1Mbps"
    DsssRate2Mbps       = "DsssRate2Mbps"
    DsssRate5_5Mbps     = "DsssRate5_5Mbps"
    ErpOfdmRate6Mbps    = "ErpOfdmRate6Mbps"
    ErpOfdmRate9Mbps    = "ErpOfdmRate9Mbps"
    DsssRate11Mbps      = "DsssRate11Mbps"
    ErpOfdmRate12Mbps   = "ErpOfdmRate12Mbps"
    ErpOfdmRate18Mbps   = "ErpOfdmRate18Mbps"
    ErpOfdmRate24Mbps   = "ErpOfdmRate24Mbps"
    ErpOfdmRate36Mbps   = "ErpOfdmRate36Mbps"
    ErpOfdmRate48Mbps   = "ErpOfdmRate48Mbps"
    ErpOfdmRate54Mbps   = "ErpOfdmRate54Mbps"


class WifiStandards(Enum):
    Standard_802_11_A = str("802.11a")
    Standard_802_11_G = str("802.11g")



class Scenario:
    dataBaseLocation = "/home/lahmer/"

    numberOfRequestedFiles = 3

    numberOfConsumers = 1

    numberOfProducers = 1

    wifiMode = str(WifiMode_802_11_A.OfdmRate6Mbps)

    mobilityTraceFile = "/home/lahmer/ndnSIM/bonnmotion-3.0.1/bin/scenario1.ns_movements"

    strategy = "/localhost/nfd/strategy/epidimicAdhoc"

    strategyFrequency = 1000

    zipfDistributionQParameter = 0.7

    zipfDistributionSParameter = 0.7

    contentCatalogPath = "/home/lahmer/ndnSIM/scenario/catalog.txt"

    command = "../build/speed-scenario"

    bufferSize = 100

    wifiStandard = WifiStandards.Standard_802_11_G

    def run(self):
        c = []
        c.append(self.command)
        c.append("--traceFile=" + self.mobilityTraceFile)
        c.append("--contentCatalogPath=" + self.contentCatalogPath)
        c.append("--Qparamater=" + str(self.zipfDistributionQParameter))
        c.append("--Sparamater=" + str(self.zipfDistributionSParameter))
        c.append("--database=" + self.dataBaseLocation)
        c.append("--numberOfClients=" + str(self.numberOfConsumers))
        c.append("--numberOfProducers=" + str(self.numberOfProducers))
        c.append("--numberOfRequestedFiles=" + str(self.numberOfRequestedFiles))
        c.append("--strategy=" + self.strategy)
        c.append("--rate=DsssRate5_5Mbps")
        c.append("--bufferSize="+ str(self.bufferSize))
        c.append("--standard=802.11g")
        print c
        call(c)


    def __init__(self):
        print " Initializing a command "
