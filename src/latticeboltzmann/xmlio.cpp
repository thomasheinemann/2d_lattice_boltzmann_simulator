#include "./latticeboltzmann/xmlio.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
namespace Qt
{
    static auto endl = ::endl;
    //static auto SkipEmptyParts = QString::SkipEmptyParts;
}
#endif

xmlio::xmlio(inpout *set_traffic)
{
    traffic=set_traffic;
}


bool xmlio::create_XML_schema(QString filename_output_xml_schema){

    traffic->remove(filename_output_xml_schema);
    //long dim_prod=dimension[0]; for(long i=1;i<dimension.size();i++)dim_prod*=dimension[i];

    QStringList S=QStringList()
    <<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
              //           <<"<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http://www.softed.de/training/kontakte\" targetNamespace=\"http://www.softed.de/training/kontakte\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\">"
    <<"<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\">"
    <<"    <xs:element name=\"latticeboltzmann\">"
    <<"        <xs:complexType>"
    <<"             <xs:sequence>"
    <<"                <xs:element name=\"lbconfig\" maxOccurs=\"unbounded\" minOccurs=\"0\">"
    <<"					<xs:complexType>"
    <<"						<xs:sequence>"
    <<"							   <xs:element name=\"grid\" maxOccurs=\"1\" minOccurs=\"1\"> <xs:simpleType> <xs:restriction base=\"xs:string\"/> </xs:simpleType> </xs:element>"
    <<"							   <xs:element name=\"f\" maxOccurs=\"1\" minOccurs=\"1\"> <xs:simpleType> <xs:restriction base=\"xs:string\"/> </xs:simpleType> </xs:element>"
    <<"                            <xs:element name=\"rho\" maxOccurs=\"1\" minOccurs=\"1\"> <xs:simpleType> <xs:restriction base=\"xs:string\"/> </xs:simpleType> </xs:element>"
    <<"                            <xs:element name=\"u\"  maxOccurs=\"1\" minOccurs=\"1\"> <xs:simpleType>  <xs:restriction base=\"xs:string\"/> </xs:simpleType> </xs:element>"
    <<"                            <xs:element name=\"f_eq\"  maxOccurs=\"1\" minOccurs=\"1\"> <xs:simpleType>  <xs:restriction base=\"xs:string\"/> </xs:simpleType> </xs:element>"
    <<"						</xs:sequence>"
    <<"						<xs:attribute name=\"dimension\" type=\"Typ_dimension\"/>"
    <<"						<xs:attribute name=\"step\" type=\"Typ_step\"/>"
    <<"						<xs:attribute name=\"time\" type=\"Typ_time\"/>"
    <<"						<xs:attribute name=\"numberlatticevectors\" type=\"xs:integer\"/>"
    <<"					</xs:complexType>"
    <<"				</xs:element>"
    <<"            </xs:sequence>"
    <<"						<xs:attribute name=\"lastpos\" type=\"xs:string\"/>"
    <<"        </xs:complexType>"
    <<"    </xs:element>"
    <<"    <xs:simpleType name=\"valuelist\">"
    <<"        <xs:list itemType=\"posdec\"/>"
    <<"    </xs:simpleType>"
    <<"    <xs:simpleType name=\"posdec\">"
    <<"        <xs:restriction base=\"xs:double\">"
    <<"            <xs:minInclusive value=\"-99999990\" />"
    <<"            <xs:maxInclusive value=\"99999999\" />"
    <<"        </xs:restriction>"
    <<"    </xs:simpleType>"
    <<"    <xs:simpleType name=\"Typ_dimension\">"
    <<"        <xs:list itemType=\"xs:string\"/>"
    <<"    </xs:simpleType>"
    <<"    <xs:simpleType name=\"Typ_step\">"
    <<"        <xs:restriction base=\"xs:integer\">"
    <<"        </xs:restriction>"
    <<"    </xs:simpleType>"
    <<"    <xs:simpleType name=\"Typ_time\">"
    <<"        <xs:restriction base=\"xs:double\">"
    <<"        </xs:restriction>"
    <<"    </xs:simpleType>"
    <<"</xs:schema>"
    <<""
      ;


    //QStringList S=QStringList()<<S_start;

    for(int i=0;i<S.length();i++){

        traffic->append_string(S[i], filename_output_xml_schema);
        if(i<S.length()-1)traffic->append_string("\n", filename_output_xml_schema);

    }

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////


bool xmlio::create_initial_lbconfig_and_save_to_XML(QString filename_output_xml, QDomDocument & xmlBOM, QDomElement & lbconfig){

    //build tree with one configuration in xmlBOM
    long x=1;    QString  Sstart= QString("%1").arg(x, 20, 10, QChar('0'));
    long x2=1;    QString  Send= QString("%1").arg(x2, 20, 10, QChar('0'));

    xmlBOM.firstChildElement().setAttribute("lastpos", Sstart+Send);

    //save entire qdomdocument xmlBOM to file
        QFile data(filename_output_xml);
        if (data.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream out(&data);
            xmlBOM.save(out,0);
        }

    //

    //overwrite first step to have correct byte positions in the attribute lastpos
        include_lbconfig_pointer(filename_output_xml, lbconfig,true,true);
    //


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////

bool xmlio::read_lbconfig_via_XMLDom(QString filename, long step, QDomDocument & xmlBOM, QDomElement & lbconfig){

    QFile f(filename);
    if (!f.open(QIODevice::ReadWrite )){ std::cerr << "Error while loading file" << std::endl; return 1;}
    //QDomDocument xmlBOM;
    xmlBOM.setContent(&f);




    lbconfig = xmlBOM.firstChildElement().firstChildElement();


    for(;lbconfig.attribute("step").toLong()<step-1 && !lbconfig.nextSibling().isNull(); lbconfig=lbconfig.nextSibling().toElement()){std::cout<<lbconfig.attribute("step").toStdString()<<"   "<<step<<std::endl;}


    f.close();
    return true;


}

////////////////////////////////////////////////////////////////////////////////

bool xmlio::read_lbconfig_via_XMLquery(QString filename, long step, QDomDocument & xmlBOM, QDomElement & lbconfig){

    QXmlQuery query;

    QString S="doc('" + filename + "')/latticeboltzmann/lbconfig[@step=" + n_to_qstring(step) + "]";


    query.setQuery(S);

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);
    QXmlSerializer serializer(query, &buffer);

    query.evaluateTo(&serializer);


    /*
    // Save the data to a file.
    QSaveFile testus("erty.tho");
    testus.open(QIODevice::WriteOnly);
    testus.write(output);
    testus.commit();
    //testus.close();
    */


    xmlBOM=QDomDocument();
    xmlBOM.setContent(output);




    lbconfig = xmlBOM.firstChild().toElement();




    return true;
}

////////////////////////////////////////////////////////////////////////////////



bool xmlio::read_lbconfig_pointer(QString filename, long step, QDomDocument & xmlBOM, QDomElement & lbconfig, bool new_config){

/*
    if (step==0){
        read_lbconfig_via_XMLquery(filename, step, xmlBOM, lbconfig);
        return true;
    }
*/
    QFile file(filename);

    //qint64 importantlinepos=1;
    qint64 startlastconf=1;
    qint64 endlastconf=1;

    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        //initialize string and position
            QTextStream out(&file);
            out.seek(1);
        //

        //variable controlling execution of scope XXXXX
            bool singleshot=true;
        //

        while(!out.atEnd()){

            //read line
                qint64 posi=out.pos();  //save position
                QString line;
                out.readLineInto(&line);
            //
            //XXXXX read first tag of <latticeboltzmann ...   then jump to the position that is standing there at attribute "lastpos"
                if(singleshot && line.contains(QString("lastpos"))){
                    //save position such that this line can be manuipulated later
                        long index=line.indexOf("lastpos");
                        //importantlinepos=posi+index+9;
                    //
                    //jump to position defined in the first half after lastpos=
                        posi=line.midRef(index+9,20).toLong();
                        qint64 posi2=line.midRef(index+29,20).toLong();
                        if(new_config==false){
                            startlastconf=posi;
                            endlastconf=posi2;
                            out.seek(startlastconf);
                        }else{
                            startlastconf=posi2;
                            //endlastconf=2*posi2-posi;
                            out.seek(startlastconf);
                            step++;
                        }

                    //
                    //execute this scope only once
                        singleshot=false;
                    //
                }
            //

            //is the line that one with the searched step?
                if (line.contains(QString("step=\""+n_to_qstring(step)+"\"")) || line.startsWith(QString("</latticeboltzmann>"))){
                if(new_config)endlastconf=posi;
                out.seek(startlastconf);
                static QString S;
                S=QString("<!DOCTYPE LBF>")
                +QString("\n")
                +QString("<latticeboltzmann>")
                +QString("\n")
                +out.read(endlastconf-startlastconf-1)
                +QString("\n")
                +QString("</latticeboltzmann>");
                //traffic->plot_string(S,"test.tho");

                xmlBOM=QDomDocument();
                xmlBOM.setContent(S);

                lbconfig=xmlBOM.firstChildElement().firstChildElement();

                file.close();


                return true;
                }
            //
        }

    }
    file.close();


    return true;

}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



