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
            console.log(x)
        fs.readdirSync(config_folder).forEach(file => {
            console.log("expoeriments" + x)
            var config = file;
            let configJson = require(config_folder + config);
            const uuidv4 = require('uuid/v4');

            configJson["id"] = uuidv4();
            configJson["input"] = './g_input/input-' + x + '.json';
            let dataJson = JSON.stringify(configJson);
            fs.writeFileSync(config_folder + config, dataJson);

            var command = "./build/kCenterProblem " + "./config/" + config;
            //console.log("./build/kCenterProblem " + "./config/" + config )
            let stdout = execSync(command);
            console.log(stdout)

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