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
    p = mp.Process(target = eU.bruteForce,args=(GlobalConfig,input))
    p.start()
    for i in range(GlobalConfig['iterations']):
        i=1
        eU.hillClimb(GlobalConfig,input)
        eU.tabuSearch(GlobalConfig,input)
    
print("finito")







#pool = ThreadPool(multiprocessing.cpu_count())
#results = []

#results.append(pool.apply_async(call_proc, ("./combine" + arguments,)))

# Close the pool and wait for each running task to complete
#pool.close()
#pool.join()
# for result in results:
#     out, err = result.get()
#     print("out: {} err: {}".format(out, err))
# subprocess.call("./merge_resized_images")

