from scenario import Scenario
import string
import random
import os
import sys
from multiprocessing.dummy import Pool as ThreadPool

tempFileLocation = "../temp"              #this is folder where to put temporarely generated mobility files
                                                                    #  and content catalog files

bonnmotion = "../../bonnmotion-3.0.1/bin/bm"          #this is the path to the bonnmotion executable


command = "../build/test"                                     #This is the scenario execution command


numberOfRepititions = 1                                             #How many time you wants each simullation to be repeated
                                                                    #each simulation will use a different seed

'''
    These parameters are for the generation of content catalog file
    The content catalog is a file that register all the available files
    and for each file the number of chunks and the size of each chunk
    ex:
        /adhoc/fzefoplerg/file1             512(chunk)          1024(1 Ko in size for each chunk)

    If the consumer wants to request this file, it will generate 512 Interests with the following names
            /adhoc/fzefoplerg/file1/1
            /adhoc/fzefoplerg/file1/2
            /adhoc/fzefoplerg/file1/3
            .....
            /adhoc/fzefoplerg/file1/512
'''

fileSize = 4 * 1024 * 1024                                  # this is the size of the file

numberOfFiles = 1000                                        # this is the total number of files available in the network
                                                            #consumers can on
dataSizes = [1024*2]


dataBaseLocation = "/home/lahmer/ndnSIM/scenario/results/datasize"      #The simulation results are stored in sqlite database
                                                                        #you shoule provide the path to the database


numberOfRequestedFiles = 3                                              #This is the number of files that each consumer will request

numberOfConsumersVector = [5]                                           #this is vector of parameters that denote the number of consumers
                                                                        #each parameter of this vector will result in a different simulation

numberOfProducers = 1                                                   #this is the number of producers


strategies = ["/localhost/nfd/strategy/epidimicNDN"]#""/localhost/nfd/strategy/SummaryEpidimic"]#,"/localhost/nfd/strategy/epidimicNhops"]




zipfDistributionQParameter = 0.7                                        #This parameters are related to computing the popularity of the content
zipfDistributionSParameter = 0.7


command = "../build/test"                                     #This is the scenario execution command



bufferSize = 20*1024*1024                                               #this is the size limit of the carried Data table


numberOfNode = 30                                                       #total number of nodes in the scenario

simulationAreaHeight = 5000
simulationAreaWidht  = 5000


nodeSpeeds = [10]                                                       # a vector of parameters that contains the maximum speed of nodes

simulationDuration = 80000                                              # the total duration of the simulation

wifiStandard = "802.11g"                                                #the wifi standard, for now we support only 802.11a, 802.11g

wifiDataMode = "ErpOfdmRate54Mbps"                                      #the phy layer bandwidth, each standard have it's own check scenario.py

numberOfThreads = 1                                                     #multiple simulations are executed simultaneously




rand_str = lambda n: ''.join([random.choice(string.lowercase) for i in xrange(
    n)])  # this is a random string generator rand_str(10): generate a string of 10 charachters


def generateCotnentCatalogFile(location="", fileSize=10 * 1024 * 1024, chunkSize=1024, numberOfFiles=1000):
    f = open(location, "w+")
    for i in range(0, numberOfFiles):
        s = "/adhoc/" + rand_str(10) + "/" + rand_str(10) + " " + str(int(fileSize / chunkSize)) + " " + str(
            chunkSize) + "\n"
        f.writelines(s)

    f.close()


def generateRandomWayPointMobilityTrace(nodes="20", width="1500", height="1500", speed="3", duration="50000",
                                        output_direcctory="/home/lahmer/scenario4", seed="100"):
    command = str()
    command += bonnmotion
    command += (" -f " + output_direcctory + " RandomWaypoint ")
    command += (" -n " + nodes)
    command += (" -d " + duration)
    command += (" -x " + width)
    command += (" -y " + height)
    command += (" -h " + speed)
    command += (" -R " + seed)
    print command
    os.system(command)
    ns2Command = str()
    ns2Command += bonnmotion
    ns2Command += " NSFile "
    ns2Command += (" -f " + output_direcctory)
    os.system(ns2Command)

def runScenario(scenario):
    scenario.run()


ScenariosPool = []

for numberOfConsumers in numberOfConsumersVector:
    for nodeSpeed in nodeSpeeds:
        for size in dataSizes:
            for rep in range(0, numberOfRepititions):
                for strategy in strategies:
                    #generating a temp mobility trace*
                    mobilityFileLocation = tempFileLocation + "/" + rand_str(20)
                    generateRandomWayPointMobilityTrace(output_direcctory=mobilityFileLocation,nodes=str(numberOfNode),width=str(simulationAreaWidht)
                                                        ,height=str(simulationAreaHeight),speed=str(nodeSpeed), duration=str(simulationDuration))
                    #generate a temp content catalog file
                    contentCatalogLocation = tempFileLocation +"/" +rand_str(20)
                    generateCotnentCatalogFile(contentCatalogLocation,fileSize=fileSize,chunkSize=size,numberOfFiles=numberOfFiles)
                    s = Scenario()
                    s.command = command
                    s.bufferSize = bufferSize
                    s.contentCatalogPath = contentCatalogLocation
                    s.dataBaseLocation = dataBaseLocation
                    s.mobilityTraceFile = mobilityFileLocation + ".ns_movements"
                    s.numberOfConsumers = numberOfConsumers
                    s.numberOfProducers = numberOfProducers
                    s.numberOfRequestedFiles = numberOfRequestedFiles
                    s.strategy = strategy
                    s.zipfDistributionQParameter = zipfDistributionQParameter
                    s.zipfDistributionSParameter = zipfDistributionSParameter
                    s.wifiStandard = wifiStandard
                    s.wifiMode = wifiDataMode
                    ScenariosPool.append(s)

pool = ThreadPool(numberOfThreads)

results = pool.map(runScenario, ScenariosPool)


pool.close()
pool.join()
