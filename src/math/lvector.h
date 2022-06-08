#ifndef LVECTOR_H
#define LVECTOR_H

#include<QVariant>

#include "./math/mathstuffbasic.h"
#include "./stringstuff/stringstuffbasic.h"
//#include <QtOpenGL/QGLWidget>

//eigenvalue problem

    //#include "gsl/gsl_matrix.h"
    //#include "gsl/gsl_vector.h"
    //#include "gsl/gsl_eigen.h"

    //Invertierung
//   #include <gsl/gsl_linalg.h>
//

#include <stdlib.h>

//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>



class lvector

{

public:
    lvector();
    lvector(const lvector &x);
    lvector(QVector<double> x);
    lvector(QList<double> x);
    lvector(long set_size,double default_value=0);
    QList<double> get_list();

    //lvector operator=(lvector a);
    double& operator[](long i);
    lvector operator->* (lvector a);
    lvector operator%(lvector a);
    lvector operator+(lvector x);
    lvector operator-(lvector x);
    double operator/ (lvector a);    //scalar product
    bool put_in_qvector_long(QVector<long> x);
    bool put_in_qvector_double(QVector<double> x);
    bool put_in_qlist_double(QList<double> x);
    long indexOf(double value, long from = 0 );
    double vector_sum();
    double squared_vector_sum();
    lvector append(double x);
    bool exp_moving_avg(double alpha);
    long size();
    lvector crop(long x, double value=0);
    lvector crop_first(long x, double value=0);
    long non_zero_elements();
    lvector sub(long start, long end);
    double avg();
    double variance();
    void del();
    QVector<QVariant> toQVectorQVariant();
    QVector<QString> toQVectorQString();
    QString toQString(QString separator);

    QString print(QString label_yes_or_no="yes", QList <bool>list=QList <bool>());

    QVector <double> vector;
    lvector remove_element(long i);

};
double norm(lvector a);
lvector operator +(double a,lvector x);
lvector operator -(double a,lvector x);
lvector operator *(double a,lvector x);
lvector operator^(lvector x, double a);
lvector operator /(double a,lvector x);
lvector exp(lvector x);
lvector log(lvector x);
lvector sinxoverx(lvector x);
lvector cos(lvector x);
lvector sin(lvector x);
lvector pow(lvector x,double a);
lvector absolute(lvector x);
lvector arctan(lvector x);
lvector randvector_2d(double r);
lvector normalize(lvector a);
lvector append(lvector a, lvector b);
lvector merge(QList<lvector> X);
bool polarcoordinates(lvector x, double& phi, double& r);
lvector qstringlist_to_lvector(QStringList X);


class lvectorfunction
{
public:
    lvectorfunction();
    lvectorfunction(long numberofpoints);
    lvectorfunction(lvector set_domain, lvector set_function);
    lvectorfunction operator+(lvectorfunction x);
    lvectorfunction operator-(lvectorfunction x);
    lvectorfunction operator->*(lvectorfunction x);
    lvectorfunction operator%(lvectorfunction x);
    lvectorfunction sub(long start, long end);
    long size();
    lvector function;
    lvector domain;
    lvectorfunction remove_element(long i);

};
lvectorfunction operator*(double a, lvectorfunction x);
lvectorfunction operator^(lvectorfunction x,double a);
lvectorfunction operator+(double a, lvectorfunction x);





lvectorfunction normalize_function (lvectorfunction x);
lvectorfunction absolute(lvectorfunction x);
lvectorfunction log(lvectorfunction x);
lvectorfunction cos(lvectorfunction x);
lvectorfunction sin(lvectorfunction x);
lvectorfunction exp(lvectorfunction x);










class Tvector: public lvector
{
public:
       double& x();
       double& y();
       double& z();
       Tvector();

       Tvector(lvector a);
       Tvector(double set_x,double set_y,double set_z);
       Tvector operator* (Tvector a);  //crossproduct
       Tvector operator+ (Tvector a);
       Tvector operator- (Tvector a);

       double operator/ (Tvector a);    //scalar product
       Tvector normalize(Tvector a);
};

Tvector projection_parallel(Tvector x,Tvector n);
Tvector projection_perpendicular(Tvector x,Tvector n);
double norm(Tvector a);
Tvector operator* (double x,Tvector a);
Tvector normalize(Tvector a);
Tvector randvector(double r);
Tvector rotation(double angle,Tvector ax, Tvector a);
Tvector rotation(Tvector x, Tvector a);
QList<Tvector> rotation_Mat(Tvector u);
QList<Tvector> rotation_Mat_from_a_to_b(Tvector a, Tvector b);
Tvector rotation_simple(Tvector x, Tvector v);
QList<double> get_list(Tvector X);
QList<double> get_list(QList<Tvector> X);
QList <Tvector> dlist_to_QLT(QList<double> X);
Tvector dlist_to_T(QList<double> X);
Tvector qstringlist_to_T(QStringList X);
double trace(QList <Tvector> X);
QList<Tvector> diag(double a, double b, double c);
QList <Tvector> rotation(double angle,Tvector ax,QList <Tvector> list);
QList <Tvector>  rotation(Tvector ax,QList <Tvector> list);
QList <Tvector>  rotation_simple(Tvector ax,QList <Tvector> list);
QList <Tvector>  operator^(QList <Tvector> a,QList <Tvector> b);
Tvector  operator^(QList <Tvector> a,Tvector b);
QList<Tvector>   operator^(Tvector a,Tvector b);
QList<Tvector>  cross(Tvector a);
QList <Tvector>  operator&(QList <Tvector> x,QList <Tvector> y);
Tvector  operator&(QList <Tvector> a,Tvector b);
QList <Tvector>  operator&(Tvector a,Tvector b);
QList <Tvector>  unitMat();
QList <Tvector>  unitMat_rank(int rank);
QList <Tvector>  zeroMat();
double Matrix_contraction(QList <Tvector> a,QList <Tvector> b);
QList <Tvector> Matrix_plus(QList <Tvector> a,QList <Tvector> b);
QList <Tvector> Matrix_minus(QList <Tvector> a,QList <Tvector> b);
QList <Tvector> operator-(QList <Tvector> a,QList <Tvector> b);
QList <Tvector> operator*(double a,QList <Tvector> b);

Tvector sphericalcoord(Tvector x);
Tvector sphericalcoord_normalized(Tvector y);
Tvector anggradofscalarproduct(Tvector u,Tvector x);
QList<Tvector> extend_orthogonal_vectors(QList <Tvector> A);
QList<Tvector> invert_matrix(QList <Tvector> A);
QList<Tvector> transpose(QList<Tvector> list);
double determinant(QList <Tvector> A);

QList<lvector> Tlist_to_Qdlist(QList<Tvector> list);
lvector Tvector_to_lvector(Tvector X);


class tvector_string{
public:
    QString string;
    Tvector vector;
    double parameter_for_string;
};



#endif // LVECTOR_H
