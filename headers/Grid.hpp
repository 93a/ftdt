#ifndef GRID_HPP
#define GRID_HPP

#include <utility>
#include <vector>
#include <string>
#include <iostream>

class Grid {
	private:
		std::pair<float, float> physical_start;
		std::pair<float, float> physical_end;
		std::pair<float, float> physical_size;
		std::pair<int, int> segmantation;
		std::pair<float, float> delta_steps;
		std::vector<std::tuple<float, float, float> > H_field;
		std::vector<std::tuple<float, float, float> > E_field;
		std::string description;
	protected:
	public:
		Grid() {
		description = "Default Constructor has made this. Shoud be done futher initiazation";}
		Grid(std::pair<float,float> start, 
				std::pair<float, float>  size,
				std::pair<int, int> segments,
				std::string des ) : description(des) {
			physical_start = start;
			physical_size = size;
			physical_end.first = start.first + size.first;
			physical_end.first = start.second + size.second;
			delta_steps.first = size.first / segments.first;
			delta_steps.second = size.second / segments.second;
			segmantation = segments;
		}

		void print_info() {
			std::cout << "Printing information about Grid object" << std::endl;
			std::cout << description << std::endl;
			std::cout << "Start at (" << physical_start.first << "," << physical_start.second << ")" << std::endl;
			std::cout << "End at (" << physical_end.first <<"," <<physical_end.second <<")" << std::endl;
			std::cout << "Size is (" << physical_size.first <<"," <<physical_size.second <<")" << std::endl;
			std::cout << "Segmnatation is (" << segmantation.first <<"," <<segmantation.second <<")"  << std::endl;
			std::cout << "Delta steps is (" << delta_steps.first <<"," <<delta_steps.second <<")" << std::endl;
		}
		std::vector<std::tuple<float, float, float> > get_E_field() {
			return E_field;}
		std::vector<std::tuple<float, float, float> > get_H_field() {
			return H_field;}
		std::pair< std::tuple<float, float, float>, std::tuple<float, float, float> > get_snapshot() {
			return std::make_pair( E_field, H_field);
		}
		~Grid() {}
};

#endif
