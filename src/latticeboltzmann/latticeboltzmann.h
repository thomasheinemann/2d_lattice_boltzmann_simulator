#ifndef LATTICEBOLTZMANN_H
#define LATTICEBOLTZMANN_H

#include <QStringList>

#include <QTimer>
#include "./io/inpout.h"
#include "./latticeboltzmann/xmlio.h"

#include <QImage>
#include <QRgb>
//#include <QByteArray>

class latticeboltzmann
{
public:
    latticeboltzmann(QStringList arguments, inpout * traffic);
    xmlio* traffic2;

//lb algorithm stuff
        bool calculate_next_2D_lattice_boltzmann_timestep(QStringList arguments);
        bool calculate_grid_vicinity(QStringList arguments, inpout *traffic);
        bool create_input_fields(QStringList arguments, inpout *traffic);
        bool save_xml_schema(QStringList arguments);
        bool check_xml_with_schema(QStringList arguments);
        bool set_pointer(QStringList arguments);
private:
        bool compute_M_f_stream(long& M, long& N, QList <QList <lvector> > & M_f_stream, QList <QList <lvector> > & M_f_old, double& speed, double& delta_t, QString BC_cond="periodic", QList <QList <lvector> > M_vicinity_grid=QList<QList<lvector> > ());
        bool compute_M_rho_new(long& M, long& N, QList <lvector>& M_rho_new, QList <QList <lvector> >& M_f_stream);
        bool compute_M_u_new(long& M, long& N, QList<QList <lvector> >& M_u_new, QList <QList <lvector> >& M_f_stream, QList <lvector>& M_rho_new, double& speed);
        bool compute_M_S(long& M, long& N, QList <QList <lvector> >& M_S, QList <QList <lvector> >&  M_u_new, double& speed);
        bool compute_M_f_eq_new(long& M, long& N, QList <QList <lvector> >& M_f_eq_new, QList <QList <lvector> >& M_S, QList <lvector> & M_rho_new);
        bool compute_M_f_new(long& M, long& N, QList<QList <lvector> >& M_f_new, QList <QList <lvector> >& M_f_stream, QList <QList <lvector> >& M_f_eq_new, double & tau);


//color stuff
public:

        bool create_image(QStringList arguments, inpout *traffic);
        bool create_image(QList<lvector> list, double white_value, double black_value, QImage &map, QString colorscheme);
        bool create_2d_vectorfield_image(QStringList arguments, inpout *traffic);
        bool create_2d_vectorfield_image(QList<QList<lvector> >& list_velo_in,QList<lvector>& list_dens_in, double phi_lower_value, double phi_upper_value, double r_lower_value, double r_upper_value, double rho_lower_value, double rho_upper_value, QImage &map, QString colorscheme);
private:
        QRgb color_scheme_1(double x);
        QRgb color_scheme_2(double x);
        QRgb color_scheme_polarcoordinates_1(lvector polar_coord);
        QRgb color_scheme_polarcoordinates_plus_1_parameter_1(lvector polar_coord, double density);


//test stuff
public:
        bool readwritetest();
        bool xquerytest(QStringList arguments, inpout *traffic);


};

#endif // LATTICEBOLTZMANN_H
