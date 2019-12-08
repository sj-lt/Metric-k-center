import multiprocessing
import subprocess
import shlex
import json
import uuid 

def call_proc(cmd):
    """ This runs in a separate thread. """
    #subprocess.call(shlex.split(cmd))  # This will block until cmd finishes
    p = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    return (out, err)
def call_run(cmd):
    """ This runs in a separate thread. """
    #subprocess.call(shlex.split(cmd))  # This will block until cmd finishes
    p = subprocess.run(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)

def prepareInputs(GlobalConfig):
    with open(GlobalConfig['input']['baseFile'],'r') as citiesJson:
        citiesDb = json.load(citiesJson)
    inputs = []
    for i in range(GlobalConfig['input']['start'],GlobalConfig['input']['end'],GlobalConfig['input']['shift']):
        print('building cities ' + str(i))
        input = []
        for j in range(i):
            input.append(citiesDb['cities'][j])
        data = {}
        data['cities'] = input
        inputPath = './g_input/input-'+str(i)+'.json'
        inputs.append(inputPath)
        with open(inputPath,'w+') as outFile:
            json.dump(data,outFile)
    return inputs


def createConfig(GlobalConfig,input,method,config):
    if(method != 'bruteForce'):
        config['iterations'] = GlobalConfig['i']
    config['n_wh'] = GlobalConfig['n_wh']
    config['method'] = method
    config['input'] = input
    config['output'] = './output/'+method + GlobalConfig['output'] + '.json'
    config['batch'] = GlobalConfig['batch']
    config['id'] = str(uuid.uuid4())
    return config

def bruteForce(GlobalConfig,input):
    if(GlobalConfig['bruteForce']):
        configPath='./g_config/bruteConfig.json'
        config = {}
        config = createConfig(GlobalConfig,input,'bruteForce',config)
        with open(configPath,'w+') as configFile:
            json.dump(config,configFile)
        out = call_proc(GlobalConfig['pPath']+configPath)
        
def hillClimb(GlobalConfig,input):
    #--------------------DETERMINISTIC
    for i in range(GlobalConfig['hillClimb']['deterministic']['i']):
        configPath='./g_config/hillDetConfig.json'
        config = {}
        config['annealing'] = 'false'
        config['random'] = 'false'
        config = createConfig(GlobalConfig,input,'hillClimb',config)
        with open(configPath,'w+') as configFile:
            json.dump(config,configFile)
        out = call_run(GlobalConfig['pPath']+configPath)
    #--------------------STOCHASTIC
    for i in range(GlobalConfig['hillClimb']['stochastic']['i']):
        configPath='./g_config/hillStoConfig.json'
        config = {}
        config['annealing'] = 'false'
        config['random'] = 'true'
        config = createConfig(GlobalConfig,input,'hillClimb',config)
        with open(configPath,'w+') as configFile:
            json.dump(config,configFile)
        out = call_run(GlobalConfig['pPath']+configPath)
    #--------------------ANNEALING
    for i in range(GlobalConfig['hillClimb']['annealing']['i']):
        configPath='./g_config/hillAnnConfig.json'
        config = {}
        config['annealing'] = 'true'
        config['random'] = 'false'
        if(i==0):
            config['temperature'] = GlobalConfig['hillClimb']['annealing']['t']
        else:
            config['temperature']+=GlobalConfig['hillClimb']['annealing']['t_s']
        config = createConfig(GlobalConfig,input,'hillClimb',config)
        with open(configPath,'w+') as configFile:
            json.dump(config,configFile)
        out = call_run(GlobalConfig['pPath']+configPath)
def tabuSearch(GlobalConfig,input):
    for i in range(GlobalConfig['tabuSearch']['i']):
        configPath='./g_config/tabuConfig.json'
        config = {}
        if(i==0):
            config['tabuSize'] = GlobalConfig['tabuSearch']['t']
        else:
            config['tabuSize']+=GlobalConfig['tabuSearch']['t_s']
        config = createConfig(GlobalConfig,input,'tabuSearch',config)
        with open(configPath,'w+') as configFile:
            json.dump(config,configFile)
        out = call_run(GlobalConfig['pPath']+configPath)