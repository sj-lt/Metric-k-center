#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>



class city_t {
public:
  std::string name;
  double longitude;
  double latitude;

  double distance(city_t &c2);
};
class problem_t {
public:

  std::vector<city_t> cities;
  
};

class solution_t {
public:

  std::shared_ptr<problem_t> problem;
  std::vector<int> warehouses;
  int numberOfWarehouses;

  double bestScore ;
  double getBestScoreKm();
  double score();

  std::vector<city_t> getCitySolution();

  solution_t(std::shared_ptr<problem_t> problem_, int numberOfWarehouses);

  solution_t(std::vector<city_t> input_cities, int numberOfWarehouses);
      
  solution_t(int numberOfWarehouses);
  solution_t();
 };
