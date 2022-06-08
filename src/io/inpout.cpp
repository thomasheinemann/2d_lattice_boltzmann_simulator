#include "./io/inpout.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
namespace Qt
{
    //static auto endl = ::endl;
    static auto SkipEmptyParts = QString::SkipEmptyParts;
}
#endif

bool inpout::create_dir(QString x,QString actual_dir){
    if (actual_dir!=""){
        QDir dummy(actual_dir);
        return dummy.mkdir(x);
    }else{
        QDir dummy(0);
        return dummy.mkdir(x);
    }
}
bool inpout::make_executable(QString filename){
    QFile file(filename);
    file.open(QIODevice::Append);
    file.setPermissions(QFile::ExeOther|QFile::WriteOther|QFile::ReadOther|QFile::ExeGroup|QFile::WriteGroup|QFile::ReadGroup|QFile::ExeUser|QFile::WriteUser|QFile::ReadUser|QFile::ExeOwner|QFile::WriteOwner|QFile::ReadOwner);
    file.close();
    return true;
}
bool inpout::create_executable_and_write_list(QString filename,QStringList list){


    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.setPermissions(QFile::ExeOther|QFile::WriteOther|QFile::ReadOther|QFile::ExeGroup|QFile::WriteGroup|QFile::ReadGroup|QFile::ExeUser|QFile::WriteUser|QFile::ReadUser|QFile::ExeOwner|QFile::WriteOwner|QFile::ReadOwner);

    for (long j=0;j<list.size();j++){
        file.write((list[j]+"\n").toLocal8Bit().constData());
    }
    file.close();


    return true;


}

bool inpout::xvg2tho(QString filenameinput,QString filenameoutput,QString separator){

    remove(filenameoutput);

    QFile file(filenameinput);
    QTextStream in(&file);
    file.open(QIODevice::ReadOnly);
    long rownumber=0;

    while(!in.atEnd()){

        rownumber++;
        QString S=in.readLine();
        if (S.left(1)!=QString("#") && S.left(1)!=QString("@")) {
            S.replace(QString(" "), separator);
            QStringList Slist= S.split(separator,Qt::SkipEmptyParts);
            S=Slist.join(separator);
            append_string(S+"\n",filenameoutput);
        }


    }

    file.close();

    return true;
}


bool inpout::normalize_file(QString filename){

    //long N_rows=Nrows(filename);
    QFile file(filename);QTextStream stream(&file);file.open(QIODevice::ReadOnly);


    QString filename_temp=add_filling_string_before_string(filename,"temp",".");
    QFile file_temp(filename_temp);file_temp.open(QIODevice::WriteOnly);

    double value=0;
    while(!stream.atEnd()){
        value+=stream.readLine().toDouble();

    }

    //prevent 0/0
        if (value==0)value=1;
    //

    file.close();
    file.open(QIODevice::ReadOnly);
    QTextStream stream2(&file);

    while(!stream2.atEnd()){

        file_temp.write((n_to_qstring(stream2.readLine().toDouble()/double(value))+"\n").toLocal8Bit().constData());

    }
    file_temp.close();

    copy_file(filename_temp,filename);

    remove(filename_temp);





    return true;



}



bool inpout::exists_file(QString filename){
    QFile file(filename);
    bool test=file.open(QIODevice::ReadOnly);
    file.close();
    return test;
}
bool inpout::file_creatable(QString filename){
    if (exists_file(filename))return true;
    QFile file(filename);
    bool test=file.open(QIODevice::WriteOnly);
    file.close();
    remove(filename);
    return test;
}
bool inpout::proof_file(QString filename,QString content, QString exists_or_creatable){
    bool testthis;

    if (exists_or_creatable=="exists?"){
        testthis=exists_file(filename);
        if (!testthis)std::cout<<"invalid: "<<content.toStdString()<<"   "<<filename.toStdString()<<std::endl;

    }else{// if (exists_or_creatable=="creatable?"){
        testthis=file_creatable(filename);
        if (!testthis)std::cout<<"invalid: "<<content.toStdString()<<"   "<<filename.toStdString()<<std::endl;

    }
    return testthis;

}

long inpout::Nrows(QString filename){


    QFile file(filename);

    QTextStream in(&file);
    file.open(QIODevice::ReadOnly);
    long rownumber=0;
    while(!in.atEnd()){in.readLine();rownumber++;}

    file.close();

    return rownumber;

}

