
#include <QCoreApplication>

//test
#include <QTextStream>
#include <iostream>
#include <QtWidgets/QWidget>
#include <QDebug>
//


#include "./math/mathstuffbasic.h"

#include <QTimer>

#include "./io/inpout.h"

#include "./latticeboltzmann/latticeboltzmann.h"


//for sleep
#include <QThread>
class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};
//




int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);
    QString appname=QFileInfo( app.applicationFilePath() ).fileName();


    QStringList arguments;
    for(long i=0;i<argc;i++) {arguments<<QString(argv[i]);}
    if (arguments.size()<=2) {
        std::cout<<"No parameter provided"<<std::endl;
        return 0;
    }


    inpout* traffic=new inpout;
    latticeboltzmann cmds_lb(arguments,traffic);

    delete traffic;

    return 0;
    // return app.exec();
}
