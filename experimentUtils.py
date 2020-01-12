import multiprocessing
import subprocess
import shlex
import json
import uuid


def call_proc(cmd):
    """ This runs in a separate thread. """
    p = subprocess.Popen(shlex.split(
        cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    return (out, err)


def call_run(cmd):
    """ This runs in a separate thread. """
    p = subprocess.run(shlex.split(
        cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)


def prepareInputs(GlobalConfig):
    with open(GlobalConfig['input']['baseFile'], 'r') as citiesJson:
        citiesDb = json.load(citiesJson)
    inputs = []
    for i in range(GlobalConfig['input']['start'], GlobalConfig['input']['end'], GlobalConfig['input']['shift']):
        print('building cities ' + str(i))
        input = []
        for j in range(i):
            input.append(citiesDb['cities'][j])
        data = {}
        data['cities'] = input
        inputPath = './g_input/input-'+str(i)+'.json'
        inputs.append(inputPath)
        with open(inputPath, 'w+') as outFile:
            json.dump(data, outFile)
    return inputs


def createConfig(GlobalConfig, input, method, config):
    if(method != 'bruteForce'):
        config['iterations'] = GlobalConfig['i']
    config['n_wh'] = GlobalConfig['n_wh']
    config['method'] = method
    config['input'] = input
    config['output'] = './output/'+method + GlobalConfig['output'] + '.json'
    config['batch'] = GlobalConfig['batch']
    config['id'] = str(uuid.uuid4())
    return config


def bruteForce(GlobalConfig, input):
    configGlobal = {}
    configGlobal['logIteration'] = str(
        GlobalConfig['hillClimb']['logIterations'])
    if(GlobalConfig['bruteForce']):
        configPath = './g_config/bruteConfig.json'
        config = configGlobal
        config = createConfig(GlobalConfig, input, 'bruteForce', config)
        with open(configPath, 'w+') as configFile:
            json.dump(config, configFile)
        out = call_proc(GlobalConfig['pPath']+configPath)


def hillClimb(GlobalConfig, input):
    configGlobal = {}
    configGlobal['logIteration'] = str(
        GlobalConfig['hillClimb']['logIterations'])

    # --------------------DETERMINISTIC
    for i in range(GlobalConfig['hillClimb']['deterministic']['i']):
        configPath = './g_config/hillDetConfig.json'
        config = configGlobal
        config['annealing'] = 'false'
        config['random'] = 'false'
        config = createConfig(GlobalConfig, input, 'hillClimb', config)
        with open(configPath, 'w+') as configFile:
            json.dump(config, configFile)
        out = call_run(GlobalConfig['pPath']+configPath)
    # --------------------STOCHASTIC
    for i in range(GlobalConfig['hillClimb']['stochastic']['i']):
        configPath = './g_config/hillStoConfig.json'
        config = configGlobal
        config['annealing'] = 'false'
        config['random'] = 'true'
        config = createConfig(GlobalConfig, input, 'hillClimb', config)
        with open(configPath, 'w+') as configFile:
            json.dump(config, configFile)
        out = call_run(GlobalConfig['pPath']+configPath)
    # --------------------ANNEALING
    for i in range(GlobalConfig['hillClimb']['annealing']['i']):
        configPath = './g_config/hillAnnConfig.json'
        config = configGlobal
        config['annealing'] = 'true'
        config['random'] = 'false'
        if(i == 0):
            config['temperature'] = GlobalConfig['hillClimb']['annealing']['t']
        else:
            config['temperature'] = GlobalConfig['hillClimb']['annealing']['t'] + \
                i*GlobalConfig['hillClimb']['annealing']['t_s']
        config = createConfig(GlobalConfig, input, 'hillClimb', config)
        with open(configPath, 'w+') as configFile:
            json.dump(config, configFile)
        out = call_run(GlobalConfig['pPath']+configPath)


def tabuSearch(GlobalConfig, input):
    configGlobal = {}
    configGlobal['logIteration'] = str(
        GlobalConfig['tabuSearch']['logIterations'])
    for i in range(GlobalConfig['tabuSearch']['i']):
        configPath = './g_config/tabuConfig.json'
        config = configGlobal
        if(i == 0):
            config['tabuSize'] = GlobalConfig['tabuSearch']['t']
        else:
            config['tabuSize'] = GlobalConfig['tabuSearch']['t'] + \
                i*GlobalConfig['tabuSearch']['t_s']
        config = createConfig(GlobalConfig, input, 'tabuSearch', config)
        with open(configPath, 'w+') as configFile:
            json.dump(config, configFile)
        out = call_run(GlobalConfig['pPath']+configPath)


def geneticAlgorithm(GlobalConfig, input):
    configGlobal = {}
    configGlobal['logIteration'] = str(
        GlobalConfig['geneticAlgorithm']['logIterations'])
    for fitFunc in GlobalConfig['geneticAlgorithm']['fitFunc']:
        for mutFunc in GlobalConfig['geneticAlgorithm']['mutFunc']:
            for selFunc in GlobalConfig['geneticAlgorithm']['selFunc']:
                for crosFunc in GlobalConfig['geneticAlgorithm']['crosFunc']:
                    for termFunc in GlobalConfig['geneticAlgorithm']['termFunc']:
                        for i in range(GlobalConfig['geneticAlgorithm']['i']):
                            configPath = './g_config/gaConfig.json'
                            config = configGlobal
                            config['fitFunc']=fitFunc
                            config['mutFunc']=mutFunc
                            config['selFunc']=selFunc
                            config['crosFunc']=crosFunc
                            config['termFunc']=termFunc
                            if(i == 0):
                                config['initPopulation'] = GlobalConfig['geneticAlgorithm']['initPop']
                            else:
                                config['initPopulation'] = GlobalConfig['geneticAlgorithm']['initPop'] + \
                                    i*GlobalConfig['geneticAlgorithm']['initPopShift']
                            if(GlobalConfig['geneticAlgorithm']['probabilityExperiment']=='true'):
                                crosProb=GlobalConfig['geneticAlgorithm']['crosProbMin']
                                crosProbMax=GlobalConfig['geneticAlgorithm']['crosProbMax']
                                mutProb=GlobalConfig['geneticAlgorithm']['mutProbMin']
                                mutProbMax=GlobalConfig['geneticAlgorithm']['mutProbMax']
                                config['crosProb']=crosProb
                                config['mutProb']=mutProb
                                while(mutProb<mutProbMax):
                                    while(crosProb<crosProbMax):
                                        config['crossoverProbability']=crosProb      
                                        config['mutationProbability']=mutProb
                                        config = createConfig(
                                            GlobalConfig, input, 'geneticAlgorithm', config)
                                        with open(configPath, 'w+') as configFile:
                                            json.dump(config, configFile)
                                        out = call_run(GlobalConfig['pPath']+configPath)
                                        crosProb+=GlobalConfig['geneticAlgorithm']['probabilityShift']
                                    mutProb+=GlobalConfig['geneticAlgorithm']['probabilityShift']

                            else:   
                                config['crossoverProbability']=GlobalConfig['geneticAlgorithm']['crosProb']      
                                config['mutationProbability']=GlobalConfig['geneticAlgorithm']['mutProb']      
                                config = createConfig(
                                    GlobalConfig, input, 'geneticAlgorithm', config)
                                with open(configPath, 'w+') as configFile:
                                    json.dump(config, configFile)
                                out = call_run(GlobalConfig['pPath']+configPath)
