#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <cmath>
#include "Field.hpp"
#include "gnuplot-iostream.h"
enum class field_enumeration  {Ex_enum, Ey_enum, Ez_enum, Hx_enum, Hy_enum, Hz_enum};

/* 
 * This class supposed to be a 2d grid for FDTD computations
 */
class Grid {
	private:
		Field Ex;
		Field Ey;
		Field Hz;

		Field Ez;
		Field Hx;
		Field Hy;

		Field TMz_CEx;	
		Field TMz_CEy;
		Field TMz_CHz;

		Field TEz_CHx;					
		Field TEz_CHy;
		Field TEz_CEz;

		Field eps;
		Field mu;

		double dx;
		double dy;
		double dt;
		double end_x;
		double end_y;
		double end_time;
		int Nx;
		int Ny;
		int Nt;
		int qTime;										// current time step

		double min_wave_length;							// minimum wave length that wil ever occur during simulation
		double max_interested_frequency;				// Ever frequency higher this will be filtered my mesh
		const double poitnts_per_wave = 15;				// used to define dx,dy

		int visualization_frequency = 5;				// Frequency of snapshots

		/* minmax of All field {{{
		 * Required for visualization bu Gnuplot
		 * later would be passed to it.
		 * Gnuplot sets with it cbrange and zrange later
		 */
		double min_ex = 0;
		double min_ey = 0;
		double min_ez = 0;
		double min_hx = 0;
		double min_hy = 0;
		double min_hz = 0;
		double max_ex = 0;
		double max_ey = 0;
		double max_ez = 0;
		double max_hx = 0;
		double max_hy = 0;
		double max_hz = 0;
		// }}}

		/** log of simulation where messages will be placed
		 * Will be displayed after end
		 */
		std::vector<std::string> log;
		std::vector<float> source_graph;
		void init_min_wave_length();
		void init();
		void update_curl();
		void time_iter();
	protected:
		/**
		 * Method writes a binary snapshot of selected by @param fileld_enumeration field
		 * that later will be used by gnuplot for 'binary matrix' plotting.
		 * see gnuplot help about 'binary' and 'binary matrix'
		 "The `binary matrix` format contains a two dimensional array of 32 bit IEEE
		 float values plus an additional column and row of coordinate values.  In the
		 `using` specifier of a plot command, column 1 refers to the matrix row
		 coordinate, column 2 refers to the matrix column coordinate, and column 3
		 refers to the value stored in the array at those coordinates." -- from that help.
		 * @param int stept used to name file corresponding file properly,
		 */
		void binary_snapshot(field_enumeration field, int step);
		/* submethod of @method void binary_snapshot(field_enumeration field, int step)
		 * that just writes binary data to file that will suit mentioned above format
		 */
		void wdata_bin(const char *fname, const double *data, const uint row, const uint col);


	public:

		/*** Just a sin source {{{
		*/
		inline double source_sin(double t) {
			return sin((2*M_PI) * max_interested_frequency * t);
		}
		double source_sin(int step){
			return sin((2*M_PI) * max_interested_frequency * step * dt); 
		}
		inline double source_sin(double t, double freq) {
			return sin((2*M_PI) * freq * t); 
		}
		inline double source_sin(int step, double freq) {
			return sin((2*M_PI) * freq * step * dt); 
		} 
		/* }}} */

		/* constants  {{{
		*/
		const double speed_of_light = 299792458;
		const double eps0 = 8.854187817e-12;
		const double mu0 = 1.256637061e-6;
		const double imp0 = sqrt(mu0/eps0);
		const double impedance0 = 376.730313461;
		const double courant_number = 1/sqrt(2);
		/* }}} */
		void print_info();
		int start();
		void make_png();
		void make_gif();
		Grid();
};
#endif
