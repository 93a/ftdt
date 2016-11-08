#ifndef FIELD_HPP
#define FIELD_HPP

#include <vector>
#include <fstream>
#include <iomanip>

class Field {
	private:
		int Nx;
		int Ny;
		std::vector<double> field;
	public:
		Field() {}
		Field(int x, int y) : Nx(x), Ny(y) {
			field.reserve(Nx*Ny);
		}

		double& operator()(int x, int y) {
			return field[x*Nx + y];
		}
		double& operator()(int x) {
			return field[x];
		}
		void printf(char* path) {
			std::ofstream os(path);
			for (int i=0; i < Nx; i++) { 
				for (int j=0; j < Ny; j++) {
					os << std::setprecision(4) << field[i*Nx + j] << " ";
				}
				os << std::endl;
			}
		}
};
#endif
