#include "./stringstuff/stringstuffbasic.h"

estring::estring(){}

estring::estring(QString set_string,QList<double> set_param){
    string=set_string;
    param=set_param;

}

estring::estring(QString set_string,double set_param){
    string=set_string;
    param<<set_param;

}

bool estring::operator==(estring x){
    return (this->param==x.param) && (this->string==x.string);
}

QList<long> indexOfList(QString S, QStringList &list){
    long index=0;
    QList<long> output;
    while(index!=-1){
        index=list.indexOf(S,index);
        if (index!=-1){output<<index;index++;}
    }
    return output;
}

QList<QStringList> get_sublist(QList<long> indexlist, QList<QStringList> list){

    QList<QStringList> sublist;
    for(long i=0;i<indexlist.size();i++)  sublist<<list[indexlist[i]];
    return sublist;
}

long indexOf(QString S,QList<estring>& x){
    for(long i=0;i<x.size();i++){
        if(x[i].string==S){
            return i;
        }
    }
    return -1;
}

QList<double> search_param(QString S,QList <estring> list){
    for(long i=0;i<list.size();i++){
        if(list[i].string==S){
            return list[i].param;
        }
    }

    QList<double> dummy;
    return dummy;

}

double search_value(QString S,QList <estring>& list){
    for(long i=0;i<list.size();i++){
        if(list[i].string==S){
            return list[i].param[0];
        }
    }
    return 0;
}

QString n_to_qstring_2(double x){
  QString S;
  return S.setNum(x,'e',5);
}

QString n_to_qstring(double x){
  QString S;
  return S.setNum(x,'g',15);
}

QString n_to_qstring(long x){
    QString S;
    return S.setNum(x);
}

QList<double> qstringlist_to_qlist_double (QStringList S){

    QList<double> x;

    for (long i=0;i<S.size();i++){
        x<<S[i].toDouble();
    }
    return x;
}

bool qstringlist_to_qlist_double (QStringList S,QList<double>& S2){

    QList<double> x;
    bool test;
    for (long i=0;i<S.size();i++){
        x<<S[i].toDouble(&test);
        if(test==false)return false;
    }
    S2=x;
    return true;
}

QString qstringlist_to_qstring(QStringList list,QString filling_string){

    QString S;

    for(long i=0;i<list.size();i++){
        S=S+list[i];
        if ((i>=0)&&i<list.size()-1)S=S+filling_string;
    }

    return S;
}

QStringList qlistdouble_to_qstringlist(QList<double> list){
    QStringList Slist;

    for(long i=0;i<list.size();i++){
        Slist<<n_to_qstring(list[i]);

    }

    return Slist;
}

QString qlistdouble_to_qstring(QList<double> list,QString filling_string){

    QString S;

    for(long i=0;i<list.size();i++){
        S=S+n_to_qstring(list[i]);
        if ((i>=0)&&i<list.size()-1)S=S+filling_string;
    }

    return S;

}

QStringList erase_in_qstringlist_value(QStringList x,long a, long b){

    for (long i=0;i<=b-a;i++){
        x.removeAt(a);
    }

    return x;
}

QStringList erase_in_qstringlist_value(QStringList x,long a){

    return erase_in_qstringlist_value(x,a,a);

}

bool erase_in_qstringlist_bool(QStringList &x,long a, long b){

    for (long i=0;i<=b-a;i++){
        x.removeAt(a);
    }
    return true;
}

bool erase_in_qstringlist_bool(QStringList &x,long a){
    return erase_in_qstringlist_bool(x, a, a);
}

QString add_filling_string_before_string(QString S,QString filling_string, QString marker_string){
    long string_index=S.lastIndexOf(marker_string);
    if (string_index!=-1)     return S.insert(string_index,filling_string); else return S+filling_string;
}

QStringList create_list(QStringList text){

    QStringList x;

    for (long i=0;i<text.size();i++){
        if (text.at(i)!="") {
            x<<text.at(i);
        }
    }
    return x;
}
