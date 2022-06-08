#include "./math/lvector.h"


lvector::lvector(){};


lvector::lvector(const lvector &x){
    vector=x.vector;
}

lvector::lvector(QVector<double> x){
    vector=x;
}

lvector::lvector(QList<double> x){
    vector=x.toVector();
}

void lvector::del(){
    vector.clear();
}

lvector lvector::remove_element(long i){
    vector.remove(i);
    return (*this);
}

lvector::lvector(long set_size,double default_value){

    vector=QVector<double> (set_size,default_value);

}

QList<double> lvector::get_list(){
    return vector.toList();
}

bool lvector::put_in_qvector_double(QVector<double> x){

    vector=x;
    return true;
}

bool lvector::put_in_qlist_double(QList<double> x){

    vector=x.toVector();
    return true;
}

bool lvector::put_in_qvector_long(QVector<long> x){
    QVector<double> y(x.size());
    for(long i=0;i<y.size();i++){
        y[i]=x[i];
    }
    vector=y;
    return true;
}

long lvector::indexOf(double value, long from){
    return vector.indexOf(value,from);
}

//lvector lvector::operator=(lvector a){
//    return a;
//}

double& lvector::operator[](long i){
    return vector[i];
}

lvector lvector::operator->*(lvector a){

    lvector temp=a;
    for (long i=0;i<a.size();i++){temp[i]=vector[i]* a[i];}

    return temp;
}

lvector lvector::operator%(lvector a){

    lvector temp=a;
    for (long i=0;i<a.size();i++){temp[i]=vector[i]/ a[i];}

    return temp;
}

lvector lvector::operator +(lvector a){

    lvector temp=a;
    for (long i=0;i<a.size();i++){temp[i]= vector[i]+a[i];}
    return temp;
}

lvector lvector::operator -(lvector a){

    lvector temp=a;
    for (long i=0;i<a.size();i++){temp[i]= vector[i]-a[i];}
    return temp;
}

double lvector::vector_sum(){
    double temp=0;
    for (long i=0;i<size();i++){
        temp+=vector[i];
    }
    return temp;

}
double lvector::squared_vector_sum(){
    double temp=0;
    for (long i=0;i<size();i++){
        temp+=vector[i]*vector[i];
    }
    return temp;

}
lvector lvector::append(double x){

    vector<<x;
    return (*this);
}

bool lvector::exp_moving_avg(double alpha){//y_{i+1}=y_i+\alpha \, (x_{i+1}-y_i)
    double y_old=vector[0];

    for (long i=0;i<vector.size()-1;i++){
        vector[i+1]=y_old+alpha*(vector[i+1]-y_old);
        y_old=vector[i+1];
    }
    return true;

}

long lvector::size(){
    return vector.size();
}

lvector lvector::crop(long x, double value){

    for(long i=x;i<vector.size();i++){
        vector[i]=value;
    }
    return (*this);
}

lvector lvector::crop_first(long x, double value){

    for(long i=0;i<x;i++){
        vector[i]=value;
    }
    return (*this);
}

long lvector::non_zero_elements(){

    long sum=0;
    for(long i=0;i<vector.size();i++){
        if (vector[i]!=0)sum++;
    }
    return sum;

}

lvector lvector::sub(long start, long end){
    lvector temp;
    temp.vector=vector.mid(start,end-start+1);
    return temp;
}

double lvector::avg(){
    return (vector_sum())/size();
}
double lvector::variance(){
    return squared_vector_sum()/size()-pow(avg(),2.0);
}

QVector<QVariant> lvector::toQVectorQVariant(){

    QVector<QVariant> x(size());

    for(long i=0;i<x.size();i++){
        x[i]=vector[i];
    }

    return x;
}

QVector<QString> lvector::toQVectorQString(){

    QVector<QString> x(size());

    for(long i=0;i<x.size();i++){

        x[i]=n_to_qstring(vector[i]);
    }

    return x;
}

QString lvector::toQString(QString separator){

    QVector<QString>  vec=this->toQVectorQString();

    QString final;
    for(long i=0;i<vec.size();i++){
        final=final+vec[i];
        if(i<vec.size()-1){final=final+separator;}
    }

    return final;
}


