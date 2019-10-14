
#include "problem.hpp"
#include "combinations.cpp"


  double city_t::distance(city_t &c2) {
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



  double solution_t::score(){

	//std::cout<<"start score--------:"<<std::endl;
    double dist=0;
    int n =0,n1=0;
    double tempDist;
    for(auto city : problem->cities){
      bool ifWarehouse = false; 
      for(int i =0;i<warehouses.size();i++){


        if(n==warehouses.at(i))
          ifWarehouse=true;
		
      }
      if(ifWarehouse){
		  n++;
        continue;
		}
      for(int i = 1;i<warehouses.size();i++){
          
                                                                     /**
                                                                         * get the nearest warehouse
                                                                         *  **/
        if(i==1){
			float a,b;

          tempDist=std::min(city.distance(problem->cities.at(warehouses.at(0))),
                                city.distance(problem->cities.at(warehouses.at(1))));
		}
        else{

		
          tempDist=std::min(tempDist,city.distance(problem->cities.at(i)));
		}

	  }
      /**
         * get farthest city from the nearest warehouse
         *  **/
      if(n1==0){
       dist=tempDist ;
	  n1++;
	  }
	dist=std::max(dist,tempDist);
	n++;
    }
	//std::cout<<dist<<std::endl;
    return dist;
  }

  solution_t::solution_t(std::shared_ptr<problem_t> problem_, int numberOfWarehouses_)
    : problem(problem_),
    numberOfWarehouses(numberOfWarehouses_),bestScore(0),
        warehouses(numberOfWarehouses) {
    for (int i = 0; i < numberOfWarehouses_; i++)
      warehouses[i] = i;
  }
    
        

  solution_t::solution_t( std::vector<city_t> input_cities, int numberOfWarehouses_)
      : problem(std::make_shared<problem_t>(problem_t{input_cities})),
      numberOfWarehouses(numberOfWarehouses_),bestScore(0),
        warehouses(numberOfWarehouses_) {
    for (int i = 0; i < numberOfWarehouses_; i++)
      warehouses[i] = i;
  }
  solution_t::solution_t(){};

int main(int argc, char **argv) {
  using namespace std;

  solution_t experiment({{"Warszawa", 52.2330269, 20.7810081},
                         {"Gdansk", 54.3612063, 18.5499431},
                         {"Bydgoszcz", 53.1169002, 17.9008963},
                         {"Poznan", 52.456009, 16.896973}},2);


  chaseSequence combinations = chaseSequence(experiment.problem->cities.size(),2,experiment);

  combinations.init();



 
};