bool inpout::remove(QString filename){
    QFile file(filename);
    bool test=file.remove();
    file.close();
    return test;
}

bool inpout::copy_file(QString filename1, QString filename2, long long pos){
    if (exists_file(filename2))remove(filename2);
    QFile file1(filename1);







    if (pos==1){
        remove(filename2);
        file1.copy(filename2);
    }else {
        file1.open(QIODevice::ReadOnly);
        QTextStream stream1(&file1);
        QFile file2(filename2);file2.open(QIODevice::WriteOnly);
        long long i=0;
        while(!stream1.atEnd()){
            i++;
            QString S=stream1.readLine()+"\n";
            if (i>=pos){
                file2.write(S.toLocal8Bit().constData());i=pos;
            }

        }
        file2.close();

    }
    file1.close();
    return true;
}

bool inpout::append_file(QString filename1, QString filename2, long long pos){

    //if (exists_file(filename2))remove(filename2);
    QFile file1(filename1);

    file1.open(QIODevice::ReadOnly);
    QTextStream stream1(&file1);
    QFile file2(filename2);file2.open(QIODevice::Append);
    long long i=0;
    while(!stream1.atEnd()){
        i++;
        QString S=stream1.readLine()+"\n";
            if (i>=pos){
                file2.write(S.toLocal8Bit().constData());i=pos;
            }

        }
        file2.close();


    file1.close();
    return true;
}

bool inpout::qstringlist_to_file(QStringList X,QString filename){

       QFile file(filename);file.open(QIODevice::WriteOnly);
       for(long i=0;i<X.size();i++){
           QString S=X[i];
           file.write(S.toLocal8Bit().constData());
       }
        file.close();
        return true;


}

bool inpout::prepend_qstringlist_in_file(QStringList X,QString filename_input, QString filename_output){



    qstringlist_to_file(X,filename_output);
    append_file(filename_input,filename_output);

    return true;
}
QString inpout::read_line(QString filename, long line_number){

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    long i=0;
    while (!in.atEnd()) {
        i++;
        QString line = in.readLine();
        file.close();
        if (i==line_number)return line;

    }
    file.close();
    return "";

}
bool inpout::line_wise_mean_variance(QString filename, lvector & vector_mean,lvector & vector_variance, long start_line_number, long start_column_number,QString separator, bool ignore_when_separator_first){

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);








    //
        QList <double> list_mean;
        QList <double> list_variance;
        long i=0;
        while (!in.atEnd()) {
            i++;
            QString line = in.readLine();

            if(i>=start_line_number){
            /*long le=0;QString doublesep=separator+separator;
            while(le!=line.length()){
                le=line.length();
                line.replace(doublesep,separator);
                std::cout<<i<<"  "<<line.toLocal8Bit().constData()<<std::endl;
            }*/
            QStringList columns =line.split(separator);

            columns.removeAll(QString(""));
            //if(columns.last()=="")columns.removeLast();
            if (ignore_when_separator_first && columns[0]=="")columns.removeFirst();
            for(long k=1;k<start_column_number;k++)columns.removeFirst();


            lvector temp;
            temp.put_in_qvector_double(qstringlist_to_qlist_double(columns).toVector());
            //std::cout<<temp[0]<<"  "<<columns.size()<<std::endl;
            list_mean<<temp.avg();
            list_variance<<temp.variance();

            }

        }
    //


    vector_mean.put_in_qvector_double(list_mean.toVector());
    vector_variance.put_in_qvector_double(list_variance.toVector());
    file.close();
    return true;

}
QString inpout::read_parameters(QString filename, QString category){

    QList<QStringList> parameters;
    QStringList categories;categories<<category;


    read_parameters(filename, categories,parameters);

    return parameters[0][1];



}
bool inpout::read_parameters(QString filename, QString category,QString& string){

    QList<QStringList> parameters;
    QStringList categories;categories<<category;


    if (read_parameters(filename, categories,parameters)){
        string =parameters[0][1];
        return true;
    }

    return false;




}

bool inpout::read_parameters(QString filename, QStringList& categories,QList<QStringList>& parameters){

    QList<QStringList> vector;

    //store all lines lines in vector whose categories match categories
        QFile file_parameters(filename);
        bool testthis=true;
        testthis=testthis&&    file_parameters.open(QIODevice::ReadOnly);
        if (!testthis) return false;

        QTextStream in(&file_parameters);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList columns =line.split("\t");

            if(categories.contains(columns[0])==true){

                vector<<columns;
            }
        }
        file_parameters.close();
    //
