#include "../headers/Grid.hpp"


Grid::Grid() : 
	Nx(200), 
	Ny(200), 
	Nt(400), 
	max_interested_frequency(2.2e9) {	
		init();
		print_info();
	};

void Grid::print_info() {
	std::cout << "This grid assumed to be 2d with incorporation of TMz and TEz modes" << std::endl;
	std::cout << "Maximun wave frequency that grid implements is: " << max_interested_frequency << std::endl;
	std::cout << "\twith wave length at free space " << speed_of_light / max_interested_frequency  << std::endl;
	std::cout << "\tmin length at grid with " << min_wave_length << std::endl;
	std::cout << "Dots per wave: " << poitnts_per_wave <<std::endl;
	std::cout << "dx and dy chosen to ensure dots per wave" << std::endl;
	std::cout << "dx: " << dx <<std::endl;
	std::cout << "dy: " << dy <<std::endl;
	std::cout << "dt derived from dx(dx assumed to be equal dy)!" << std::endl;
	std::cout << "dt: " << dt <<std::endl;
	std::cout << "Nx is: " << Nx <<std::endl;
	std::cout << "Ny is: " << Ny <<std::endl;
	std::cout << "Nt is: " << Nt <<std::endl;
	std::cout << "X axis ends at physical: " << end_x <<std::endl;
	std::cout << "Y axis ends at physical: " << end_y <<std::endl;
	std::cout << "End time of simulation is:  " << end_time <<std::endl;
	std::cout << "Speed of light: " << speed_of_light <<std::endl;
	std::cout << "eps0: " << eps0 <<std::endl;
	std::cout << "mu0: " << mu0 <<std::endl;
	std::cout << "imp0: " << impedance0<<std::endl;

	std::cout << "Printing the log: " << std::endl;
	for (int i=0; i < log.size(); i++)
		std::cout << log[i] << std::endl;
}
/* initialization {{{
 * reserves memory for	field
 *						Curl CE CH vectors
 *						mu, eps vectors and set them to 1
 * Everything except mu,eps sets to 0
 */
void Grid::init() {
	Ex = Field(Nx,Ny);
	Ey = Field(Nx,Ny);
	Ez = Field(Nx,Ny);

	Hx = Field(Nx,Ny);
	Hy = Field(Nx,Ny);
	Hz = Field(Nx,Ny);

	TMz_CEx = Field(Nx,Ny);
	TMz_CEy = Field(Nx,Ny);
	TMz_CHz = Field(Nx,Ny);

	TEz_CHx = Field(Nx,Ny);
	TEz_CHy = Field(Nx,Ny);
	TEz_CEz = Field(Nx,Ny);

	eps = Field(Nx,Ny);
	mu = Field(Nx,Ny);

	/*initialize fields{{{*/
	for (int i=0; i < Nx*Ny; i++) {
		eps(i) = 1;
		mu(i) = 1;
		Ex(i) = 0;
		Ey(i) = 0;
		Ez(i) = 0;

		Hx(i) = 0;
		Hy(i) = 0;
		Hz(i) = 0;

		TMz_CEx(i) = 0;
		TMz_CEy(i) = 0;
		TMz_CHz(i) = 0;

		TEz_CHx(i) = 0;
		TEz_CHy(i) = 0;
		TEz_CEz(i) = 0;
	}
	/*}}}*/

	init_min_wave_length();
	dx =		min_wave_length / poitnts_per_wave;
	dy =		dx;
	dt =		dx/speed_of_light;
	end_x =		dx * Nx;
	end_y =		dx * Ny;
	end_time =	dt * Nt;
}/*}}}*/


/** init_min_wave_length() {{{ 
 * Scroll all the grid and find maximum eps*mu of the material
 * there will be a wave with minimum length
 * compute and define @var min_wave_length;
 * pushes result to the log
 */
void Grid::init_min_wave_length() {

	//for propper logging
	char* buff = (char*) malloc(sizeof(char) * 512);
	int position;

	double max = 0 ;
	double tmp;
	for (int i=0; i < Nx*Ny; i++) {
		tmp = eps(i) * mu(i);
		if (tmp > max) {
			max = tmp;
			position = i;
		}
	}
	min_wave_length = speed_of_light / sqrt(max) / max_interested_frequency;
	sprintf(buff, "\tMin wave leght of %g will be at node (%d,%d) where (eps,mu)=(%g,%g)", 
			min_wave_length, position/Nx, position%Nx, eps(position), mu(position));
	log.push_back(buff);
}
/*}}}*/

/* snapshots of field {{{
 * takes enum of what field should it write
 */
