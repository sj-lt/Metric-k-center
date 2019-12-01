var today = new Date();
var dd = today.getDate();
var mm = today.getMonth() + 1; //January is 0!

var yyyy = today.getFullYear();
if (dd < 10) {
    dd = '0' + dd;
}
if (mm < 10) {
    mm = '0' + mm;
}
var today = dd + '-' + mm + '-' + yyyy;



const experiment_folder = `./output/${today}/`;
const generated_input_folder = `./g_input/`;
const config_folder = `./config/`;
const fs = require('fs');
const execSync = require('child_process').execSync;


const config = require('./experimentConfig.json');
const citiesDb = require('./input/citiesTest6Csv.json');
console.log(config)
for (let i = config["num_cities_b"]; i < config["num_cities_e"]; i++) {

    console.log("build cities" + i)
    let input = []
    for (let j = 0; j < i; j++) {
        input.push(citiesDb['cities'][j])
    }
    let data = JSON.stringify({ 'cities': input });
    fs.writeFileSync('./g_input/input-' + i + '.json', data);
}

for (let ix = 0; ix < config["iterations"]; ix++) {
    for (let x = config["num_cities_b"]; x < config["num_cities_e"]; x++) {
            //console.log(x);

            fs.readdirSync(config_folder).forEach(file => {

                var configName = file;
                let methodConfig = require(config_folder + configName);

                //----------------------------SETUP-AND-SAVE-EXPERIMETNS-CONFIG------------------------------
                if(methodConfig["method"] == "tabuSearch"){
                    methodConfig["tabuSize"] = config["tabuToAddExperiment"];
                    var research=config["tabuExperiment"];
                }
                else if(methodConfig["method"] == "hillClimb"){
                    methodConfig["temperature"] = config["hillTempToAddExperiment"];
                    var research=config["hillExperiment"];
                }
                let dataJson = JSON.stringify(methodConfig);
                fs.writeFileSync(config_folder + configName, dataJson);


                do{
                    //console.log("expoeriments" + x)
                    let methodConfig = require(config_folder + configName);
                    const uuidv4 = require('uuid/v4');
                    //----------------------------EXTRA-EXPERIMENTS------------------------------
                    if(methodConfig["method"] == "tabuSearch" && config["tabuAdditionalExperiments"]=="true"){
                        var a = methodConfig["tabuSize"];
                        var b = config["tabuToAddExperiment"];
                        methodConfig["tabuSize"] = a+b;
                        research=research-1;
                    }
                    else if(methodConfig["method"] == "hillClimb"&& config["hillAdditionalExperiments"]=="true"){
                        var a = methodConfig["temperature"];
                        var b = config["hillTempToAddExperiment"];
                        methodConfig["temperature"] = a+b;
                        research=research-1;
                    }
                    else{research = 0;} 
                    //----------------------------SETUP-AND-SAVE-CONFIG------------------------------
                    methodConfig["id"] = uuidv4();
                    methodConfig["batch"] = config["batch"];
                    methodConfig["input"] = './g_input/input-' + x + '.json';
                    let dataJson = JSON.stringify(methodConfig);
                    fs.writeFileSync(config_folder + configName, dataJson);
                    //--------------------------------RUN-----------------------------------
                    var command = "./build/kCenterProblem " + "./config/" + configName;
                    let stdout = execSync(command);
                    //console.log(stdout)
                
                }while(research>0)

            });
    }
}
/*
//MAYBE MAKE JSON PREETY AGAIN
var fs = require('fs');

var myData = {
  name:'test',
  version:'1.0'
}

var outputFilename = '/tmp/my.json';

fs.writeFile(outputFilename, JSON.stringify(myData, null, 4), function(err) {
    if(err) {
      console.log(err);
    } else {
      console.log("JSON saved to " + outputFilename);
    }
}); */