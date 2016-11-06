/*
   Mon Oct 17 12:42:03 MSK 2016
   Written by Andrey Borzunov.
   Andrey.Borzunov@gmail.com 
   */

#include "../headers/gnuplot-iostream.h"
#include "../headers/Field.hpp"
#include <iostream>
#include <fstream>
#include <cmath>


using std::cout;
using std::endl;
using std::ofstream;

Field Ex;
Field Ey;
Field Hz;

Field Ez;
Field Hx;
Field Hy;

float min_ez = 0;
float max_ez = 0;
float min_hx = 0;
float max_hx = 0;
float min_hy = 0;
float max_hy = 0;

int Nx =200;
int Ny =200;
int Nt =200;

float begin_x = 0;
float begin_y = 0;
float end_x = 1;
float end_y = 1;
float end_time = 1;
float current_time = 0;

float delta_step_x = (end_x - begin_x) / Nx;
float delta_step_y = (end_y - begin_y) / Ny;
float delta_step_time = (end_time) / Nt;

float imp0 = 377.;					// sqrt(mu0/eps0)

const float speed_of_light = 299792458;
const float eps0 = 8.854187817 * pow(10.,-12.);
const float mu0 = 1.256637061 * pow(10.0,-6.0);
const float impedance0 = 376.730313461;
const float courant_number = 1/sqrt(2);

const int visualization_frequency = 8;

void make_png();
void make_gif();
void print_info(); 
int init();
bool courant_check();

float gauss(float t, float shift, float duration) {
	return exp(- (t-shift)*(t-shift)/(duration*duration));
}
float source(int qTime) {
	return gauss(qTime,30,30) - gauss(qTime,60,30);
}

int main(int argc, char** argv) {
	char* buff = (char*) malloc(sizeof(char) * 200);
	std::vector<float> source_gp;
	std::vector<float> slice(Nx);
	std::cout << "You should pass path to file with parameters of simulation" << std::endl;
	if (argc <2) {
		std::cout << "No parameter file. Default parameters will be used" << std::endl;
	} else { 
		// TODO: incorporate this feature
		std::cout << argv[1] << std::endl;
		std::cout << "Reading from file is not implemented, yet.\n Default Settings will be used" << std::endl;
	}
	init();
	print_info();
	Gnuplot gp;
	for (int qTime=0; qTime < Nt; qTime++) {
		// TMz
		for (int i=1; i < Nx-1; i++) { 
			for (int j=1; j < Ny-1; j++) { 
				Hx(i,j) = Hx(i,j) - courant_number * (Ez(i,j+1) - Ez(i,j)) / imp0;
				Hy(i,j) = Hy(i,j) + courant_number * (Ez(i+1,j) - Ez(i,j)) / imp0; 
				min_hx = (Hx(i,j) < min_hx) ? Hx(i,j) : min_hx;
				min_hy = (Hy(i,j) < min_hy) ? Hy(i,j) : min_hy;
				max_hx = (Hx(i,j) > max_hx) ? Hx(i,j) : max_hx;
				max_hy = (Hy(i,j) > max_hy) ? Hy(i,j) : max_hy;
			}
		}
		for (int i=1; i < Nx-1; i++) {
			for (int j=1; j < Ny-1; j++) {
				Ez(i,j) = Ez(i,j) + courant_number * imp0 * ( (Hy(i,j) - Hy(i-1,j)) - (Hx(i,j) - Hx(i,j-1)));
				min_ez = (Ez(i,j) < min_ez) ?Ez(i,j) : min_ez;
				max_ez = (Ez(i,j) > max_ez) ? Ez(i,j) : max_ez;
			}
		}

		for (int i=1; i < Nx -1 ; i++) {
	//		Ez(i,1) +=source(qTime);
		}
		/*
		 Ez(Nx/4-1,Ny/4) += source(qTime);
			Ez(Nx/4+1,Ny/4) += source(qTime);
			Ez(Nx/4,Ny/4-1) += source(qTime);
			Ez(Nx/4,Ny/4+1) += source(qTime);
		*/
			Ez(Nx/4,Ny/4) += source(qTime);
		source_gp.push_back(source(qTime));
		if ( qTime %visualization_frequency == 0) {
			sprintf(buff, "results/tdat/ez/ez_%d.tdat",qTime);
			Ez.printf(buff);
			sprintf(buff, "results/tdat/hx/hx_%d.tdat",qTime);
			Hx.printf(buff);
			sprintf(buff, "results/tdat/hy/hy_%d.tdat",qTime);
			Hy.printf(buff);
		}


		if (qTime == 60) {
			int j=Ny/4;
			for (int i=1; i<Nx-1; i++) {
				slice[i] = Ez(i,j);
			}
		}
		/*
		   cout << "step " << qTime << endl;
		   cout << "	max_ez: " << max_ez<< endl;
		   cout << "	min_ez: " << min_ez << endl;
		   cout << "	max hx: " << max_hx << endl;
		   cout << "	min_hx: " << min_hx << endl;
		   cout << "	max hy: " << max_hy << endl;
		   cout << "	min_hy: " << min_hy << endl;
		   */
	}


	//gp << "plot '-' with lines notitle\n";
	//gp.send1d(slice);

	gp << "set term png\n";
	gp << "set output 'results/source.png\n";
	gp << "set title \"source function of time\"\n";
	gp << "plot '-' with lines\n";
	gp.send1d(source_gp);


	cout << "Final max min" << endl;
	cout << "max_ez: " << max_ez<< endl;
	cout << "min_ez: " << min_ez << endl;
	cout << "max hx: " << max_hx << endl;
	cout << "min_hx: " << min_hx << endl;
	cout << "max hy: " << max_hy << endl;
	cout << "min_hy: " << min_hy << endl;
	make_png();
	make_gif();
	return 0;
}
void make_png() {
	Gnuplot gp;
	char* buff = (char*) malloc(512*sizeof(char));
	gp << "set terminal png large size 800,600\n";

	sprintf(buff, "set cbrange [%g:%g]\n",min_ez,max_ez);
	gp << buff;
	gp << "set pm3d at bs interpolate 2,2\n";
	//gp << "set contour base\n";
	gp << "unset key\n"; 
	//gp << "unset surface\n";
	gp << "set view map\n";
	gp << "set view 45,45\n";



	gp << "set palette defined ( 0 '#000090', 1 '#000fff', 2 '#0090ff', 3 '#0fffee',";
	gp << "4 '#90ff70', 5 '#ffee00', 6 '#ff7000', 7 '#ee0000', 8 '#7f0000')\n";

	gp << "set title \"Ez(t)\"\n";
	cout << "Making ez pngs" << endl;
		
	
	for (int i=0; i < Nt; i+=visualization_frequency) {
		sprintf(buff,"set output \"results/png/ez/ez_%d.png\"\n", i);
		gp << buff;
		sprintf(buff, "splot \"results/tdat/ez/ez_%d.tdat\" matrix with lines\n",i);
		gp << buff;

	}
	/*
	   sprintf(buff, "set cbrange [%g:%g]\n",min_hx,min_hx);
	   gp << buff;
	   cout << "Making hx pngs" << endl;
	   for (int i=0; i < Nt; i+=visualization_frequency) {
	   sprintf(buff,"set output \"results/png/hx/hx_%d.png\"\n", i);
	   gp << buff;
	   sprintf(buff, "splot \"results/tdat/hx/hx_%d.tdat\" with lines notitle\n",i);
	   gp << buff;
	   }
	   sprintf(buff, "set cbrange [%g:%g]\n",min_hy,min_hy);
	   gp << buff;
	   cout << "Making hy pngs" << endl;
	   for (int i=0; i < Nt; i+=visualization_frequency) {
	   sprintf(buff,"set output \"results/png/hy/hy_%d.png\"\n", i);
	   gp << buff;
	   sprintf(buff, "splot \"results/tdat/hy/hy_%d.tdat\" with lines notitle\n",i);
	   gp << buff;

	   }
	   */
}