//TEST
//QStringList testz=(QStringList()<<"ert"<<"uiojm"<<"ert");
//std::cout<<"fghjk"<<indexOfList("ert",testz)[0]<<std::endl;
    //cremove multiple entries (only the first are taken)
        QList<QStringList> vector2;
        QStringList categories2;
        for(long i=0;i<vector.size();i++){

            //one exception - sorry ;)
                bool test=false;
//TEST
std::cout<<"testi: "<<vector[i][0].toLocal8Bit().constData()<<"        "<<vector[i][1].toLocal8Bit().constData()<<std::endl;
            if(vector[i][0]=="simulation: define particle type" && categories2.contains(vector[i][0])==true){
                QList<long> indexlist=indexOfList(vector[i][0],categories2);
                for(long j=0;j<indexlist.size();j++){

                    if(vector2[indexlist[j]][1]==vector[i][1])test=true;
//TEST
                    std::cout<<vector[i][0].toLocal8Bit().constData()<<"  "<<vector2[indexlist[j]][1].toLocal8Bit().constData()<<"  "<<vector[i][1].toLocal8Bit().constData()<<"  "<<test<<indexlist.size()<<std::endl;
                }
            }
//TEST
        std::cout<<"fertsch"<<std::endl;
        //
            if((categories2.contains(vector[i][0])==false) || ((categories2.contains(vector[i][0])==true) && !test)){categories2<<vector[i][0];vector2<<vector[i];}
        }
    //

    //try if all categories are in file
        for(long i=0;i<categories.size();i++){
            testthis=testthis && categories2.contains(categories[i]);
            std::cout<<categories[i].toStdString();
            if(categories2.contains(categories[i])){std::cout<<"  "<<"true"<<std::endl;}else{std::cout<<"  "<<"false"<<std::endl;}
        }
        //if (testthis==false)return false;
    //

    //look if there is at least 1 entry per category
        for(long i=0;i<categories2.size();i++)testthis=testthis && (vector[i].size()>1);
        //if (testthis==false)return false;
    //

    categories=categories2;

    parameters=vector2;
    return testthis;
}

bool inpout::write_parameters(QString filename,QList<estring> X){
    QFile file_write(filename);
    file_write.open(QIODevice::WriteOnly);

    for(long i=0;i<X.size();i++){

        QString S=X[i].string+"\t"+qlistdouble_to_qstring(X[i].param,"\t");
        file_write.write(S.toLocal8Bit().constData());


    }



    file_write.close();
    return true;
}


bool inpout::change_attributes(QString filename_source,QString filename_destination,QStringList categories_to_change, QStringList new_values, QString tabsequence){

    QFile file_read(filename_source);
    file_read.open(QIODevice::ReadOnly);

    QFile file_write(filename_destination);
    file_write.open(QIODevice::WriteOnly);


    QTextStream in(&file_read);
    while (!in.atEnd()) {
        QString line = in.readLine();
        long index=-1;
        long i=0;

        while(i<categories_to_change.size() && index==-1)    {

            if (line.startsWith(categories_to_change[i]+tabsequence))index=i;
            i++;
        }




        if (index!=-1) {
            file_write.write((categories_to_change[index]+tabsequence+new_values[index]+"\n").toLocal8Bit().constData());
        }else{
            file_write.write((line+"\n").toLocal8Bit().constData());
        }
    }
    file_write.close();
    file_read.close();


    return true;

}

bool inpout::plot_vector_list(QList<QVector <QString > > &list,QStringList categories,QString filename, QString separator){

    QFile file_out(filename);
    file_out.remove();
    file_out.open(QIODevice::WriteOnly);
    long numberofcols=list.size();
    QString categoryline;
    for (long i=0;i<categories.size();i++){categoryline+=categories[i];if (i<categories.size()-1){categoryline+=QString("\t");}}

    file_out.write((categoryline+"\n").toLocal8Bit().constData());


    for (long row=0;row<list[0].size();row++){

        QString string;
        for (long col=0;col<numberofcols;col++){
            if(list[col].size()>row){
                QString S;

                string+=list[col][row];
                if (col<numberofcols-1)string+=separator;
            }
        }
        file_out.write((string+"\n").toLocal8Bit().constData());
    }


    file_out.close();
    std::cout<<"saved: "<<filename.toStdString()<<std::endl;
    return true;

}


