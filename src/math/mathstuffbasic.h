#ifndef MATHSTUFFBASIC_H
#define MATHSTUFFBASIC_H

#include <QtGlobal>
#include <QVector>
#include <QList>
#include <QtGlobal>

#include <iostream>
#include <math.h>
#include <limits>
#include <complex>

//gaussian random number
    #include <stdio.h>
//    #include <gsl/gsl_rng.h>


#include <random>

//    #include <gsl/gsl_randist.h>
//



double rnd(double max, long int randomseed=1);
//double gauss_rand(double sigma, long int randomseed=1);
double theta(double x);
//double boltzmann_rand(double nu, long int randomseed=1);
//double gamma_dist(double a, double b, long int randomseed=1);
double sinxoverx(double x);
double Minimum_own(QList<double> x);
long int Minimum_own_index(QList<double> x);
double Maximum_own(QList<double> x);
long int Maximum_own_index(QList<double> x);
double sgn(double x);
double absolute(double x);
QVector<long int> one_d_to_X_d(long number, QList<long int> base);
QVector<long int> one_d_to_X_d(long number, long base, long X);
int KroneckerDelta(int i, int j);

void fifo(double  x[],int size,double v);
void fifo_avg(double  x[],int size,double v,double& avg);

long int which_drawer_floor(double value,double start,double end, long int Ndrawers);
double drawer_value_floor(double value,double start,double end, long int Ndrawers);

bool inc_multidim_cursor(QList<long int>& cursor, QList<long int> base);
double catch_value(double x_start,double x_end,long int number, long int index);

double rad_to_degree(double x);

#endif // MATHSTUFFBASIC_H
