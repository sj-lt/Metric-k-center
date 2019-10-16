#include "problem.cpp" 
int main(int argc, char **argv) {
  using namespace std;

  solution_t experiment({{"Warszawa", 52.2330269, 20.7810081},
                         {"Gdansk", 54.3612063, 18.5499431},
                         {"Bydgoszcz", 53.1169002, 17.9008963},
                         {"Poznan", 52.456009, 16.896973},
                         {"Wrocław",51.1000000,17.0333300},
                         {"Kraków",50.0614300,19.9365800},
                         {"Lublin",51.2500000,22.5666700}},WAREHOUSES);

    ifstream is("citiesCsv.json"); // open file
    is >> experiment;

    experiment.numberOfWarehouses = WAREHOUSES;
    chaseSequence combinations = chaseSequence(experiment);

    //std::cout<<combinations.t_<<std::endl;
    combinations.init();
  
	cout<<"best score: "<<combinations.problem_.getBestScoreKm()<<endl;
	ofstream os("magazyny.json"); // open file
    os << combinations.problem_;

    std::ofstream htmlout("vis.html");
    htmlout << html_header;

    for (auto city : combinations.problem_.problem->cities) {
        htmlout << html_marker_start;
        htmlout << city.longitude << ",";
        htmlout << city.latitude << std::endl;
        htmlout << html_marker_end;

    }
    for (auto city : combinations.problem_.getCitySolution()) {
        htmlout << html_circle_start;
        htmlout << city.longitude << ",";
        htmlout << city.latitude<<"],{radius: "<<combinations.problem_.bestScore << std::endl;
        htmlout << html_circle_end;
            //50.5, 30.5], {radius: 200 to add beetween

    }
  htmlout << html_footer;
  htmlout.close();

};