double lvector::operator/ (lvector a){/*scalar product*/
    double temp=0;
    for (long i=0; i<a.size();i++){
        temp+=vector[i]*a[i];
    }
    return temp;

}


QString lvector::print(QString label_yes_or_no, QList <bool>list){

    QString S;
    if(label_yes_or_no=="yes"){
        for(long i=0;i<vector.size();i++){


        bool A1=false;if (i<list.size())if (list[i])A1=true;
        bool A2=(list.size()==0);

            if (A1 || A2)S+=QString("Num ")+n_to_qstring(i)+": "+n_to_qstring(vector[i])+" ";
        }
    }else if(label_yes_or_no=="no"){
        for(long i=0;i<vector.size();i++){
            bool A1=false;if (i<list.size())if (list[i])A1=true;
            bool A2=(list.size()==0);

                if (A1 || A2)S+=n_to_qstring(vector[i])+" ";
        }
    }
    return S;

}

double norm(lvector a) {return pow(a/a,0.5);}

lvector operator+(double a, lvector x){

    lvector temp=x;
    for (long i=0;i<x.size();i++){temp[i]= a+x[i];}

    return temp;
}

lvector operator-(double a, lvector x){

    lvector temp=x;
    for (long i=0;i<x.size();i++){temp[i]= a-x[i];}

    return temp;
}

lvector operator*(double a, lvector x){

    lvector temp=x;
    for (long i=0;i<x.size();i++){temp[i]= a*x[i];}

    return temp;
}

lvector operator^(lvector x, double a){

    lvector temp=x;
    for (long i=0;i<x.size();i++){temp[i]= pow(x[i],a);}

    return temp;
}

lvector operator/(double a, lvector x){

    lvector temp=x;
    for (long i=0;i<x.size();i++){temp[i]= a/x[i];}

    return temp;
}

lvector exp(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=exp(x[i]);
    }
    return temp;

}

lvector log(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=log(x[i]);
    }
    return temp;

}
lvector sinxoverx(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=sinxoverx(x[i]);
    }
    return temp;

}
lvector cos(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=cos(x[i]);
    }
    return temp;

}
lvector sin(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=sin(x[i]);
    }
    return temp;

}

lvector pow(lvector x,double a){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=pow(x[i],a);
    }
    return temp;

}

lvector absolute(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=absolute(x[i]);
    }
    return temp;

}

lvectorfunction absolute(lvectorfunction x){

    lvectorfunction temp=x;

    for (long i=0; i<x.function.size();i++){
        temp.function[i]=absolute(x.function[i]);
    }
    return temp;

}

lvectorfunction log(lvectorfunction x){

    lvectorfunction temp=x;

    for (long i=0; i<x.function.size();i++){
        temp.function[i]=log(x.function[i]);
    }
    return temp;

}
lvectorfunction sin(lvectorfunction x){

    lvectorfunction temp=x;

    for (long i=0; i<x.function.size();i++){
        temp.function[i]=sin(x.function[i]);
    }
    return temp;

}

lvectorfunction exp(lvectorfunction x){

    lvectorfunction temp=x;

    for (long i=0; i<x.function.size();i++){
        temp.function[i]=exp(x.function[i]);
    }
    return temp;

}
lvectorfunction cos(lvectorfunction x){

    lvectorfunction temp=x;

    for (long i=0; i<x.function.size();i++){
        temp.function[i]=cos(x.function[i]);
    }
    return temp;

}
lvector arctan(lvector x){

    lvector temp=x;

    for (long i=0; i<x.size();i++){
        temp[i]=atan(x[i]);
    }
    return temp;

}
lvector normalize(lvector a){
        double n=norm(a);
        if (n==0) return lvector(a.size(),0);
        return (1.0/n)*a;
}

lvector append(lvector a, lvector b){
    lvector temp;
    temp.vector<<a.vector<<b.vector;
    return temp;
}

lvector merge(QList<lvector> X){
    lvector temp;
    for(long i=0;i<X.size();i++){
        temp=append(temp,X[i]);
    }
    return temp;
}

lvectorfunction::lvectorfunction(){}

lvectorfunction::lvectorfunction(long numberofpoints){
    function=lvector(numberofpoints,0);
    domain=lvector(numberofpoints,0);
}

lvectorfunction::lvectorfunction(lvector set_domain, lvector set_function){
    function=set_function;
    domain=set_domain;


}

