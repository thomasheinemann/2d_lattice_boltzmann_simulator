#ifndef XMLIO_H
#define XMLIO_H


#include "./io/inpout.h"
#include "./stringstuff/stringstuffbasic.h"


#include <string>
#include <iostream>
//#include <boost/foreach.hpp>

#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>

#include <QXmlQuery>
#include <QXmlSerializer>
#include <QBuffer>
#include <QSaveFile>
#include <QXmlStreamReader>

#include <QTextStream>
#include <QUrl>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <QtGlobal>
#include <QString>



class xmlio
{
    inpout *traffic;
public:
    xmlio(inpout * set_traffic);

    //schema and first lbconfig
        bool create_XML_schema(QString filename_output_xml_schema);
        bool create_initial_lbconfig_and_save_to_XML(QString filename_output_xml, QDomDocument & xmlBOM, QDomElement & lbconfig);
    //

    //read lbconfig step
        bool read_lbconfig_via_XMLDom(QString filename, long step, QDomDocument & xmlBOM, QDomElement & lbconfig);
        bool read_lbconfig_via_XMLquery(QString filename, long step, QDomDocument & xmlBOM, QDomElement & lbconfig);
        bool read_lbconfig_pointer(QString filename, long step, QDomDocument & xmlBOM, QDomElement & lbconfig, bool new_config=false);
    //

    //include lbconfig
        bool include_lbconfig_via_XMLDom(QString filename, QDomElement & lbconfig);
        bool include_lbconfig_pointer(QString filename, QDomElement & lbconfig, bool finalize=true, bool reset_pointer=false);
    //

    //convert values from or to QDomElement
        bool values2node(QDomDocument& doc, lvector dimensions,long step, double time, QDomElement & lbconfig,QVector <lvector> &grid, QVector <lvector> &M_f,QVector <lvector> &M_rho,QVector <lvector> &M_u,QVector <lvector> &M_f_eq);
        bool node2values(lvector& dimensions,long &step, double &time, QDomElement & lbconfig,QVector <lvector> &grid, QVector <lvector> &M_f,QVector <lvector> &M_rho,QVector <lvector> &M_u,QVector <lvector> &M_f_eq);
    //



//test
    bool readwritetest();

    bool xml_validation(QString filename_xml_file, QString filename_xml_schema_file);

};

#endif // XMLIO_H