bool xmlio::include_lbconfig_via_XMLDom(QString filename, QDomElement & lbconfig){


    long step=lbconfig.attribute("step").toLong();

    QFile f(filename);
    if (!f.open(QIODevice::ReadWrite )){ std::cerr << "Error while loading file" << std::endl; return 1;}


    QDomDocument xmlBOM_new;
    xmlBOM_new.setContent(&f);   //read in file



    //find old element mit "step"=step-1
        QDomElement lbconfigalt = xmlBOM_new.firstChildElement().firstChildElement();for(;lbconfigalt.attribute("step").toLong()<step-1; lbconfigalt=lbconfigalt.nextSibling().toElement());
    //

    //include new element right after old element
        QDomElement lbconfig_new =  xmlBOM_new.createElement("lbconfig");
        lbconfig_new=lbconfig;//shallow copy
        xmlBOM_new.firstChild().insertAfter(lbconfig_new,lbconfigalt);
    //

    //save entire qdomdocument to same file
        QTextStream stream(&f); f.resize(0); xmlBOM_new.save(stream,0);
    //

    f.close(); //close file


    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool xmlio::include_lbconfig_pointer(QString filename, QDomElement & lbconfig, bool finalize, bool reset_pointer){


    long step=lbconfig.attribute("step").toLong();

    QFile file(filename);

    qint64 importantlinepos=1;
    qint64 startnewconf=1;
    qint64 endnewconf=1;

    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        //initialize string and position
            QTextStream out(&file);
            out.seek(1);
        //

        //variable controlling execution of scope XXXXX
            bool singleshot= true;
        //

        while(!out.atEnd()){

            //read line
                qint64 posi=out.pos();  //save position
                QString line;
                out.readLineInto(&line);
            //
            //XXXXX read first tag of <latticeboltzmann ...   then jump to the position that is standing there at attribute "lastpos"
                if(singleshot && line.contains(QString("lastpos"))){
                    //save position such that this line can be manuipulated later
                        long index=line.indexOf("lastpos");
                        importantlinepos=posi+index+9;

                    //
                    //jump to position defined in the latter half after lastpos=
                        posi=line.midRef(index+29,20).toLong();
                        if(reset_pointer)posi=1;
                        out.seek(posi);
                    //
                    //execute this scope only once
                        singleshot=false;
                    //
                }
            //

            //is the line that one with the searched step?
                if (line.contains(QString("step=\""+n_to_qstring(step)+"\"")) || line.startsWith(QString("</latticeboltzmann>"))){
                    startnewconf=posi;
                    //manipulate line that has the tag <latticeboltzmann ... with the position of this node
                        out.seek(importantlinepos);
                        QString  S= QString("%1").arg(startnewconf, 20, 10, QChar('0'));
                        out<<S;
                    //

                    //go back to position of this node
                        out.seek(posi);
                    //

                    //save node
                        lbconfig.save(out,0);
                        endnewconf =out.pos();
                    //
                    //manipulate line that has the tag <latticeboltzmann ... with the position of this node
                        out.seek(importantlinepos+20);
                        QString  S2= QString("%1").arg(endnewconf, 20, 10, QChar('0'));
                        out<<S2;
                    //
                        std::cout<<"position: "<<posi<<std::endl;
                    //include final close tag and truncate file
                        if(finalize){
                            out.seek(endnewconf);
                            out<<"</latticeboltzmann>"<<Qt::endl;//<<temp;
                            qint64 lastpos=out.pos();
                            file.resize(lastpos);
                        }
                    //
                    file.close();


                    return true;

                }
            //
        }

    }
    file.close();


    return true;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


bool xmlio::values2node(QDomDocument& doc,lvector dimensions,long step, double time, QDomElement & lbconfig,QVector <lvector> &grid, QVector <lvector> &M_f,QVector <lvector> &M_rho,QVector <lvector> &M_u,QVector <lvector> &M_f_eq){



    lbconfig.setAttribute("dimension", qlistdouble_to_qstring(dimensions.vector.toList(),"x"));
    lbconfig.setAttribute("step", n_to_qstring(step));
    lbconfig.setAttribute("time", n_to_qstring(time));
    lbconfig.setAttribute("numberlatticevectors", "9");


    //concat all lists to qstrings
    //grid
    {
        //QDomElement tag = root.firstChild().firstChild().toDocument().createElement("grid");
        QDomElement tag = doc.createElement("grid");
        lbconfig.appendChild(tag);QString S_mid;
        for(long b=0;b<grid.size();b++){
            for(long a=0;a<grid[0].size();a++){
                S_mid+=n_to_qstring_2(grid[b][a]);S_mid+=" ";
            }
        }
        S_mid.truncate(S_mid.size()-1);
        QByteArray qbS_mid2=qCompress(S_mid.toStdString().data()).toBase64().data();
        S_mid=QString(qbS_mid2);
        QDomText t = lbconfig.toDocument().createTextNode(S_mid);
        tag.appendChild(t);

    }
    //
    //f
    {
        QDomElement tag = doc.createElement("f");
        lbconfig.appendChild(tag);QString S_mid;
        for(long b=0;b<M_f.size();b++){
            for(long a=0;a<M_f[0].size();a++){
                S_mid+=n_to_qstring_2(M_f[b][a]);S_mid+=" ";
            }
        }
        S_mid.truncate(S_mid.size()-1);
        QByteArray qbS_mid2=qCompress(S_mid.toStdString().data()).toBase64().data();
        S_mid=QString(qbS_mid2);
        QDomText t = lbconfig.toDocument().createTextNode(S_mid);
        tag.appendChild(t);
    }
    //
    //rho
    {
        QDomElement tag = doc.createElement("rho");
        lbconfig.appendChild(tag);QString S_mid;
        for(long b=0;b<M_rho.size();b++){
            for(long a=0;a<M_rho[0].size();a++){
                S_mid+=n_to_qstring_2(M_rho[b][a]);S_mid+=" ";
            }
        }
        S_mid.truncate(S_mid.size()-1);
        QByteArray qbS_mid2=qCompress(S_mid.toStdString().data()).toBase64().data();
        S_mid=QString(qbS_mid2);
        QDomText t = lbconfig.toDocument().createTextNode(S_mid);
        tag.appendChild(t);
    }
    //
    //u
    {
        QDomElement tag = doc.createElement("u");
        lbconfig.appendChild(tag);QString S_mid;
        for(long b=0;b<M_u.size();b++){
            for(long a=0;a<M_u[0].size();a++){
                S_mid+=n_to_qstring_2(M_u[b][a]);S_mid+=" ";
            }
        }
        S_mid.truncate(S_mid.size()-1);
        QByteArray qbS_mid2=qCompress(S_mid.toStdString().data()).toBase64().data();
        S_mid=QString(qbS_mid2);
        QDomText t = lbconfig.toDocument().createTextNode(S_mid);
        tag.appendChild(t);
    }
    //
    //f_eq
    {
        QDomElement tag = doc.createElement("f_eq");
        lbconfig.appendChild(tag);QString S_mid;
        for(long b=0;b<M_f_eq.size();b++){
            for(long a=0;a<M_f_eq[0].size();a++){
                S_mid+=n_to_qstring_2(M_f_eq[b][a]);S_mid+=" ";
            }
        }
        S_mid.truncate(S_mid.size()-1);
        QByteArray qbS_mid2=qCompress(S_mid.toStdString().data()).toBase64().data();
        S_mid=QString(qbS_mid2);
        QDomText t = lbconfig.toDocument().createTextNode(S_mid);
        tag.appendChild(t);
    }
    //


    return true;
}


////////////////////////////////////////////////////////////////////////////////


bool xmlio::node2values(lvector& dimensions,long &step, double &time, QDomElement & lbconfig,QVector <lvector> &grid, QVector <lvector> &M_f,QVector <lvector> &M_rho,QVector <lvector> &M_u,QVector <lvector> &M_f_eq){



    QStringList dimension=lbconfig.attribute("dimension").split("x");

    long M=dimension[0].toLong();
    long N=dimension[1].toLong();
    dimensions=lvector(2,0);
    dimensions[0]=M;dimensions[1]=N;
    time=lbconfig.attribute("time").toDouble();
    step=lbconfig.attribute("step").toLong();

    grid=QVector <lvector> (N,lvector(9*M,0));
    M_f=QVector <lvector> (N,lvector(9*M,0));
    M_rho=QVector <lvector> (N,lvector(M,0));
    M_u=QVector <lvector> (N,lvector(2*M,0));
    M_f_eq=QVector <lvector> (N,lvector(9*M,0));

    QDomElement e = lbconfig.firstChildElement();


    while (!e.isNull()) {
        if (e.isElement()) {

            //std::cout << "Element name: " << e.tagName().toStdString() << std::endl;



            //lvector temp=qstringlist_to_lvector(e.text().split(" "));
            QByteArray qbS_mid(e.text().toStdString().data());
            QString S_mid=QString().fromStdString(qUncompress(QByteArray().fromBase64(e.text().toStdString().data())).toStdString());
            lvector temp=qstringlist_to_lvector(S_mid.split(" "));

            if (e.tagName()=="grid"){


                for(long i=0;i<N;i++){lvector x(9*M,0);for(long j=0;j<9*M;j++)x[j]=temp[9*M*i+j];grid[i]=x;}
            }else if (e.tagName()=="f"){
                for(long i=0;i<N;i++){lvector x(9*M,0);for(long j=0;j<9*M;j++)x[j]=temp[9*M*i+j];M_f[i]=x;}
            }else if (e.tagName()=="rho"){
                for(long i=0;i<N;i++){lvector x(M,0);for(long j=0;j<M;j++)x[j]=temp[M*i+j];M_rho[i]=x;}
            }else if (e.tagName()=="u"){
                for(long i=0;i<N;i++){lvector x(2*M,0);for(long j=0;j<2*M;j++)x[j]=temp[2*M*i+j];M_u[i]=x;}
            }else if (e.tagName()=="f_eq"){
                for(long i=0;i<N;i++){lvector x(9*M,0);for(long j=0;j<9*M;j++)x[j]=temp[9*M*i+j];M_f_eq[i]=x;}
            }
        }
        e=e.nextSibling().toElement();
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool xmlio::xml_validation(QString filename_xml_file, QString filename_xml_schema_file){
    QUrl schemaUrl(filename_xml_schema_file);

    QXmlSchema schema;
    QFile file(filename_xml_schema_file);
    file.open(QIODevice::ReadOnly);
    schema.load(&file, QUrl::fromLocalFile(file.fileName()));
    if (schema.isValid()) {
        QFile file(filename_xml_file);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            std::cout << "instance document is valid" <<std::endl;
        else
            std::cout << "instance document is invalid" <<std::endl;

    }else {
        std::cout<<"schema is invalid"<<std::endl;
    }
    file.close();
    return true;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////













//test

bool xmlio::readwritetest(){
    QString filename="test.dat";
    QString line;
    QFile file(filename);
    //bool testbool=true;
    qint64 lastpos=0;
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream out(&file);
        while(!out.atEnd()){
            qint64 posi=out.pos();
            line = out.readLine();

                if (line.contains(QString("schluss"))){
                    QString temp=out.readAll();
                    out.seek(posi);
                    out<<"XXX"<<Qt::endl<<temp;
                    //testbool=false;
                    lastpos=out.pos();
                    file.resize(lastpos);
                }

        }

    }
    file.close();

return true;
}