lvectorfunction lvectorfunction::sub(long start, long end){

    return lvectorfunction(domain.sub(start,end),function.sub(start,end));

}

lvectorfunction lvectorfunction::remove_element(long i){
    return lvectorfunction(domain.remove_element(i), function.remove_element(i));




}

lvectorfunction lvectorfunction::operator+(lvectorfunction x){

    return lvectorfunction(x.domain,function+x.function);

}
lvectorfunction lvectorfunction::operator-(lvectorfunction x){

    return lvectorfunction(x.domain,function-x.function);

}
lvectorfunction lvectorfunction::operator->*(lvectorfunction x){

    return lvectorfunction(x.domain,function->*x.function);

}
lvectorfunction lvectorfunction::operator%(lvectorfunction x){

    return lvectorfunction(x.domain,function % x.function);

}
long lvectorfunction::size(){
    return this->domain.size();
}

lvectorfunction operator*(double a, lvectorfunction x){
    return lvectorfunction(x.domain,a*x.function);
}

lvectorfunction operator^(lvectorfunction x,double a){
    return lvectorfunction(x.domain,(x.function) ^ a);
}

lvectorfunction operator+(double a, lvectorfunction x){
    return lvectorfunction(x.domain,a+x.function);
}


lvector randvector_2d(double r){

    double phi=rnd(M_PI*2);


    return lvector(QList<double>()<<r*cos(phi)<<r*sin(phi));

}





lvectorfunction normalize_function (lvectorfunction x){

    return lvectorfunction(x.domain,normalize(x.function));
}

//Tvector

Tvector::Tvector(){vector=QVector<double> (3,0);x()=0;y()=0;z()=0;};


Tvector::Tvector(lvector a){

    vector= QVector<double>(3,0);
    x()=a[0];
    y()=a[1];
    z()=a[2];

}


double& Tvector::x()  {  return vector[0];}
double& Tvector::y()  {  return vector[1];}
double& Tvector::z()  {  return vector[2];}


Tvector::Tvector(double set_x,double set_y,double set_z){
    vector=QVector<double>(3,0);
      x()=set_x;
      y()=set_y;
      z()=set_z;
}



Tvector Tvector::operator* (Tvector a) {return Tvector (y()*a.z()-z()*a.y(),z()*a.x()-x()*a.z(),x()*a.y()-y()*a.x());}//cross product
Tvector Tvector::operator+ (Tvector a) {return Tvector(x()+a.x(),y()+a.y(),z()+a.z());}
Tvector Tvector::operator- (Tvector a) {return Tvector(x()-a.x(),y()-a.y(),z()-a.z());}

double Tvector::operator/ (Tvector a){   return a.x()*x()+a.y()*y()+a.z()*z();     }//scalar product

Tvector operator* (double x,Tvector a){return Tvector(a.x()*x,a.y()*x,a.z()*x);}
Tvector projection_parallel(Tvector x,Tvector n){
    n=normalize(n);
    return (x/n)*n;

}

Tvector projection_perpendicular(Tvector x,Tvector n){
    n=normalize(n);
    return x-(x/n)*n;

}

double norm(Tvector a) {return double (pow((a.x()*a.x())+(a.y()*a.y())+(a.z()*a.z()),0.5));}


Tvector normalize(Tvector a){
        double n=norm(a);
        if (n==0) return Tvector(0,0,0);
        return Tvector(a.x()/n,a.y()/n,a.z()/n);
}


Tvector randvector(double r){

              double phi=rnd(M_PI*2);
              double theta=rnd(M_PI);theta/=sin(theta);
              Tvector temp=(r*Tvector(cos(phi)*sin(theta),sin(phi)*sin(theta),cos(theta)*1));
              return temp  ;
}

Tvector rotation(double angle,Tvector ax, Tvector a){
  if ((angle==0)|| (norm(ax)==0))return a;
  Tvector axis=normalize(ax);
  return (axis/a)*axis+cos(angle)*((axis*a)*axis)+sin(angle)*(axis*a);
}