bool inpout::plot_vector_list(QList<lvector> &list, QStringList categories, QString filename, QString separator){

    QFile file_out(filename);
    file_out.remove();
    file_out.open(QIODevice::WriteOnly);
    long numberofcols=list.size();
    QString categoryline;
    for (long i=0;i<categories.size();i++){categoryline+=categories[i];if (i<categories.size()-1){categoryline+=QString("\t");}}
    if(categories.size()>0)file_out.write((categoryline+"\n").toLocal8Bit().constData());
    for (long row=0;row<list[0].size();row++){

        QString string;
        for (long col=0;col<numberofcols;col++){
            if(list[col].size()>row){
                string+=n_to_qstring(list[col][row]);
                if (col<numberofcols-1)string+=separator;
            }
        }
        file_out.write((string+"\n").toLocal8Bit().constData());
    }


    file_out.close();
    std::cout<<"saved: "<<filename.toStdString()<<std::endl;
    return true;

}


bool inpout::get_vector(QString categories, QString filename,lvector& output,QString separator, bool file_with_header, long columnnumber, long numberofvaluedlinestoskip){

    QVector<lvector> temp(1);
    get_vector_list((QStringList()<< categories), filename,temp, separator, file_with_header,numberofvaluedlinestoskip);
    output=temp[columnnumber];

    return true;


}

//for reading numerical values
bool inpout::get_vector_list(QStringList categories, QString filename,QVector<lvector>& output, QString separator, bool file_with_header, bool ignore_when_separator_first, long numberofvaluedlinestoskip){


    QVector<QStringList> temp;
    get_vector_list(categories, filename,temp,separator, file_with_header,ignore_when_separator_first,numberofvaluedlinestoskip);

    ulong size=temp.size();

    output=QVector<lvector>(size,lvector(temp[0].size(),0));



    for (ulong i=0;i<size;i++){
        output[i].put_in_qvector_double(qstringlist_to_qlist_double(temp[i]).toVector());
    }

    return true;

}

//for reading strings
bool inpout::get_vector_list(QStringList categories, QString filename,QVector<QStringList>& output, QString separator, bool file_with_header, bool ignore_when_separator_first, long numberofvaluedlinestoskip){

    QStringList header=read_line(filename,1).split(separator);if (ignore_when_separator_first && header[0]=="")header.removeFirst();
    //
        QFile file_in(filename);
        file_in.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&file_in);
    //


    //define permutation
        long numberofcategories;
        QVector<long> permutation;

        if (file_with_header){

            numberofcategories=categories.size();
            permutation=QVector<long> (numberofcategories);

            for(long i=0;i<numberofcategories;i++){ permutation[i] =header.indexOf(categories[i]); }
            stream.readLine();
        }else{
            numberofcategories=header.size();
            permutation=QVector<long> (numberofcategories);
            for(long i=0;i<numberofcategories;i++){ permutation[i] =i;}
        }
    //

    //read in attributes
        QVector<QStringList> list(numberofcategories);
        long j=0;

		while (!stream.atEnd() && j<numberofvaluedlinestoskip){stream.readLine();j++;} //skip valued lines
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList columns =line.split(separator);
			//if (ignore_when_separator_first && columns[0]=="")columns.removeFirst();
			columns.removeAll(QString(""));

            for (long i=0;i<qMin(numberofcategories,long(columns.size()));i++){
                if (permutation[i]!=-1){list[i]<<columns[permutation[i]];}
            }
            j++;
        }
    //

    file_in.close();


    output=list;
    return true;

}


bool inpout::get_vector(QString filename, lvector& f,QString separator, long columnnumber, long numberofvaluedlinestoskip){
    QVector<lvector> temp;
    get_vector_list(QStringList(), filename,temp,separator, false,true, numberofvaluedlinestoskip);
    f=temp[columnnumber];

    return true;
}

