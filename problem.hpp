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

class city_t
{
public:
  std::string name;
  double longitude;
  double latitude;

  double distance(city_t &c2);
};

class problem_t
{
public:
  std::vector<city_t> cities;
};

class solution_t
{
public:
  std::shared_ptr<problem_t> problem;
  std::vector<int> warehouses; //solution
  int numberOfWarehouses;
  nlohmann::json config_json; //config

  double timeTaken;
  double bestScore;
  double getBestScoreKm();
  double score();
  double scoreParallel(std::vector<int> warehouses);
  std::vector<city_t> getCitySolution();

  solution_t(std::shared_ptr<problem_t> problem_, nlohmann::json config_json);
  solution_t(std::vector<city_t> input_cities, nlohmann::json config_json);
  solution_t(nlohmann::json config_json);
  solution_t();
};

class solver_t
{
public:
  solution_t problem_;
  std::string logIteration_;
  void virtual gimmeSolution() = 0;
  double calculate();
  inline void logger(nlohmann::json logMsg);
};
double solver_t::calculate()
{
  auto t1 = std::chrono::high_resolution_clock::now();
  gimmeSolution();
  auto t2 = std::chrono::high_resolution_clock::now();
  return problem_.timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000000.0;
}

inline void solver_t::logger(nlohmann::json logMsg)
{
  using namespace std;
  string output = problem_.config_json["output"];
  ofstream ofs(output.c_str(), std::ios_base::out | std::ios_base::app);
  ofs << logMsg << '\n';
  ofs.close();
}
