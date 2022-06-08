#include "./math/mathstuffbasic.h"

double rnd(double max, long int randomseed){
    //static bool first=true;
    //std::random_device rd;  // Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(randomseed); // Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<> dis(0, max);
    return dis(gen);
    //static gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);
    //if(first) { gsl_rng_set (r, randomseed);first=false;}
    //
    //return gsl_rng_uniform(r)*max;

}
/*
double gauss_rand(double sigma, long int randomseed){

    static bool first=true;
    static gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);

    if(first) { gsl_rng_set (r, randomseed);first=false;}

    return gsl_ran_gaussian (r, sigma);

    //gsl_rng_free (r);

}
*/
double theta(double x){
    if(x>0) return 1;
    return 0;
}
/*
double boltzmann_rand(double nu, long int randomseed){

    static bool first=true;
    static gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);

    if(first) { gsl_rng_set (r, randomseed);first=false;}

    return  gsl_ran_chisq  (r, nu);

    //gsl_rng_free (r);

}
double gamma_dist(double a, double b, long int randomseed){
    static bool first=true;
    static gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);

    if(first) { gsl_rng_set (r, randomseed);first=false;}

    return gsl_ran_gamma(r, a,b);
}
*/
double sinxoverx(double x){

    if (x==0)return 1;
    return sin(x)/x;
}
double Minimum_own(QList<double> x){
    if (x.size()==0)return 0;
    double mini=x[0];
    for (long int i=1;i<x.size();i++){
        if (x[i]<mini)mini=x[i];
    }
    return mini;
}

long int Minimum_own_index(QList<double> x){
    if (x.size()==0)return -1;
    double mini=x[0];
    long int j=0;
    for (long int i=1;i<x.size();i++){
        if (x[i]<mini){mini=x[i];j=i;}
    }
    return j;
}

double Maximum_own(QList<double> x){
    if (x.size()==0)return 0;
    double maxi=x[0];
    for (long int i=1;i<x.size();i++){
        if (x[i]>maxi)maxi=x[i];
    }
    return maxi;
}

long int Maximum_own_index(QList<double> x){
    if (x.size()==0)return 0;
    double maxi=x[0];
    long int j=0;
    for (long int i=1;i<x.size();i++){
        if (x[i]>maxi){maxi=x[i];j=i;}
    }
    return j;
}

double sgn(double x){

    if (x==0) return 0.0;
    if (x<0) return -1.0;
    if (x>0) return 1.0;
    return 0;
}

double absolute(double x){
    return sgn(x)*x;
}

int KroneckerDelta(int i, int j){
    if (i==j)return 1;
    return 0;
}

QVector<long> one_d_to_X_d(long int number, QList<long> base){


    QVector<long> temp(base.size(),0);

    for (long i=1;i<=number;i++){
        temp[0]++;
        for(long j=0;j<base.size()-1;j++)if (temp[j]==base[j]){temp[j]=0;temp[j+1]++;}


    }

    return temp;
}

QVector<long> one_d_to_X_d(long number, long base, long X){

    QVector<long> temp(X,0);

    for (long i=X-1;i>=0;i--){

        int y=long(number/pow(base,i));
        if (y>0){temp[i]=y;number-=y*pow(base,i);}

    }

    return temp;
}

void fifo(double  x[],int size,double v){

    for(int i=0;i<size-1;i++){
        x[i]=x[i+1];

    }
    x[size-1]=v;
}

void fifo_avg(double x[],int size,double v,double& avg){

    for(int i=0;i<size-1;i++){
        x[i]=x[i+1];

    }
    x[size-1]=v;
    avg=avg-x[0]/size+v/size;

}

long int which_drawer_floor(double value,double start,double end, long int Ndrawers){
    long int bla=Ndrawers*(value-start)/(end-start);
    if (bla==Ndrawers)bla--;
    return bla;
    //0 ... drawers-1
}

double drawer_value_floor(double value,double start,double end, long int Ndrawers){
    double drawer=which_drawer_floor(value, start,end,  Ndrawers);
    return (drawer/double(Ndrawers))*(end-start)+start;

}


bool inc_multidim_cursor(QList<long int>& cursor, QList<long int> base){
    int size=cursor.size(); if (size!=base.size()) return false;

    for (int i=size-1;i>-1;i--){


        cursor[i]++;
        if (cursor[i]==base[i])cursor[i]=0;else return true;
    }

    return false;


}
double catch_value(double x_start,double x_end,long int number, long int index){

    return double(index)*(x_end-x_start)/double(number)+x_start;

}
