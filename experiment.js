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
const input_folder = `./input/`;
const config_folder = `./config/`;
const fs = require('fs');
const execSync = require('child_process').execSync;


let summary_stats = {};
fs.readdirSync(config_folder).forEach(file => {

        var config = file;
        var i = 0;
        fs.readdirSync(input_folder).forEach(file2 => {

                var input = file2;
                var outputName = config.split(".")
                var output = outputName[0] + "-" + input + "." + outputName[1];
                var command = "./build/kCenterProblem " + "./config/" + config + " ./input/" + input + " ./output/" + output;
                //console.log("./build/kCenterProblem " + "./config/" + config + " ./input/" + input + " ./output/" + output)
                let stdout = execSync(command);
                console.log(stdout)
                i++;
            
        });
    
});
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