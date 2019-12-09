import multiprocessing as mp
import subprocess
import shlex
import yaml
import json
import uuid 
import experimentUtils as eU
#----------------LOAD CONFIG
configPath = './configs/experimentsConfig.yaml'
with open(configPath,'r') as f:
    GlobalConfig = yaml.load(f, Loader=yaml.FullLoader)

#----------------PREPARE INPUTS
inputs = eU.prepareInputs(GlobalConfig)
iterations = GlobalConfig['iterations']

for input in inputs:
    #bruteforce as separate proceses
    p = mp.Process(target = eU.bruteForce,args=(GlobalConfig,input))
    p.start()
    for i in range(GlobalConfig['iterations']):
        #uses all cpu 
        # p = mp.Process(target = eU.hillClimb,args=(GlobalConfig,input))
        # p.start()
        # p = mp.Process(target = eU.tabuSearch,args=(GlobalConfig,input))
        # p.start()
         eU.hillClimb(GlobalConfig,input)
         eU.tabuSearch(GlobalConfig,input)
    
print("finito")