Tvector rotation(Tvector temp, Tvector v){// v around temp with angle asin(|temp|)
    double xnorm=norm(temp);   //xnorm is always positive!!
    if (xnorm==0)return v;
    if (xnorm>1)xnorm=1;       //for rotations smaller 90 degrees! for rotations larger than 90 degrees temp is not enough!
    Tvector axis=(1.0/xnorm)*temp;
    return (axis/v)*axis+pow(1.0-xnorm*xnorm,0.5)*((axis*v)*axis)+(temp*v);
}

Tvector rotation_simple(Tvector temp, Tvector v){
    return v+(temp*v);
}

QList<Tvector> rotation_Mat(Tvector u){ //(0,0,1) -> u
//careful!
    Tvector temp=Tvector(0,0,1)*normalize(u);

    double xnorm=norm(temp);   //xnorm is always positive!!
    if (xnorm==0)return unitMat();
    Tvector axis=(1.0/xnorm)*temp;

    double theta=acos(Tvector(0,0,1)/normalize(u));

    return Matrix_plus((cos(theta)*unitMat()),Matrix_plus(sin(theta)*cross(axis), (1.0-cos(theta))*(axis^axis)));
}

QList<Tvector> rotation_Mat_from_a_to_b(Tvector a, Tvector b){ //a -> b
//careful!
    Tvector temp=normalize(a)*normalize(b);

    double xnorm=norm(temp);   //xnorm is always positive!!
    if (xnorm<1e-50)return unitMat();
    Tvector axis=(1.0/xnorm)*temp;
    double theta=acos(normalize(a)/normalize(b));
    //std::cout<<"theta: "<<theta<<std::endl;

    return Matrix_plus((cos(theta)*unitMat()),Matrix_plus(sin(theta)*cross(axis), (1.0-cos(theta))*(axis^axis)));
}


QList <Tvector>  rotation(double angle,Tvector ax,QList <Tvector> list){
//careful!
    QList <Tvector> temp;temp<<rotation(angle,ax,list[0])<<rotation(angle,ax,list[1])<<rotation(angle,ax,list[2]);
    static QList <Tvector> unitmatrix=unitMat();
    QList <Tvector> R_inv;R_inv<<rotation(-angle,ax,unitmatrix[0])<<rotation(-angle,ax,unitmatrix[1])<<rotation(-angle,ax,unitmatrix[2]);
    temp=(temp&R_inv);

    return temp;
}


QList <Tvector>  rotation(Tvector ax,QList <Tvector> list){
//careful!
    QList <Tvector> temp;temp<<rotation(ax,list[0])<<rotation(ax,list[1])<<rotation(ax,list[2]);
    Tvector ax2=(-1)*ax;
    static QList <Tvector> unitmatrix=unitMat();
    QList <Tvector> R_inv;R_inv<<rotation(ax2,unitmatrix[0])<<rotation(ax2,unitmatrix[1])<<rotation(ax2,unitmatrix[2]);
    temp=(temp&R_inv);

    return temp;
}

QList <Tvector>  rotation_simple(Tvector ax,QList <Tvector> list){
//careful!
    QList <Tvector> temp;temp<<rotation_simple(ax,list.at(0))<<rotation_simple(ax,list.at(1))<<rotation_simple(ax,list.at(2));
    static QList <Tvector> unitmatrix=unitMat();
    Tvector ax2=(-1)*ax;
    QList <Tvector> R_inv;R_inv<<rotation_simple(ax2,unitmatrix[0])<<rotation_simple(ax2,unitmatrix[1])<<rotation_simple(ax2,unitmatrix[2]);
    temp=(temp&R_inv);

    return temp;
}

QList<double> get_list(Tvector X){
    return get_list((QList<Tvector>()<<X));
}

QList<double> get_list(QList<Tvector> X){
    QList<double> temp;
    for(long i=0;i<X.size();i++){
        temp<<X[i][0]<<X[i][1]<<X[i][2];
    }
    return temp;
}

QList <Tvector> dlist_to_QLT(QList<double> X){
    QList<Tvector> temp;
    for(long i=0;(i+2<X.size());i+=3){
        temp<<Tvector(X[i+0],X[i+1],X[i+2]);
    }
    return temp;
}

Tvector dlist_to_T(QList<double> X){

    return Tvector(X[0],X[1],X[2]);

}

Tvector qstringlist_to_T(QStringList X){

    return dlist_to_T(qstringlist_to_qlist_double(X));

}


