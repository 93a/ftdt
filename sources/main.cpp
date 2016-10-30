/*
Mon Oct 17 12:42:03 MSK 2016
Written by Andrey Borzunov.
Andrey.Borzunov@gmail.com 
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iterator>
#include "../headers/gnuplot-iostream.h"
#include <unistd.h>
#include <stdlib.h>

using std::vector;
using std::cout;
using std::endl;
using std::ofstream;
const int SIZE = 200;
const double imp0 = 377.0;
const int TIME =250;
const int VISUALIZATION_FREQUENCY = 5;
double max_ez = 0;
double max_hy = 0;
double min_ez = 0;
double min_hy = 0;

inline void mySleep(unsigned millisec) {
    ::usleep(millisec);
}

void make_png();
void make_gif(); 
int main(int argc, char** argv) {
    vector<double> ez(SIZE), hy(SIZE);
    char* buff = (char*) malloc(512*sizeof(char));
    Gnuplot gp;
    for (int qTime = 0; qTime < TIME; qTime++) {

        hy[SIZE-1] = hy[SIZE - 2];
        for (int i=0; i < SIZE - 1; i++) {
            hy[i] = hy[i] + (ez[i+1] - ez[i]) / imp0;
            max_hy = hy[i] > max_hy ? hy[i] : max_hy;
            min_hy = hy[i] < min_hy ? hy[i] : min_hy;
        }
        ez[0] = ez[1];
        for (int i=1; i < SIZE; i++) {
            ez[i] = ez[i] + (hy[i] - hy[i-1]) * imp0;
            max_ez = (ez[i] > max_ez) ? ez[i] : max_ez; 
            min_ez = (ez[i] < min_ez) ? ez[i] : min_ez;
        }
        /* use additive source at node 50 */
         ez[50] += exp(-(qTime - 30.0) * (qTime -30.0)/200);
        
		//ez[50] += sin(qTime*M_PI/100);
        /* ABC realization */

        if ( qTime%VISUALIZATION_FREQUENCY == 0) {
            sprintf(buff,  "results/tdat/ez/ez_%d.tdat", qTime);
            ofstream out_ez(buff);
            sprintf(buff,  "results/tdat/hy/hy_%d.tdat", qTime);
            ofstream out_hy(buff);
            std::ostream_iterator<double> out_iter_ez(out_ez, "\n");
            std::ostream_iterator<double> out_iter_hy(out_hy, "\n");
            copy(ez.begin(), ez.end(), out_iter_ez);
            copy(hy.begin(), hy.end(), out_iter_hy);
            out_ez.close();
            out_hy.close();
        }

    }
    //make_png();
    //make_gif();
    cout << "max_ez: " << max_ez<< endl;
    cout << "min_ez: " << min_ez << endl;
    cout << "max hy: " << max_hy << endl;
    cout << "min_hy: " << min_hy << endl;
    return 0;
} 

void make_png() {
    Gnuplot gp;
    char* buff = (char*) malloc(512*sizeof(char));
    gp << "set terminal png\n";

    sprintf(buff, "set yrange [%g:%g]\n",min_ez, max_ez);
    gp << buff;
    cout << "Making ez pngs" << endl;
    for (int i=0; i < TIME; i+=VISUALIZATION_FREQUENCY) {
        sprintf(buff,"set output \"results/png/ez/ez_%d.png\"\n", i);
        gp << buff;
        sprintf(buff, "plot \"results/tdat/ez/ez_%d.tdat\" with lines notitle\n",i);
        gp << buff;
    }

    sprintf(buff, "set yrange [%g:%g]\n",min_hy, max_hy);
    gp << buff;

    cout << "Making hy pngs" << endl;
        for (int i=0; i < TIME; i+=VISUALIZATION_FREQUENCY) {
            sprintf(buff,"set output \"results/png/hy/hy_%d.png\"\n", i);
            gp << buff;
            sprintf(buff, "plot \"results/tdat/hy/hy_%d.tdat\" with lines notitle\n",i);
            gp << buff;
        }
}

void make_gif() { 
    char* buff = (char*) malloc(512*sizeof(char));
    cout << "Making ez gif" << endl;
    sprintf(buff, "convert -delay 5 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/ez/ez_%%s.png \" $a; done;) ./results/gif/ez.gif", TIME, VISUALIZATION_FREQUENCY);
    system(buff);
    cout << "Making hy gif" << endl;
    sprintf(buff, "convert -delay 5 -loop 0 $(for ((a=0; a < %d; a+=%d )); do printf -- \"./results/png/hy/hy_%%s.png \" $a; done;) ./results/gif/hy.gif", TIME, VISUALIZATION_FREQUENCY);
    system(buff);
}
