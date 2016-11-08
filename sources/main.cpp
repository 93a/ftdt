/*
   Mon Oct 17 12:42:03 MSK 2016
   Written by Andrey Borzunov.
   Andrey.Borzunov@gmail.com 
   */

#include "../headers/gnuplot-iostream.h"
#include "../headers/Grid.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

int main(int argc, char** argv) {
	Grid g;
	g.start();
	g.make_png();
	g.make_gif();
	return 0;
}

/* old stuff {{{*/
/*
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
	   
}

void make_gif() { 
	char* buff = (char*) malloc(512*sizeof(char));
	cout << "Making ez gif" << endl;
	sprintf(buff, "convert -delay 15 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/ez/ez_%%s.png \" $a; done;) ./results/gif/ez.gif", Nt, visualization_frequency);
	system(buff);

	 cout << "Making hy gif" << endl;
	   sprintf(buff, "convert -delay 5 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/hy/hy_%%s.png \" $a; done;) ./results/gif/hy.gif", Nt, visualization_frequency);
	   system(buff);
	   
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
	std::cout << "dx " << delta_step_x << std::endl;
	std::cout << "dy " << delta_step_y << std::endl;
	std::cout << "dt " << delta_step_time << std::endl;
	std::cout << "begin x " << begin_x<< std::endl;
	std::cout << "begin y " << begin_y<< std::endl;
	std::cout << "end x " << end_x<< std::endl;
	std::cout << "end y " << end_y<< std::endl;
	std::cout << "end time " << end_time<< std::endl;

	std::cout << "Courant cheach " << ( (courant_check()) ? "passed" : "failed")
		<< "\n\t" << speed_of_light * delta_step_time / delta_step_x << " < " << (1 / sqrt(2)) << endl;
}
*/
/* }}}} */