bool inpout::get_function(QString filename, lvectorfunction& f,QString separator){
    QVector<lvector> temp;
    get_vector_list(QStringList(), filename,temp,separator, false,true);
    f=lvectorfunction(temp[0],temp[1]);

    return true;
}







  bool inpout::plot_value(double x,QString filename){

        QFile file_out(filename);
        file_out.remove();
        file_out.open(QIODevice::WriteOnly);



        file_out.write((n_to_qstring(x)+"\n").toLocal8Bit().constData());


        file_out.close();

        std::cout<<"saved: "<<filename.toStdString()<<std::endl;
        return true;
  }
  bool inpout::plot_string(QString S,QString filename){

        QFile file_out(filename);
        file_out.remove();
        file_out.open(QIODevice::WriteOnly);



        file_out.write((S+"\n").toLocal8Bit().constData());


        file_out.close();
        std::cout<<"saved: "<<filename.toStdString()<<std::endl;
        return true;
  }

bool inpout::plot_vector(lvector x,QString filename){

    QFile file_out(filename);
    file_out.remove();
    file_out.open(QIODevice::WriteOnly);

    for (long i=0;i<x.size();i++){

        file_out.write((n_to_qstring(x[i])+"\n").toLocal8Bit().constData());

    }
    file_out.close();
    std::cout<<"saved: "<<filename.toStdString()<<std::endl;
    return true;
}
bool inpout::append_value(double x,QString filename){

    QFile file_out(filename);

    file_out.open(QIODevice::Append);



    file_out.write((n_to_qstring(x)+"\n").toLocal8Bit().constData());

    file_out.close();
    return true;
}
bool inpout::append_string(QString S,QString filename){

    QFile file_out(filename);

    file_out.open(QIODevice::Append);



    file_out.write((S).toLocal8Bit().constData());

    file_out.close();
    return true;
}
bool inpout::append_vector(lvector x,QString filename){

    QFile file_out(filename);
    //file_out.remove();
    file_out.open(QIODevice::Append);

    for (long i=0;i<x.size();i++){

        file_out.write((n_to_qstring(x[i])+"\n").toLocal8Bit().constData());

    }
    file_out.close();
    return true;
}
bool inpout::append_vector_list(QList<lvector> x,QString filename){


    //file_out.remove();

    for(long row=0; row<x[0].size();row++){
        lvector tmp;
        for (long col=0;col<x.size();col++){
            tmp.append(x[col][row]);
        }
        append_vector_h(tmp, filename);

    }

    return true;
}

bool inpout::append_vector_h(lvector x,QString filename, QString separator){

    QFile file_out(filename);
    //file_out.remove();
    file_out.open(QIODevice::Append);
    QString S;
    for (long i=0;i<x.size()-1;i++){
        S+=n_to_qstring(x[i])+separator;

    }
    S+=n_to_qstring(x[x.size()-1]);
    file_out.write((S+"\n").toLocal8Bit().constData());
    file_out.close();
    return true;
}

bool inpout::plot_vector_h(lvector x,QString filename, QString separator){

    QFile file_out(filename);
    //file_out.remove();
    file_out.open(QIODevice::WriteOnly);
    QString S;
    for (long i=0;i<x.size()-1;i++){
        S+=n_to_qstring(x[i])+separator;

    }
    S+=n_to_qstring(x[x.size()-1]);
    file_out.write((S).toLocal8Bit().constData());
    file_out.close();
    std::cout<<"saved: "<<filename.toStdString()<<std::endl;
    return true;
}

bool inpout::plot_function(lvectorfunction x,QString filename, QString separator){

    QFile file_out(filename);
    file_out.remove();
    file_out.open(QIODevice::WriteOnly);

    for (long i=0;i<x.domain.size();i++){
        file_out.write((n_to_qstring(x.domain[i])+separator+n_to_qstring(x.function[i])+"\n").toLocal8Bit().constData());

    }
    file_out.close();
    std::cout<<"saved: "<<filename.toStdString()<<std::endl;
    return true;
}

bool inpout::save_image(QPixmap map, QString filename, QString format, int quality){

    std::cout<<"saved: "<<filename.toStdString()<<std::endl;

    QFile testfile(filename);
    testfile.open(QIODevice::WriteOnly);
    map.save(&testfile, format.toStdString().c_str(), quality);


    return true;

}
bool inpout::save_image(QImage map, QString filename, QString format, int quality){

    QFile testfile(filename);
    testfile.open(QIODevice::WriteOnly);
    map.save(&testfile, format.toStdString().c_str(), quality);


    std::cout<<"saved: "<<filename.toStdString()<<std::endl;




    return true;

}
