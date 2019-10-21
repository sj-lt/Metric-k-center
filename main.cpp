#include "problem.cpp"
void visualize(solution_t *problem);
int main(int argc, char **argv)
{
	using namespace std;

	solution_t experiment({{"Warszawa", 52.2330269, 20.7810081},
						   {"Gdansk", 54.3612063, 18.5499431},
						   {"Bydgoszcz", 53.1169002, 17.9008963},
						   {"Poznan", 52.456009, 16.896973},
						   {"Wrocław", 51.1000000, 17.0333300},
						   {"Kraków", 50.0614300, 19.9365800},
						   {"Lublin", 51.2500000, 22.5666700}},
						  WAREHOUSES);

	ifstream is("citiesTest6Csv.json"); // open file
	is >> experiment;

	experiment.numberOfWarehouses = WAREHOUSES;
	chaseSequence combinations = chaseSequence(experiment);
	auto duration = combinations.mesureTime();

	cout << "duration: " << duration << endl;
	cout << "best score: " << combinations.problem_.getBestScoreKm() << endl;
	cout<<"number of cities"<<combinations.problem_.problem->cities.size()<<endl;
	ofstream os("magazyny1.json"); // open file
	os << combinations.problem_;
	visualize(&combinations.problem_);
  
};


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

