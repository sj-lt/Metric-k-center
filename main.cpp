#include "problem.cpp"

/*
	build --->  
				mkdir build | mkdir g_input | mkdir g_config | mkdir output | g++ main.cpp -std=c++17 -o ./build/kCenterProblem	
	run brute --> 
				./build/kCenterProblem ./config/bruteConfig.json 
	run hill  --> 
				./build/kCenterProblem ./config/hillConfig.json 
	run tabu  --> 
				./build/kCenterProblem ./config/tabuConfig.json 
*/
std::map<std::string, std::function<solver_t*(solution_t)>> generate_methods_map() ;
void visualize(solution_t *problem);
int main(int argc, char **argv)
{
	using namespace std;
	nlohmann::json config_json;

  	map<string,function<solver_t*(solution_t)>> 
		methodMap = generate_methods_map();


//Read config
	if (argc > 1) {
		ifstream is(argv[1]); // open file
		config_json = nlohmann::json::parse(is);
	} else {
		config_json = nlohmann::json::parse(cin);
	}
	solution_t experiment(config_json);

//Read input data
	if (experiment.config_json["input"].is_string()) {
		ifstream is(experiment.config_json["input"]); // open file
		is >> experiment;
	} else {
		cin >> experiment;
	}
//load solver from configs
	solver_t *combinations;
	try{
		cout<<"Trying to start: "<<config_json["method"]<<endl;
	combinations = methodMap[config_json["method"]](experiment);	//throwing an instance of 'std::bad_function_call' if wrong params
	
	}
	catch(const bad_function_call& e){
		cout<<e.what()<<endl;
		cout<<"----------Probably wrong method argument specified----------"<<endl;
		cout<<"-------------------check your config file-------------------"<<endl;

	}

	auto duration = combinations->calculate();
	cout << "duration: " << duration <<" seconds"<< endl;
	cout << "best score: " << combinations->problem_.getBestScoreKm() << endl;
	cout<<"number of cities"<<combinations->problem_.problem->cities.size()<<endl;

//Save or print to stdout
	solution_t result;
	result = combinations->problem_;

	using json = nlohmann::json;
	auto warehousesAsCities = result.getCitySolution();

	for (auto city : warehousesAsCities)
	{
	json jx ={};
	
		jx["solution"]["location"] =  {city.longitude,city.latitude};
		jx["config"] =  result.config_json;
		
		combinations->logger(jx);
	}
		for (auto city : result.problem->cities)
	{
	json jx ={};

		jx["problem"]["location"] =  {city.longitude,city.latitude};
		jx["config"] =  result.config_json;
		combinations->logger(jx);

	}


	if (experiment.config_json["output"].is_string()) {
		ofstream os(experiment.config_json["output"], std::ios_base::out | std::ios_base::app); // open file
		os << result << '\n';
	} else {
		cout << result << endl;
	}

	visualize(&combinations->problem_);
  
};

std::map<std::string, std::function<solver_t*(solution_t)>> generate_methods_map() {
  using namespace std;
	map<string,std::function<solver_t*(solution_t)>> methodMap;

	methodMap["bruteForce"] = [](auto experiment) {
    return new chaseSequence(experiment);
  };
  	methodMap["hillClimb"] = [](auto experiment) {
    return new hillClimber(experiment);
  };
  	methodMap["tabuSearch"] = [](auto experiment) {
    return new tabuSearcher(experiment);
  };
  return methodMap;
}


void visualize(solution_t *problem){
  std::ofstream htmlout("vis.html");
    htmlout << html_header;

    for (auto city : problem->problem->cities) {
        htmlout << html_marker_start;
        htmlout << city.latitude << ",";
        htmlout << city.longitude << std::endl;
        htmlout << html_marker_end;

    }
    for (auto city : problem->getCitySolution()) {
        htmlout << html_circle_start;
        htmlout << city.latitude << ",";
        htmlout << city.longitude<<"],{radius: "<<problem->bestScore << std::endl;
        htmlout << html_circle_end;
            //50.5, 30.5], {radius: 200 to add beetween

    }
  htmlout << html_footer;
  htmlout.close();
};