double trace(QList <Tvector> X){
    return X[0][0]+X[1][1]+X[2][2];
}

QList<Tvector> diag(double a, double b, double c){
    QList <Tvector> temp;

    temp<<Tvector(a,0,0)
        <<Tvector(0,b,0)
        <<Tvector(0,0,c);
    return temp;
}

QList <Tvector>  operator^(QList <Tvector> a,QList <Tvector> b){//row matrix times col matrix = col matrix
    QList <Tvector> temp;

    temp<<Tvector(a[0]/b[0],a[1]/b[0],a[2]/b[0])
        <<Tvector(a[0]/b[1],a[1]/b[1],a[2]/b[1])
        <<Tvector(a[0]/b[2],a[1]/b[2],a[2]/b[2]);
    return temp;

}

Tvector  operator^(QList <Tvector> a,Tvector b){//row matrix times col vector = vector


    return Tvector(a[0]/b,a[1]/b,a[2]/b);
}

QList<Tvector>  operator^(Tvector a,Tvector b){//col vector times row vector = col matrix
    QList <Tvector> temp;

    temp<<Tvector(a.x()*b.x(),a.y()*b.x(),a.z()*b.x())
            <<Tvector(a.x()*b.y(),a.y()*b.y(),a.z()*b.y())
            <<Tvector(a.x()*b.z(),a.y()*b.z(),a.z()*b.z());
    return temp;
}

QList<Tvector>  cross(Tvector a){//col vector times row vector = col matrix
    QList <Tvector> temp;

    temp<<Tvector(0,a.z(),-a.y())
        <<Tvector(-a.z(),0,a.x())
        <<Tvector(a.y(),-a.x(),0);
    return temp;
}

QList <Tvector>  operator&(QList <Tvector> a,QList <Tvector> b){//col matrix times col matrix = col matrix
    //matrix-matrix multiplication 3X3 times 3X3

        QList <Tvector> a2;
        a2<<Tvector(a[0].x(),a[1].x(),a[2].x())
            <<Tvector(a[0].y(),a[1].y(),a[2].y())
            <<Tvector(a[0].z(),a[1].z(),a[2].z());

        QList <Tvector> temp;

        temp<<Tvector(a2[0]/b[0],a2[1]/b[0],a2[2]/b[0])
            <<Tvector(a2[0]/b[1],a2[1]/b[1],a2[2]/b[1])
            <<Tvector(a2[0]/b[2],a2[1]/b[2],a2[2]/b[2]);

        return temp;
}

Tvector  operator&(QList <Tvector> a,Tvector b){//col matrix times col vector =  col vector
    //matrix-matrix multiplication 3X3 times 3X1

        QList <Tvector> a2;

        a2<<Tvector(a[0].x(),a[1].x(),a[2].x())
          <<Tvector(a[0].y(),a[1].y(),a[2].y())
          <<Tvector(a[0].z(),a[1].z(),a[2].z());

        return Tvector(a2[0]/b,a2[1]/b,a2[2]/b);
}

QList <Tvector>  operator&(Tvector a,Tvector b){//col vector times row vector =  col matrix
    //matrix-matrix multiplication

        QList <Tvector> temp;

        temp<<Tvector(a.x()*b.x(),a.y()*b.x(),a.z()*b.x())
            <<Tvector(a.x()*b.y(),a.y()*b.y(),a.z()*b.y())
            <<Tvector(a.x()*b.z(),a.y()*b.z(),a.z()*b.z());
        return temp;
}

QList <Tvector>  unitMat(){
    static QList <Tvector> temp=(QList<Tvector>()<<Tvector(1,0,0)<<Tvector(0,1,0)<<Tvector(0,0,1));
    return temp;
}
QList <Tvector>  unitMat_rank(int rank){
    if (rank==3) return (QList<Tvector>()<<Tvector(1,0,0)<<Tvector(0,1,0)<<Tvector(0,0,1));
    if (rank==2) return (QList<Tvector>()<<Tvector(1,0,0)<<Tvector(0,1,0)<<Tvector(0,0,0));
    if (rank==1) return (QList<Tvector>()<<Tvector(1,0,0)<<Tvector(0,0,0)<<Tvector(0,0,0));
    return (QList<Tvector>()<<Tvector(0,0,0)<<Tvector(0,0,0)<<Tvector(0,0,0));
}
QList <Tvector>  zeroMat(){
    static QList <Tvector> temp=QList<Tvector>()<<Tvector(0,0,0)<<Tvector(0,0,0)<<Tvector(0,0,0);
    return temp;
}