void make_gif() { 
	char* buff = (char*) malloc(512*sizeof(char));
	cout << "Making ez gif" << endl;
	sprintf(buff, "convert -delay 15 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/ez/ez_%%s.png \" $a; done;) ./results/gif/ez.gif", Nt, visualization_frequency);
	system(buff);

	/* cout << "Making hy gif" << endl;
	   sprintf(buff, "convert -delay 5 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/hy/hy_%%s.png \" $a; done;) ./results/gif/hy.gif", Nt, visualization_frequency);
	   system(buff);
	   */
}
bool courant_check() {
	return  ( speed_of_light * delta_step_time / delta_step_x < (1 / sqrt(2))) ;
}
int init() {
	Ex = Field(Nx,Ny);
	Ey = Field(Nx,Ny);
	Hx = Field(Nx,Ny);

	Hx = Field(Nx,Ny);
	Hy = Field(Nx,Ny);
	Ez = Field(Nx,Ny);

	return 0;
}
void print_info() {
	cout << "WARNING, NOW WE SET COURANT NUMBER To match stability, thus real physical size will be neglected!" << endl;
	std::cout << "Printing information about Grid " << std::endl;
	std::cout << "Start at (" << begin_x << "," << begin_y << ")" << std::endl;
	std::cout << "End at (" << end_x <<"," << end_y <<")" << std::endl;
	std::cout << "Segmnatation is (" << Nx <<"," << Ny <<")"  << std::endl;
	std::cout << "Delta steps is (" << delta_step_x <<"," << delta_step_y  <<")" << std::endl;
	std::cout << "Speed of light " << speed_of_light <<std::endl;
	std::cout << "Vacuum permetivity " << eps0 <<std::endl;
	std::cout << "Vacum permeability " << mu0 <<std::endl;
	std::cout << "Vacuum impedance " << impedance0 <<std::endl;
	std::cout << "Courant cheach " << ( (courant_check()) ? "passed" : "failed")
		<< "\n\t" << speed_of_light * delta_step_time / delta_step_x << " < " << (1 / sqrt(2)) << endl;
}
