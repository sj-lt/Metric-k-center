
#include "problem.hpp"
#include "combinations.cpp"
#include "hillClimber.cpp"
#include "tabuSearcher.cpp"
#include "skel.cpp"
#define WAREHOUSES 2

double city_t::distance(city_t &c2)
{
	using namespace std;
	auto &[name2, lon2, lat2] = c2;
	auto R = 6371e3; // metres
	auto fi1 = latitude * M_PI / 180.0;
	auto fi2 = lat2 * M_PI / 180.0;
	auto deltafi = (lat2 - latitude) * M_PI / 180.0;
	auto deltalambda = (lon2 - longitude) * M_PI / 180.0;
	auto a = sin(deltafi / 2) * sin(deltafi / 2) +
			 cos(fi1) * cos(fi2) * sin(deltalambda / 2) * sin(deltalambda / 2);
	auto c = 2 * atan2(sqrt(a), sqrt(1 - a));
	auto d = R * c;
	return d;
}
double solution_t::getBestScoreKm()
{
	return bestScore / 1000;
}
double solution_t::score()
{ /*
@brief
*calculates score of cities and warehouses stored in class
*@returns double score of warehouse configuration
**/

	//std::cout<<"start score--------:"<<std::endl;
	double dist ;
	double tempDist;

	int n = 0, n1 = 0;
	//for (city_t city : problem->cities)
	for(int h=0 ;h<problem->cities.size();h++)
	{
		bool ifWarehouse = false;
		for (auto w : warehouses)
		{
			if (n == w){
				
				ifWarehouse = true;}
		}
		if (ifWarehouse)
		{
			n++;
			continue;
		}
		else
		{
			for (int i = 1; i < warehouses.size(); i++)
			{
				/**
                                                                         * get the nearest warehouse
                                                                         *  **/
				if (i == 1)
				{
					//float a, b;
					//a=city.distance(problem->cities.at(warehouses.at(0)));
					//b=city.distance(problem->cities.at(warehouses.at(1)));
					//std::cout<<"distance:"<<city.name<<problem->cities.at(warehouses.at(0)).name<<"  "<<a<<std::endl;
					//std::cout<<"distance:"<<city.name<<problem->cities.at(warehouses.at(1)).name<<"  "<<b<<std::endl;
					//tempDist = std::min(a,b	);
					tempDist = std::min(problem->cities.at(h).distance(problem->cities.at(warehouses.at(0))),problem->cities.at(h).distance(problem->cities.at(warehouses.at(1)))	);
				}
				else
				{
					//std::cout<<"distance:"<<city.name<<problem->cities.at(warehouses.at(0)).name<<"  "<<a<<std::endl;
					tempDist = std::min(tempDist, problem->cities.at(h).distance(problem->cities.at(warehouses.at(i))));
				}
			}
			/**
         * get farthest city from the nearest warehouse
         *  **/
			if (n1 == 0)
			{
				dist = tempDist;
				n1++;
			}
			dist = std::max(dist, tempDist);
			n++;
		}
	}
	//std::cout<<dist<<std::endl;
	return dist;
}

std::vector<city_t> solution_t::getCitySolution()
{
	std::vector<city_t> warehouseCities;
	for (int i = 0; i < warehouses.size(); i++)
	{
		warehouseCities.push_back(problem->cities.at(warehouses.at(i)));
	}
	return warehouseCities;
}

solution_t::solution_t(std::shared_ptr<problem_t> problem_, nlohmann::json config_json)
	: problem(problem_),
	  config_json (config_json),
	  numberOfWarehouses(config_json["numberOfWarehouses"]), bestScore(0),
	  warehouses((int)config_json["numberOfWarehouses"])
{
	for (int i = 0; i < (int)config_json["numberOfWarehouses"]; i++)
		warehouses[i] = i;
}

solution_t::solution_t(std::vector<city_t> input_cities, nlohmann::json config_json)
	: problem(std::make_shared<problem_t>(problem_t{input_cities})),
	  config_json (config_json),
	  numberOfWarehouses(config_json["numberOfWarehouses"]), bestScore(0),
	  warehouses((int)config_json["numberOfWarehouses"])
{
	for (int i = 0; i < (int)config_json["numberOfWarehouses"]; i++)
		warehouses[i] = i;
}

solution_t::solution_t(nlohmann::json config_json)
	:numberOfWarehouses((int)config_json["numberOfWarehouses"]),
	config_json (config_json),
	bestScore(0),
	warehouses((int)config_json["numberOfWarehouses"])
{
	for (int i = 0; i < (int)config_json["numberOfWarehouses"]; i++)
		warehouses[i] = i;
}

solution_t::solution_t(){};

std::ostream &operator<<(std::ostream &s, solution_t &sol)
{
	using json = nlohmann::json;
	json j = {};
	auto warehousesAsCities = sol.getCitySolution();

	for (auto city : warehousesAsCities)
	{
		j["/cities/-"_json_pointer] = {city.name, city.longitude, city.latitude};
	}
	j["score"] = sol.getBestScoreKm();
	s << j;

	return s;
}

std::istream &operator>>(std::istream &s, solution_t &sol)
{
	std::cout<<sol.config_json["numberOfWarehouses"]<<std::endl;
	nlohmann::json sol_json;
	sol_json = sol_json.parse(s);
	
	std::vector<city_t> cities;
	for (auto element : sol_json["cities"])
	{
		city_t c{element[0], element[2], element[1]};
		cities.push_back(c);
	}
	solution_t solnew(cities, sol.config_json);
	sol = solnew;
	return s;
}
/*std::istream &operator>>(std::istream &s, nlohmann::json &config_json)
{
	nlohmann::json config_json;
	config_json = config_json.parse(s);
	return s;
}*/