double Matrix_contraction(QList <Tvector> a,QList <Tvector> b){
    Tvector temp=(a[0]->*b[0])+(a[1]->*b[1])+(a[2]->*b[2]);

    return temp[0]+temp[1]+temp[2];
}

QList <Tvector> Matrix_plus(QList <Tvector> a,QList <Tvector> b){
    QList <Tvector> temp;temp<<(a[0]+b[0])<<(a[1]+b[1])<<(a[2]+b[2]);

    return temp;
}

QList <Tvector> Matrix_minus(QList <Tvector> a,QList <Tvector> b){
    QList <Tvector> temp;temp<<(a[0]-b[0])<<(a[1]-b[1])<<(a[2]-b[2]);

    return temp;
}

QList <Tvector> operator-(QList <Tvector> a,QList <Tvector> b){
    QList <Tvector> temp;temp<<(a[0]-b[0])<<(a[1]-b[1])<<(a[2]-b[2]);

    return temp;
}

QList <Tvector> operator*(double a,QList <Tvector> b){
    QList <Tvector> temp;temp<<a*b[0]<<a*b[1]<<a*b[2];

    return temp;
}



bool polarcoordinates(lvector x, double& phi, double& r){

    std::complex<double> temp(x[0],x[1]);
    phi=arg(temp);
    r=norm(x);

    return true;
}

Tvector sphericalcoord(Tvector x){

    //r-coordinate

    double r=norm(x);if (r==0)return Tvector(0,0,0);
    Tvector y=(1.0/r)*x;
    //theta
    double theta=acos(y.z());
    //phi
    double phi;
    std::complex<double> temp(y.x(),y.y());
    if(sin(theta)==0) phi=0; else phi=arg(temp);

    return Tvector(r,phi,theta);

}

Tvector sphericalcoord_normalized(Tvector y){

    //theta
    double theta=acos(y.z());
    //phi
    double phi;
    std::complex<double> temp(y.x(),y.y());
    if(sin(theta)==0) phi=0; else phi=arg(temp);

    return Tvector(0,phi,theta);

}

Tvector anggradofscalarproduct(Tvector u,Tvector x){

    Tvector temp=sphericalcoord(u);
    double phi=temp.y();
    double theta=temp.z();

    Tvector ephi=Tvector(-sin(phi),cos(phi),0);
    Tvector etheta=Tvector(cos(phi)*cos(theta),sin(phi)*cos(theta),-sin(theta));

    return ((ephi/x)*ephi)+((etheta/x)*etheta);
}

QList<Tvector> extend_orthogonal_vectors(QList <Tvector> A){

    Tvector v1=normalize(A[0]);Tvector v2=normalize(projection_perpendicular(A[1],v1));
    return A;
}

QList<Tvector> invert_matrix(QList <Tvector> A){
    QList<Tvector> B;
    B<<A[1]*A[2]<<A[2]*A[0]<<A[0]*A[1];
    return (1.0/determinant(A))*transpose(B);
}

double determinant(QList <Tvector> A){
    return A[0]/(A[1]*A[2]);
}

QList<Tvector> transpose(QList<Tvector> list){
    return list^unitMat();

}

QList<lvector> Tlist_to_Qdlist(QList<Tvector> list){
    QList <lvector> list2;
    static lvector temp;
    for (long i=0;i<list.size();i++){
        QVector<double> x(3);
        x[0]=list[i][0];
        x[1]=list[i][1];
        x[2]=list[i][2];

        temp.put_in_qvector_double(x);
        list2<<temp;
    }
    return list2;
}

lvector qstringlist_to_lvector(QStringList X){
    lvector temp(X.size(),0);
    for(long i=0;i<X.size();i++){
        temp[i]=X[i].toDouble();
    }
    return temp;
}


lvector Tvector_to_lvector(Tvector X){
    lvector Y(3,0);



        Y[0]=X[0];
        Y[1]=X[1];
        Y[2]=X[2];

    return Y;
}
