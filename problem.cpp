
#include "problem.hpp"
#include "combinations.cpp"
#include "hillClimber.cpp"
#include "tabuSearcher.cpp"
#include "genetic.cpp"
#include "skel.cpp"
/*
*@returns distance beetwen cities
**/
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

/*
@brief
*calculates score of cities and warehouses stored in class
*@returns double score of warehouse configuration
**/
double solution_t::score()
{ 

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
			if (n == w)
				ifWarehouse = true;
		}
		if (ifWarehouse)
		{
			n++;	continue;
		}
		else
		{
			for (int i = 1; i < warehouses.size(); i++)
			{
                                                                         //get the nearest warehouse
				if (i == 1)
					tempDist = std::min(problem->cities.at(h).distance(problem->cities.at(warehouses.at(0))),problem->cities.at(h).distance(problem->cities.at(warehouses.at(1)))	);
				else
					tempDist = std::min(tempDist, problem->cities.at(h).distance(problem->cities.at(warehouses.at(i))));
			}
         												//get farthest city from the nearest warehouse
			if (n1 == 0)
			{
				dist = tempDist;	n1++;
			}
			dist = std::max(dist, tempDist);	n++;
		}
	}
	return dist;
}
/*
*@returns score in km
**/
double solution_t::getBestScoreKm()
{
	return bestScore / 1000;
}
/*
*@brief decode warehouses to cities
*@returns list of city_t
**/
std::vector<city_t> solution_t::getCitySolution()
{
	std::vector<city_t> warehouseCities;
	for (int i = 0; i < warehouses.size(); i++)
	{
		warehouseCities.push_back(problem->cities.at(warehouses.at(i)));
	}
	return warehouseCities;
}
//------------------------------------------CONSTRUCTORS-------------------------------------------

solution_t::solution_t(std::shared_ptr<problem_t> problem_, nlohmann::json config_json)
	: problem(problem_),
	  config_json (config_json),
	  numberOfWarehouses(config_json["n_wh"]), bestScore(0),
	  warehouses((int)config_json["n_wh"])
{
	for (int i = 0; i < (int)config_json["n_wh"]; i++)
		warehouses[i] = i;
}

solution_t::solution_t(std::vector<city_t> input_cities, nlohmann::json config_json)
	: problem(std::make_shared<problem_t>(problem_t{input_cities})),
	  config_json (config_json),
	  numberOfWarehouses(config_json["n_wh"]), bestScore(0),
	  warehouses((int)config_json["n_wh"])
{
	for (int i = 0; i < (int)config_json["n_wh"]; i++)
		warehouses[i] = i;
}

solution_t::solution_t(nlohmann::json config_json)
	:numberOfWarehouses((int)config_json["n_wh"]),
	config_json (config_json),
	bestScore(0),
	warehouses((int)config_json["n_wh"])
{
	for (int i = 0; i < (int)config_json["n_wh"]; i++)
		warehouses[i] = i;
}

solution_t::solution_t(){};
//------------------------------------------OPERATORS-------------------------------------------
/*
@brief
*writes solution do file
**/
std::ostream &operator<<(std::ostream &s, solution_t &sol)
{
	using json = nlohmann::json;
	json j={} ;


	j["TotalBestscore"] = sol.getBestScoreKm();
	j["timeSpend"] = sol.timeTaken;
	j["config"] = sol.config_json;
	j["summary"] = "true";
	s << j;

	return s;
}
/*
@brief
*read cities from json
**/
std::istream &operator>>(std::istream &s, solution_t &sol)
{
	std::cout<<sol.config_json["n_wh"]<<std::endl;
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
