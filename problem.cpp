#include <cmath>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

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
 * The cities we would like to see.
 */
class problem_t {
public:
  /**
   * @brief list of the cities to visit.
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
   * The order in wich we would like to see every city
   */
  std::vector<int> warehouses;


  };

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
