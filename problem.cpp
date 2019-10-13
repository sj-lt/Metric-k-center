#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#define STARTDIST 1000
/**
 * @brief class representing single city on the map
 */
class city_t {
public:
  std::string name;
  double longitude;
  double latitude;

  /**
   * @brief calculate distance between two cities
   *
   * @param c2 second city to calculate distance to
   * @return double distance on the planet Earth
   */
  double distance(city_t &c2) {
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
};

/**
 * @brief the problem definition
 *
 * The cities .
 */
class problem_t {
public:
  /**
   * @brief list of the cities .
   *
   * This is the default order - [1,2,3, ...].
   */
  std::vector<city_t> cities;
  int numberOfWarhouse;
};

/**
 * @brief the solution type
 *
 */
class solution_t {
public:
  /**
   * @brief smart pointer that have reference to the problem
   */
  std::shared_ptr<problem_t> problem;

  /**
   * @brief cities indices.
   * cities with warehouse 
   */
  std::vector<int> warehouses;

  double bestScore = STARTDIST;
 /**
   * @brief goal function value.
   *
   *    Calculate distance from city to warehouse.
   *
   * @return double lenght of the path.
   */
  double score(){
    double dist;
    int n =0;
    double tempDist;
    for(auto city : problem->cities){

      for(int i = 1;i<warehouses.size();i++){
        /**
         * get the nearest warehouse
         *  **/
        if(i==1)
          tempDist=std::min(city.distance(problem->cities.at(0)),city.distance(problem->cities.at(1)));
        tempDist=std::min(tempDist,city.distance(problem->cities.at(i)));
      }
      /**
         * get farthest city from the nearest warehouse
         *  **/
      if(n++)
       dist=tempDist ;
      dist=std::max(dist,tempDist);

    }
    return dist;
  }
 /**
   * @brief Construct a new solution t object
   * It takes shared pointer to the problem definition. This way we don't have
   * to copy the problem, only smartpointer to it.
   * @param problem_ the problem smartpointer
   */
  solution_t(std::shared_ptr<problem_t> problem_)
        : problem(problem_), warehouses(problem_->numberOfWarhouse) {
      for (int i = 0; i < warehouses.size(); i++)
        warehouses[i] = i;
    };
  };

 
 solution_t brute_force_find_solution(solution_t problem) {
  using namespace std;
  solution_t best = problem;
  
  return best;
}
