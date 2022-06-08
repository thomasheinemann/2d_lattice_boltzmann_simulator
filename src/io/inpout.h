#ifndef INPOUT_H
#define INPOUT_H

#include <QTextStream>
#include <QFile>
#include <iostream>
#include <QDir>

#include "./math/mathstuffbasic.h"
#include "./math/lvector.h"

#include "./stringstuff/stringstuffbasic.h"



#include <QPixmap>
#include <QImage>
#include <QtGlobal>


class inpout{
public:
    bool create_dir(QString x,QString actual_dir="");
    bool make_executable(QString filename);
    bool create_executable_and_write_list(QString filename,QStringList list);
    bool xvg2tho(QString filenameinput,QString filenameoutput,QString separator="\t");
    bool normalize_file(QString filename);
    bool exists_file(QString filename);
    bool file_creatable(QString filename);
    bool proof_file(QString filename,QString content, QString exists_or_creatable);
    long Nrows(QString filename);
    bool remove(QString filename);
    bool change_attributes(QString filename_source,QString filename_destination,QStringList categories_to_change, QStringList new_values, QString tabsequence="\t");
    bool copy_file(QString filename1, QString filename2, long long pos=1);
    bool append_file(QString filename1, QString filename2, long long pos=1);
    QString read_line(QString filename, long line_number=1);
    bool plot_value(double x,QString filename);
    bool plot_vector(lvector x,QString filename);
    bool append_vector_h(lvector x,QString filename, QString separator="\t");
    bool plot_vector_h(lvector x,QString filename, QString separator="\t");
    bool append_value(double x,QString filename);
    bool append_string(QString S,QString filename);
    bool plot_string(QString S,QString filename);
    bool append_vector(lvector x,QString filename);
    bool append_vector_list(QList<lvector> x,QString filename);
    bool plot_function(lvectorfunction x, QString filename, QString separator="\t");
    bool line_wise_mean_variance(QString filename, lvector & vector_mean,lvector & vector_variance, long start_line_number, long start_column_number, QString separator, bool ignore_when_separator_first);
    QString read_parameters(QString filename, QString category);
    bool read_parameters(QString filename, QString category,QString& string);
    bool read_parameters(QString filename, QStringList& categories,QList<QStringList>& parameters);
    bool write_parameters(QString filename,QList<estring> X);
    bool plot_vector_list(QList<lvector> &list,QStringList categories,QString filename, QString separator=QString("\t"));
    bool plot_vector_list(QList<QVector <QString > > &list, QStringList categories, QString filename, QString separator=QString("\t"));

    bool get_vector(QString categories, QString filename, lvector& output, QString separator, bool file_with_header, long columnnumber=0, long numberofvaluedlinestoskip=0);
    bool get_vector_list(QStringList categories, QString filename, QVector<lvector>& output, QString separator, bool file_with_header, bool ignore_when_separator_first=false, long numberofvaluedlinestoskip=0);//=true);
    bool get_vector_list(QStringList categories, QString filename, QVector<QStringList>& output, QString separator, bool file_with_header, bool ignore_when_separator_first=false, long numberofvaluedlinestoskip=0);//=true);
    bool get_vector(QString filename, lvector& f, QString separator="\t", long columnnumber=0, long numberofvaluedlinestoskip=0);
    bool get_function(QString filename, lvectorfunction& f, QString separator="\t");

    bool qstringlist_to_file(QStringList X,QString filename);
    bool prepend_qstringlist_in_file(QStringList X,QString filename_input, QString filename_output);
    bool save_image(QPixmap map, QString filename, QString format, int quality);
    bool save_image(QImage map, QString filename, QString format, int quality);


};



#endif // INPOUT_H
