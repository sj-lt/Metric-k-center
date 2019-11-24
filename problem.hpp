#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include "json.hpp"

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
  nlohmann::json config_json;

  double bestScore ;
  double getBestScoreKm();
  double score();

  std::vector<city_t> getCitySolution();

  solution_t(std::shared_ptr<problem_t> problem_, nlohmann::json config_json);

  solution_t(std::vector<city_t> input_cities, nlohmann::json config_json);
      
  solution_t(nlohmann::json config_json);
  solution_t();
 };

 class solver_t {
  public:
    solution_t problem_;
    void virtual gimmeSolution()=0;
    double calculate();


 };
double solver_t::calculate(){
    auto t1 = std::chrono::high_resolution_clock::now();
    gimmeSolution(); 
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()/1000000.0;
}