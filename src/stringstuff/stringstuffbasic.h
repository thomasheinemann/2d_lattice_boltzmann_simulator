
#ifndef STRINGSTUFFBASIC_H
#define STRINGSTUFFBASIC_H

#include <QStringList>
#include <QString>
#include <QList>
#include <QVector>
#include <stdio.h>
#include <iostream>

class estring
{
public:
    estring();
    estring(QString set_string,QList<double> set_param);
    estring(QString set_string,double set_param);

    bool operator==(estring x);
    QString string;
    QList<double> param;
};


QList<long> indexOfList(QString S, QStringList &list);
QList<QStringList> get_sublist(QList<long> indexlist, QList<QStringList> list);
long indexOf(QString S,QList<estring>& x);
QList<double> search_param(QString S,QList <estring> list);
double search_value(QString S,QList <estring>& list);

QString n_to_qstring_2(double x);
QString n_to_qstring(double x);
QString n_to_qstring(long x);

bool qstringlist_to_qlist_double (QStringList S,QList<double>& S2);
QList<double> qstringlist_to_qlist_double (QStringList S);

QString qstringlist_to_qstring(QStringList list,QString filling_string);
QStringList qlistdouble_to_qstringlist(QList<double> list);
QString qlistdouble_to_qstring(QList<double> list,QString filling_string);

QStringList erase_in_qstringlist_value(QStringList x,long a, long b);
QStringList erase_in_qstringlist_value(QStringList x,long a);
bool erase_in_qstringlist_bool(QStringList &x,long a, long b);
bool erase_in_qstringlist_bool(QStringList &x,long a);
QString add_filling_string_before_string(QString S,QString filling_string, QString marker_string);
QStringList create_list(QStringList text);

#endif // STRINGSTUFFBASIC_H