void Grid::binary_snapshot(field_enumeration f, int t) {
	char* buff = (char*) malloc(sizeof(char) * 512);
	switch(f) {
		case field_enumeration::Ex_enum:
			sprintf(buff, "results/bindat/ex/ex_%d.bin", t);
			wdata_bin(buff, &Ex(0), Nx, Ny);
			break;
		case field_enumeration::Ey_enum:
			sprintf(buff, "results/bindat/ey/ey_%d.bin", t);
			wdata_bin(buff, &Ey(0), Nx, Ny);
			break;
		case field_enumeration::Ez_enum:
			sprintf(buff, "results/bindat/ez/ez_%d.bin", t);
			wdata_bin(buff, &Ez(0), Nx, Ny);
			break;
		case field_enumeration::Hx_enum:
			sprintf(buff, "results/bindat/hx/hx_%d.bin", t);
			wdata_bin(buff, &Hx(0), Nx, Ny);
			break;
		case field_enumeration::Hy_enum:
			sprintf(buff, "results/bindat/hy/hy_%d.bin", t);
			wdata_bin(buff, &Hy(0), Nx, Ny);
			break;
		case field_enumeration::Hz_enum:
			sprintf(buff, "results/bindat/hz/hz_%d.bin", t);
			wdata_bin(buff, &Hz(0), Nx, Ny);
			break;
	}

}
void Grid::wdata_bin(const char *fname, const double *data, const uint row, const uint col) {
	uint x, y;
	float *tsp = (float *)malloc((row + 1) * (col + 1) * sizeof(float));

	for(x = 0; x < row + 1; x++)
		for(y = 0; y < col + 1; y++)
			if(!y && !x)
				tsp[0] = col;
			else if(!x && y != 0)
				tsp[x * (col + 1) + y] = y - 1;
			else if(!y && x != 0)
				tsp[x * (col + 1) + y] = x - 1;
			else
				tsp[x * (col + 1) + y] = (float)data[(x - 1) * col + y - 1];

	FILE *outfile = fopen(fname, "w");
	fwrite(tsp, sizeof(float), (row + 1) * (col + 1), outfile);
	fclose(outfile);
	free(tsp);
}
/* }}} */

int Grid::start() {
	for (qTime=0; qTime < Nt; qTime++) {
		time_iter();
		 (!(qTime % visualization_frequency)) ? 
			binary_snapshot(field_enumeration::Ez_enum,qTime) : ;
	}
	return 0;
}
void Grid::time_iter() {
	
	for (int i=1; i<Nx-1; i++) {
		for (int j=1; j<Ny-1; j++) {
			Hx(i,j) = Hx(i,j) - (1 / sqrt(2)) * (Ez(i,j+1) - Ez(i,j)) / imp0;
			Hy(i,j) = Hy(i,j) + (1/sqrt(2)) * (Ez(i+1,j) - Ez(i,j)) / imp0;
			min_hx = (Hx(i,j) < min_hx) ? Hx(i,j) : min_hx;	
			min_hy = (Hy(i,j) < min_hy) ? Hy(i,j) : min_hy;	
			max_hx = (Hx(i,j) > max_hx) ? Hx(i,j) : max_hx;	
			max_hy = (Hy(i,j) > max_hy) ? Hy(i,j) : max_hy;	
		}
	}
	for (int i=1; i<Nx-1; i++) { 
		for (int j=1; j<Ny-1; j++) {
			Ez(i,j) = Ez(i,j) + (1/sqrt(2)) *  imp0 * ( (Hy(i,j) - Hy(i-1,j)) - (Hx(i,j) - Hx(i,j-1)));
			min_ez = (Ez(i,j) < min_ez) ? Ez(i,j) : min_ez;	
			max_ez = (Ez(i,j) > max_ez) ? Ez(i,j) : max_ez ;	
		}
	}
	Ez(Nx/4,Ny/4) += source_sin(qTime, max_interested_frequency/4);
	Ez(Nx/4*3,Ny/4*3) += - source_sin(qTime, max_interested_frequency/4);
	source_graph.push_back(source_sin(qTime, max_interested_frequency/4));	
}

void Grid::update_curl() {
	for(int i=1; i < Nx - 1; i++) {
		for (int j=1; j < Ny -1; j++) {
			TMz_CEx(i,j) = (Ez(i,j+1) - Ez(i,j)) / dx;
			TMz_CEy(i,j) = (Ez(i+1,j) - Ez(i,j)) / dy;
			TMz_CHz(i,j) = (Hy(i,j) - Hy(i-1,j)) / dx - (Hx(i,j) - Hx(i,j-1)) / dy;

			//TEz_CHx = 0;
			//TEz_CHy = 0
			//TEz_CEz = 0;
		}
	}
}

void Grid::make_png() {

	Gnuplot gp;
	char* buff = (char*) malloc(512*sizeof(char));
	gp << "set terminal png large size 800,600\n";

	sprintf(buff, "set cbrange [%g:%g]\n",-1.0,1.0);
	gp << buff;
	sprintf(buff, "set zrange [%g:%g]\n",-1.0,1.0);
	gp << buff;
	gp << "set pm3d at bs interpolate 2,2\n";
	//gp << "set contour base\n";
	gp << "unset key\n"; 
	//gp << "unset surface\n";
	gp << "set view 45,45\n";
	gp << "set palette defined ( 0 '#000090', 1 '#000fff', 2 '#0090ff', 3 '#0fffee',";
	gp << "4 '#90ff70', 5 '#ffee00', 6 '#ff7000', 7 '#ee0000', 8 '#7f0000')\n";
	gp << "set title \"Ez(t)\"\n";
	std::cout << "Making ez pngs" << std::endl;

	for (int i=0; i < Nt; i+=visualization_frequency) {
		sprintf(buff,"set output \"results/png/ez/ez_%d.png\"\n", i);
		gp << buff;
		sprintf(buff, "splot 'results/bindat/ez/ez_%d.bin' binary matrix notitle with lines\n",i);
		gp << buff;

	}
}

void Grid::make_gif() {
	char* buff = (char*) malloc(512*sizeof(char));
	std::cout << "Making ez gif" << std::endl;
	sprintf(buff, "convert -delay 15 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/ez/ez_%%s.png \" $a; done;) ./results/gif/ez.gif", Nt, visualization_frequency);
	system(buff);

}
