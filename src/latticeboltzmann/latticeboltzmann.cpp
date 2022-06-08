#include "./latticeboltzmann/latticeboltzmann.h"


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

latticeboltzmann::latticeboltzmann(QStringList arguments, inpout *traffic)
{
    traffic2=new xmlio(traffic);

    if (QString(arguments[1])=="calculate_next_2D_lattice_boltzmann_timestep"){
        calculate_next_2D_lattice_boltzmann_timestep(arguments);
    } else if (QString(arguments[1])=="create_image"){
        create_image(arguments, traffic);
    } else if (QString(arguments[1])=="create_input_fields"){
        create_input_fields(arguments, traffic);
    } else if (QString(arguments[1])=="set_pointer"){
        set_pointer(arguments);
    } else if (QString(arguments[1])=="calculate_grid_vicinity"){
        calculate_grid_vicinity(arguments, traffic);
    } else if (QString(arguments[1])=="create_2d_vectorfield_image"){
        create_2d_vectorfield_image(arguments, traffic);
    } else if (QString(arguments[1])=="save_xml_schema"){
        save_xml_schema(arguments);
    } else if (QString(arguments[1])=="check_xml_with_schema"){
        check_xml_with_schema(arguments);
    } else if (QString(arguments[1])=="xquerytest"){
        xquerytest(arguments, traffic);
    } else if (QString(arguments[1])=="readwritetest"){
        readwritetest();
    }

}




// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////main part///////////////////////////



QString separator=";";
////////////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------
    //defining basic vectors
        lvector e0=lvector(QList<double>()<<0<<0);lvector e1=lvector(QList<double>()<<1<<0);lvector e2=lvector(QList<double>()<<0<<1);
        lvector e3=lvector(QList<double>()<<-1<<0);lvector e4=lvector(QList<double>()<<0<<-1);lvector e5=lvector(QList<double>()<<1<<1);
        lvector e6=lvector(QList<double>()<<-1<<1);lvector e7=lvector(QList<double>()<<-1<<-1);lvector e8=lvector(QList<double>()<<1<<-1);

        QList<lvector> e_index=(QList<lvector>() <<e0<<e1<<e2<<e3<<e4<<e5<<e6<<e7<<e8);
        //QList<lvector> e_index_inverse=(QList<lvector>() <<e0<<e3<<e4<<e1<<e2<<e7<<e8<<e5<<e6);

        lvector inverse_directions(QList<double>()<<0<<3<<4<<1<<2<<7<<8<<5<<6);
    //
    //define weights
        QList<double> w=(QList<double>()<<4.0/9.0<<1.0/9.0<<1.0/9.0<<1.0/9.0<<1.0/9.0<<1.0/36.0<<1.0/36.0<<1.0/36.0<<1.0/36.0);


    //

//--------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////

struct lattice{ //a,b in
    static int in_or_out_or_surface(lvector& vicinity_vector){//0=out; 1=in; 2=surface
        int v0=vicinity_vector[0];
        int v1=vicinity_vector[1];
        int v2=vicinity_vector[2];
        int v3=vicinity_vector[3];
        int v4=vicinity_vector[4];
        int v5=vicinity_vector[5];
        int v6=vicinity_vector[6];
        int v7=vicinity_vector[7];
        int v8=vicinity_vector[8];


        if(v0==1 &&(v5+v6+v7+v8==3)&& v1+v2+v3+v4==4) return 3;                                       //surface at inner edge
        if(v0==1 && v1+v2+v3+v4==3) return 2;   //surface
        if(v0==1 && v1+v2+v3+v4==4) return 1;   //in

        return 0;                               //out
    }
    static int surface_vector(lvector& vicinity_vector){//returns the index corresponding to e[index]
        int v0=vicinity_vector[0];
        int v1=vicinity_vector[1];
        int v2=vicinity_vector[2];
        int v3=vicinity_vector[3];
        int v4=vicinity_vector[4];
        int v5=vicinity_vector[5];
        int v6=vicinity_vector[6];
        int v7=vicinity_vector[7];
        int v8=vicinity_vector[8];

        if(v0==1 &&(v5+v6+v7+v8==3 && v5==0)&& v1+v2+v3+v4==4) return 5;
        if(v0==1 &&(v5+v6+v7+v8==3 && v6==0)&& v1+v2+v3+v4==4) return 6;
        if(v0==1 &&(v5+v6+v7+v8==3 && v7==0)&& v1+v2+v3+v4==4) return 7;
        if(v0==1 &&(v5+v6+v7+v8==3 && v8==0)&& v1+v2+v3+v4==4) return 8;


        if(v0==1 && v1+v2+v3+v4==3 && v1==0) return 1;
        if(v0==1 && v1+v2+v3+v4==3 && v2==0) return 2;
        if(v0==1 && v1+v2+v3+v4==3 && v3==0) return 3;
        if(v0==1 && v1+v2+v3+v4==3 && v4==0) return 4;
        return 0;
    }
    static lvector get_vector_9(long a, long b, QVector<lvector> & M_in){
      return M_in[b].sub(a*9,a*9+8);
    }
    static lvector get_vector_2(long a, long b, QVector<lvector> & M_in){
      return M_in[b].sub(a*2,a*2+1);
    }

    static QList <QList <lvector> > structure_vector_field_M_N_9(long M, long N){
        QList <QList <lvector> > structure;
        for(long b=0;b<N;b++){
            QList<lvector> temp;
            for(long a=0;a<M;a++){
                temp<<lvector(QList<double>()<<0<<0<<0<<0<<0<<0<<0<<0<<0);
            }
            structure<<temp;
        }
        return structure;
    }

    static QList <QList <lvector> > structure_vector_field_M_N_2(long M, long N){
        QList <QList <lvector> > structure;
        for(long b=0;b<N;b++){
            QList<lvector> temp;
            for(long a=0;a<M;a++){
                temp<<lvector(QList<double>()<<0<<0);
            }
            structure<<temp;
        }
        return structure;
    }

    static QList <lvector >structure_vector_field_M_N(long M, long N){
        QList <lvector > structure;
        for(long b=0;b<N;b++){
            structure<<lvector(M,0);
        }
        return structure;
    }

    static QList <QList <lvector> > transform_vector_field_9M_N_to_M_N_9(QVector<lvector> & M_in, long M, long N){

      QList <QList <lvector> > M_out;
      for(long b=0;b<N;b++){
          QList<lvector> temp;
          for(long a=0;a<M;a++){
              temp<<get_vector_9(a,b,M_in);
          }
          M_out<<temp;
      }
      return M_out;
    }

    static QList <QList <lvector> > transform_vector_field_2M_N_to_M_N_2(QVector<lvector> & M_in, long M, long N){

      QList <QList <lvector> > M_out;
      for(long b=0;b<N;b++){
          QList<lvector> temp;
          for(long a=0;a<M;a++){
              temp<<get_vector_2(a,b,M_in);
          }
          M_out<<temp;
      }
      return M_out;
    }

    static QList<lvector> transform_vector_field_M_N_9_to_9M_N(QList <QList <lvector> > & M_in, long M, long N){

      QList <lvector> templist;
      for(long b=0;b<N;b++){
                templist<<lvector(9*M,0);
                for(long a=0;a<M;a++){
                    for(long k=0;k<9;k++) templist.last()[a*9+k]=M_in[b][a][k];
                }
      }
      return templist;
    }

    static QList<lvector> transform_vector_field_M_N_2_to_2M_N(QList <QList <lvector> > & M_in, long M, long N){

      QList <lvector> templist;
      for(long b=0;b<N;b++){
                templist<<lvector(2*M,0);
                for(long a=0;a<M;a++){
                    for(long k=0;k<2;k++) templist.last()[a*2+k]=M_in[b][a][k];
                }
        }
        return templist;
    }

    static long PB_one_dim(long x, long L){
        if (x>=0) return x % L; else return L- ((-x) % L);
    }

    static lvector PB(lvector x, long L1, long L2){
        lvector x2=lvector(2,0);
        x2[0]=PB_one_dim(x[0],L1);
        x2[1]=PB_one_dim(x[1],L2);
        return x2;

    }
};

// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::calculate_next_2D_lattice_boltzmann_timestep(QStringList arguments){


//


    //Lattice Boltzmann Method for Fluid Simulations;  Yuanxun Bill Bao & Justin Meskas;   April 14, 2011

    double speed(QString(arguments[2]).toDouble()); //denoted as c in the article
    double tau(QString(arguments[3]).toDouble()); //relaxation time
    double delta_t(QString(arguments[4]).toDouble()); //time step delta
    QString BC_cond(arguments[5]);     //type of boundary conditions
    QString filename_xml(arguments[6]); //xml file for reading/writing
    long step(QString(arguments[7]).toLong());



//--------------------------------------------------LOAD M_f_old, M_rho_old, M_u_old, M_f_eq_old


        //read lbconfig-node
            QDomDocument xmlBOM("LBF");
            QDomElement root = xmlBOM.createElement("latticeboltzmann");
            xmlBOM.appendChild(root);

            QDomElement lbconfig = xmlBOM.createElement("lbconfig");
            root.appendChild(lbconfig);

            traffic2->read_lbconfig_pointer(filename_xml,step,xmlBOM,lbconfig);  //very fast but requires include_lbconfig_pointer( ... ) in "save files" below
            //traffic2->read_lbconfig_via_XMLquery(filename_xml,step,xmlBOM,lbconfig);//quite fast
            //traffic2->read_lbconfig_via_XMLDom(filename_xml,step,xmlBOM,lbconfig);//slow

        //



        //read out values from the specific node of step=step

            QStringList dimension=lbconfig.attribute("dimension").split("x");
            long M=dimension[0].toLong();
            long N=dimension[1].toLong();

            lvector dimensions=lvector(2);dimensions[0]=M;dimensions[1]=N;

            double time;

            QVector <lvector> grid_io;
            QVector <lvector> M_f_io;
            QVector <lvector> M_rho_io;
            QVector <lvector> M_u_io;
            QVector <lvector> M_f_eq_io;

            traffic2->node2values(dimensions,step,  time, lbconfig,grid_io,M_f_io,M_rho_io,M_u_io,M_f_eq_io);
        //


        //indices stuff

            //M_f_old - 9MxN fields
            //M_f_old[0]=[[0...8], [9...17],... [8M, 9M-1]]
            //...
            //
            //M_f_old[N-1]=[[0...8], [9...17],... [8M, 9M-1]]

            //m=0,...,9M-1
            //n=0,...,N-1

            // a in [0,...,M-1], b in [0,...,N-1];    j=n
        //
        if(!(grid_io.size()==N && (grid_io)[0].size()==9*M)){std::cout<<"dimension mismatch"<<std::endl;return false;}
        QList <QList <lvector> > grid_old=lattice::transform_vector_field_9M_N_to_M_N_9(grid_io, M, N);
        QList <QList <lvector> > M_f_old=lattice::transform_vector_field_9M_N_to_M_N_9(M_f_io, M, N);
        //QList <lvector> M_rho_old=M_rho_io.toList();
        //QList <QList <lvector> > M_u_old=lattice::transform_vector_field_2M_N_to_M_N_2(M_u_io, M, N);
        //QList <QList <lvector> > M_f_eq_old=lattice::transform_vector_field_9M_N_to_M_N_9(M_f_eq_io, M, N);



//--------------------------------------------------2. STREAMING

    QList <QList <lvector> > M_f_stream;
    compute_M_f_stream(M, N, M_f_stream, M_f_old, speed, delta_t,BC_cond, grid_old);
   //     QList <QList <lvector> > M_f_stream=M_f_old;

//--------------------------------------------------3. COMPUTE new rho AND u

    //build rho and get rho - system averaged
        QList <lvector> M_rho_new;
        compute_M_rho_new(M, N, M_rho_new, M_f_stream);
    //

    //build u
        QList <QList <lvector> > M_u_new;
        compute_M_u_new(M, N, M_u_new, M_f_stream, M_rho_new, speed);
    //

//--------------------------------------------------4. COMPUTE M_f_eq

    QList <QList <lvector> > M_S;
    compute_M_S(M, N, M_S, M_u_new, speed);

    QList <QList <lvector> > M_f_eq_new;
    compute_M_f_eq_new(M, N, M_f_eq_new, M_S, M_rho_new);

//--------------------------------------------------5. COLLISION STEP

    QList <QList <lvector> > M_f_new;//=M_f_stream;
    compute_M_f_new(M, N, M_f_new, M_f_stream, M_f_eq_new, tau);
    //QList <QList <lvector> > M_f_new=M_f_stream;

//--------------------------------------------------SAVE FILES

    grid_io=lattice::transform_vector_field_M_N_9_to_9M_N(grid_old, M, N).toVector();
    M_f_io=lattice::transform_vector_field_M_N_9_to_9M_N(M_f_new, M, N).toVector();
    M_rho_io=M_rho_new.toVector();
    M_u_io=lattice::transform_vector_field_M_N_2_to_2M_N(M_u_new, M, N).toVector();
    M_f_eq_io=lattice::transform_vector_field_M_N_9_to_9M_N(M_f_eq_new, M, N).toVector();


    step++;   //increase step
    time+=delta_t;

    xmlBOM=QDomDocument("LBF");
    root = xmlBOM.createElement("latticeboltzmann");
    xmlBOM.appendChild(root);

    lbconfig = xmlBOM.createElement("lbconfig");
    root.appendChild(lbconfig);
    traffic2->values2node(xmlBOM, lvector(QList<double>()<<M<<N),step,  time, lbconfig,grid_io,M_f_io,M_rho_io,M_u_io,M_f_eq_io);


    traffic2->include_lbconfig_pointer(filename_xml, lbconfig);
    //traffic2->include_lbconfig_via_XMLDom(filename_xml, lbconfig);


    ////////////////////////


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::calculate_grid_vicinity(QStringList arguments, inpout *traffic){
    /*

    */
    QString filename_grid(arguments[2]);     //MxN
    QString filename_vicinity_grid(arguments[3]);     //9*MxN


    //determine dimensions of the grid;
        long M2,N2;
        {
            QImage grid_image; grid_image.load(filename_grid);
            N2=grid_image.width();
            M2=grid_image.height();
        }

        long M=M2;
        long N=N2;
        std::cout<<"dimension: "<<M<<" x "<<N<<std::endl;

    //--------------------------------------------------LOAD M_grid

        QImage grid_image; grid_image.load(filename_grid);
        QList<lvector> M_grid;
        for(long b=0;b<N;b++){
            static lvector temp(M,0);
            for(long a=0;a<M;a++)temp[a]=(qGray(grid_image.pixel(b,a))==255) ? 0: 1;//0=out=standard; 1=point of an obstacle
            M_grid<<temp;
        }

    //determine vicinity and save in vicinity grid
        QList< QList <lvector> > M_vicinity_grid=lattice::structure_vector_field_M_N_9(M,N);
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){
                static lvector vicinity_pattern(9,0);
                for(int i=0;i<9;i++){
                    lvector move_vector=(e_index[i]);
                    lvector x_new=lattice::PB(lvector(QList<double>()<<b<<a)+move_vector,N,M);
                    vicinity_pattern[i]=M_grid[x_new[0]][x_new[1]];
                }
                //use vicinity_pattern vector for the surface vector determination and in-out detection
                    M_vicinity_grid[b][a]=vicinity_pattern;
                //
            }
        }
    //

    //save surface vector grid
        QList <lvector> outputlist;
        outputlist=lattice::transform_vector_field_M_N_9_to_9M_N(M_vicinity_grid, M, N);
        traffic->plot_vector_list(outputlist,QStringList(),filename_vicinity_grid, separator);
    //


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::create_input_fields(QStringList arguments, inpout *traffic){


    QString filename_input_vicinity_grid(arguments[2]);
    double speed(QString(arguments[3]).toDouble());
    QString filename_output_xml(arguments[4]);  //xml file for writing
    long step(QString(arguments[5]).toLong());
    double time(QString(arguments[6]).toDouble());

    //initialize dimensions
        long M,N;
        {QVector <lvector> M_vicinity_grid_raw; traffic->get_vector_list(QStringList(), filename_input_vicinity_grid,M_vicinity_grid_raw, separator, false,false, 0);
        N=M_vicinity_grid_raw.size();
        M=M_vicinity_grid_raw[0].size()/9;}
    //

    //M_f
        QList <QList <lvector> > M_f=lattice::structure_vector_field_M_N_9(M,N);
    //

    //M_rho
        QList <lvector> M_rho=lattice::structure_vector_field_M_N(M,N);
    //

    //M_u
        QList <QList <lvector> > M_u=lattice::structure_vector_field_M_N_2(M,N);
    //

    //M_f_eq
        QList <QList <lvector> > M_f_eq=lattice::structure_vector_field_M_N_9(M,N);
    //

    //--------------------------------------------------LOAD M_vicinity_grid

        //get M_vicinity_grid from picture
            QVector <lvector> M_vicinity_grid_raw; traffic->get_vector_list(QStringList(), filename_input_vicinity_grid,M_vicinity_grid_raw, separator, false,false, 0);
            QList <QList <lvector> > M_vicinity_grid=lattice::transform_vector_field_9M_N_to_M_N_9(M_vicinity_grid_raw, M, N);
        //
        //initialize M_rho and M_u depending on whether it is a grid, surface point or within the solid material
            for(long b=0;b<N;b++){
                for(long a=0;a<M;a++){

                    int inoutsurf=lattice::in_or_out_or_surface(M_vicinity_grid[b][a]);
                    if(inoutsurf==0 || inoutsurf==2 || inoutsurf==3){//only if the point is not lying on a material grid point defined by the grid
                    //if(inoutsurf==0){//only if the point is free
                        if(pow(a-0.5*M,2.0)+pow(b-0.5*N,2.0)<pow(Minimum_own(QList<double>()<<double(M)<<double(N))*0.6,2.0)/4.0){  //within a circle

                            M_rho[b][a]=0.95*rnd(1)+0.05;
                            //M_rho[b][a]=0.5;

                            M_u[b][a]=randvector_2d(0.000);

                            /*for(long i=0;i<9;i++){
                                M_f[b][a][i]=rnd(1);
                                M_f_eq[b][a][i]=rnd(1);
                            }*/

                        }else{
                            M_rho[b][a]=0.7;

                            M_u[b][a]=randvector_2d(0.000);

                            /*for(long i=0;i<9;i++){
                                M_f[b][a][i]=rnd(0);
                                M_f_eq[b][a][i]=rnd(0);
                            }*/
                        }
                    }
                }
            }
        //
        //calc M_f_eq and set for M_f=M_f_eq
            QList <QList <lvector> > M_S;
            compute_M_S(M, N, M_S, M_u, speed);
            compute_M_f_eq_new(M, N, M_f_eq, M_S, M_rho);
            M_f=M_f_eq;
        //


    //--------------------------------------------------SAVE FILES


        QVector <lvector> grid_io=lattice::transform_vector_field_M_N_9_to_9M_N(M_vicinity_grid, M, N).toVector();
        QVector <lvector> M_f_io=lattice::transform_vector_field_M_N_9_to_9M_N(M_f, M, N).toVector();
        QVector <lvector> M_rho_io=M_rho.toVector();
        QVector <lvector> M_u_io=lattice::transform_vector_field_M_N_2_to_2M_N(M_u, M, N).toVector();
        QVector <lvector> M_f_eq_io=lattice::transform_vector_field_M_N_9_to_9M_N(M_f, M, N).toVector();

        QDomDocument xmlBOM("LBF");
        QDomElement root = xmlBOM.createElement("latticeboltzmann");
        xmlBOM.appendChild(root);

        QDomElement lbconfig = xmlBOM.createElement("lbconfig");
        root.appendChild(lbconfig);

        traffic2->values2node(xmlBOM, lvector(QList<double>()<<M<<N),step,  time, lbconfig,grid_io,M_f_io,M_rho_io,M_u_io,M_f_eq_io);
        traffic2->create_initial_lbconfig_and_save_to_XML(filename_output_xml, xmlBOM, lbconfig);



    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::save_xml_schema(QStringList arguments){

    QString filename_xml_schema(arguments[2]);

    //read in dimensions
        QString filename_xml(arguments[3]);
        long  step0((arguments[4]).toLong());


        //read lbconfig-node
            QDomDocument xmlBOM;
            QDomElement lbconfig;
            traffic2->read_lbconfig_via_XMLquery(filename_xml,step0,xmlBOM,lbconfig);
        //

        //read out values from the specific node of step=step
            QStringList dimension=lbconfig.attribute("dimension").split("x"); lvector dimensions=qstringlist_to_lvector(dimension);
            long numberlatticevectors=lbconfig.attribute("numberlatticevectors").toLong();
        //
    //
    if (dimension.size()==2 && numberlatticevectors==9){
        traffic2->create_XML_schema(filename_xml_schema);
    }else{
        std::cout<<"No scheme available";
    }


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::check_xml_with_schema(QStringList arguments){
    QString filename_xml_file(arguments[2]);
    QString filename_xml_schema_file(arguments[3]);
    return traffic2->xml_validation(filename_xml_file,filename_xml_schema_file);
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::set_pointer(QStringList arguments){

    //arguments
        QString filename_xml(arguments[2]);
        long step(QString(arguments[3]).toLong());
        QString reset_pointer(arguments[4]);
    //


    //read lbconfig-node
        QDomDocument xmlBOM("LBF");
        QDomElement root = xmlBOM.createElement("latticeboltzmann");
        xmlBOM.appendChild(root);

        QDomElement lbconfig = xmlBOM.createElement("lbconfig");
        root.appendChild(lbconfig);
        if(reset_pointer=="reset"){
            traffic2->read_lbconfig_via_XMLquery(filename_xml,step,xmlBOM,lbconfig);
            traffic2->include_lbconfig_pointer(filename_xml,lbconfig,false,true);
        }else if(reset_pointer=="set"){
            //traffic2->read_lbconfig_via_XMLquery(filename_xml,step,xmlBOM,lbconfig);//hier neues read_lbconfig_pointer machen das die naechste konfiguration einliest
            traffic2->read_lbconfig_pointer(filename_xml,step,xmlBOM,lbconfig,true);
            traffic2->include_lbconfig_pointer(filename_xml,lbconfig,false,false);
        }else {std::cout<<"bug in set_pointer"<<std::endl;}


    //


    return true;

}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////









// ////////////////////////////////////////////////////////////////////////////////////////////////
bool latticeboltzmann::compute_M_f_stream(long& M, long& N, QList <QList <lvector> > & M_f_stream, QList <QList <lvector> > & M_f_old, double& speed, double& delta_t, QString BC_cond, QList <QList <lvector> > M_vicinity_grid){

    M_f_stream=M_f_old;

//--------------------------------------------------boundary conditions
    //--------------------------------------------------periodic
    if(BC_cond=="periodic"){
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){
                lvector x=lvector(QList<double>()<<b<<a);
                for(int i=0;i<9;i++){
                    lvector move_vector=((speed*delta_t)*e_index[i]);lvector x_new=lattice::PB(x+move_vector,N,M);
                    M_f_stream[x_new[0]][x_new[1]][i]= M_f_old[b][a][i];
                }

            }
        }
    }
    //--------------------------------------------------on grid bounce back
    else if (BC_cond=="on_grid_bounce_back"){
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){
                lvector x=lvector(QList<double>()<<b<<a);
                int inoutsurf=lattice::in_or_out_or_surface(M_vicinity_grid[b][a]);
                if(inoutsurf==0){//point is free
                    for(int i=0;i<9;i++){
                        lvector move_vector=((speed*delta_t)*e_index[i]);lvector x_new=lattice::PB(x+move_vector,N,M);
                        M_f_stream[x_new[0]][x_new[1]][i]= M_f_old[b][a][i];
                    }
                }else if(inoutsurf==1){//point is in material
                    M_f_stream[b][a]= 0;//M_f_old[b][a];
                }else if(inoutsurf==2){//point is on surface?
                    int surface=lattice::surface_vector(M_vicinity_grid[b][a]);//determine surface vector
                    for(int i=0;i<9;i++){
                        lvector x_new;
                        if(e_index[surface]/e_index[i]<0){
                            M_f_stream[b][a][inverse_directions[i]]= M_f_old[b][a][i];
                        }else{
                            lvector move_vector=((speed*delta_t)*e_index[i]); x_new=lattice::PB(x+move_vector,N,M);
                            M_f_stream[x_new[0]][x_new[1]][i]= M_f_old[b][a][i];
                        }
                    }
                }else if(inoutsurf==3){//point is on inner edge surface?
                    int surface=lattice::surface_vector(M_vicinity_grid[b][a]);//determine surface vector
                    for(int i=0;i<9;i++){
                        lvector x_new;
                        if(e_index[surface]/e_index[i]<0){
                            M_f_stream[b][a][inverse_directions[i]]= M_f_old[b][a][i];
                        }else if(e_index[surface]/e_index[i]>0){
                            lvector move_vector=((speed*delta_t)*e_index[i]); x_new=lattice::PB(x+move_vector,N,M);
                            M_f_stream[x_new[0]][x_new[1]][i]= M_f_old[b][a][i];
                        }else{//=0
                            M_f_stream[b][a][i]= 0;
                        }
                    }
                }
            }
        }
    }

    //


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::compute_M_rho_new(long& M, long& N, QList <lvector>& M_rho_new, QList <QList <lvector> >& M_f_stream){


    //build rho //and get rho - system averaged
        M_rho_new.clear();
        //double rho=0;

        for(long b=0;b<N;b++){
            QList <double> temp;
            for(long a=0;a<M;a++){
                temp<<M_f_stream[b][a].vector_sum();
            }
            lvector temp2;temp2.put_in_qlist_double(temp);
            M_rho_new<<temp2;

            //lvector test; test.put_in_qvector_double(temp.toVector());
            //rho=rho+test.vector_sum();
        }
        //rho=rho/(M*N);
    //


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::compute_M_u_new(long& M, long& N, QList<QList <lvector> >& M_u_new, QList <QList <lvector> >& M_f_stream, QList <lvector>& M_rho_new, double& speed){

    M_u_new.clear();
    for(long b=0;b<N;b++){
        QList <lvector> temp0;
        for(long a=0;a<M;a++){
            lvector  temp(2,0);
            if(M_rho_new[b][a]>0) {for(int i=0;i<9;i++) temp=temp+((speed/M_rho_new[b][a]*M_f_stream[b][a][i])*e_index[i]);}
            temp0<<temp;
        }
        M_u_new<<temp0;
    }
    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::compute_M_S(long& M, long& N, QList <QList <lvector> >& M_S, QList <QList <lvector> >&  M_u_new, double& speed){
    //S
        M_S.clear();
        for(long b=0;b<N;b++){
            QList <lvector> temp0;
            for(long a=0;a<M;a++){
                static lvector temp(9,0);
                for(int i=0;i<9;i++)            temp[i]=w[i]*(3.0*(e_index[i]/M_u_new[b][a])/speed+4.5*pow(e_index[i]/M_u_new[b][a],2.0)/speed/speed-1.5*(M_u_new[b][a]/M_u_new[b][a])/speed/speed);
                temp0<<temp;
            }
            M_S<<temp0;
        }
    //

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::compute_M_f_eq_new(long& M, long& N, QList <QList <lvector> >& M_f_eq_new, QList <QList <lvector> >& M_S, QList <lvector> & M_rho_new){

    M_f_eq_new.clear();

            for(long b=0;b<N;b++){
                QList <lvector> temp0;
                for(long a=0;a<M;a++){
                    static lvector temp(9,0);
                    for(int i=0;i<9;i++)            temp[i]=w[i]*M_rho_new[b][a]+M_rho_new[b][a]*M_S[b][a][i];
                    temp0<<temp;
                }
                M_f_eq_new<<temp0;
            }


    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::compute_M_f_new(long& M, long& N, QList<QList <lvector> >& M_f_new, QList <QList <lvector> >& M_f_stream, QList <QList <lvector> >& M_f_eq_new, double & tau){


    M_f_new.clear();
    for(long b=0;b<N;b++){
        QList <lvector> temp0;
        for(long a=0;a<M;a++){
            static lvector temp(9,0);
            for(int i=0;i<9;i++)          temp[i]=M_f_stream[b][a][i]-1.0/tau*(M_f_stream[b][a][i]-M_f_eq_new[b][a][i]);
            temp0<<temp;
        }
        M_f_new<<temp0;
    }

    return true;

}






// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////GRAPHICS/////////////////////////////////

//struct helping in color calculations
    struct color_calculator{
        static double adjusted_value(double value, double cutoff_min, double cutoff_max){
            if (value>cutoff_max)value=cutoff_max;
            if (value<cutoff_min)value=cutoff_min;

            //return (value-min)/(max-min)*255;
            return (value-cutoff_min)/(cutoff_max-cutoff_min);
        }

    };


// ////////////////////////////////////////////////////////////////////////////////////////////////



bool latticeboltzmann::create_image(QStringList arguments, inpout *traffic){

    //arguments
        QString filename_xml(arguments[2]);
        QString filename_picture(arguments[3]);
        QString picture_file_format(arguments[4]);
        double lower_value(QString(arguments[5]).toDouble());
        double upper_value(QString(arguments[6]).toDouble());
        int quality(QString(arguments[7]).toInt());
        QString colorscheme(arguments[8]);
        long step(QString(arguments[9]).toLong());

    //


    //read lbconfig-node
        QDomDocument xmlBOM("LBF");
        QDomElement root = xmlBOM.createElement("latticeboltzmann");
        xmlBOM.appendChild(root);

        QDomElement lbconfig = xmlBOM.createElement("lbconfig");
        root.appendChild(lbconfig);
        traffic2->read_lbconfig_pointer(filename_xml,step,xmlBOM,lbconfig);
    //



    //read out values from the specific node of step=step

        QStringList dimension=lbconfig.attribute("dimension").split("x");
        long M=dimension[0].toLong();
        long N=dimension[1].toLong();
        std::cout<<M<<"   "<<N<<std::endl;
        lvector dimensions=lvector(2);dimensions[0]=M;dimensions[1]=N;

        double time;

        QVector <lvector> grid=QVector <lvector> (N,lvector(9*M,0));
        QVector <lvector> M_f=QVector <lvector> (N,lvector(9*M,0));
        QVector <lvector> M_rho=QVector <lvector> (N,lvector(M,0));
        QVector <lvector> M_u=QVector <lvector> (N,lvector(2*M,0));
        QVector <lvector> M_f_eq=QVector <lvector> (N,lvector(9*M,0));

        long step_node;
        traffic2->node2values(dimensions,step_node,  time, lbconfig,grid,M_f,M_rho,M_u,M_f_eq);
    //

    //create image
        QList <lvector> list=M_rho.toList();
        QImage map;
        create_image(list,lower_value, upper_value, map, colorscheme);
        traffic->save_image(map, filename_picture, picture_file_format, quality);
    //


    return true;

}


// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::create_image(QList<lvector> list,double lower_value, double upper_value, QImage &map, QString colorscheme){

    long N=list.size();
    long M=list[0].size();

    //rescale values
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){
                //std::cout<<list[b][a]<<"  "<<white_value<<"   "<<black_value<<"fin  ";
                //if(a==b && a==0)std::cout<<list[b][a]<<"   ";
                list[b][a]=color_calculator::adjusted_value( list[b][a], lower_value, upper_value);
                //if(a==b && a==0)std::cout<<list[b][a]<<"   "<<std::endl;
                //std::cout<<list[b][a]<<"  ";
            }
        }
    //

    //write values into QImage map
        map=QImage(N,M, QImage::Format_RGB32);
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){

                //map.setPixel(b,a,qRgb(255*list[b][a],255*list[b][a],255*list[b][a]));
               //map.setPixel(b,a,colorpalette1(list[b][a]));
                if(colorscheme=="color_scheme_1"){
                    map.setPixel(b,a,color_scheme_1(list[b][a]));
                }else if(colorscheme=="color_scheme_2"){
                    map.setPixel(b,a,color_scheme_2(list[b][a]));
                }
            }
        }
    //


    //

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::create_2d_vectorfield_image(QStringList arguments, inpout *traffic){

    QString filename_xml(arguments[2]);

    QString filename_picture(arguments[3]);
    QString picture_file_format(arguments[4]);
    double phi_lower_value(QString(arguments[5]).toDouble());
    double phi_upper_value(QString(arguments[6]).toDouble());
    double r_lower_value(QString(arguments[7]).toDouble());
    double r_upper_value(QString(arguments[8]).toDouble());
    double rho_lower_value(QString(arguments[9]).toDouble());
    double rho_upper_value(QString(arguments[10]).toDouble());
    int quality(QString(arguments[11]).toInt());
    QString colorscheme(arguments[12]);
    long step(QString(arguments[13]).toLong());


    //read lbconfig-node
        QDomDocument xmlBOM("LBF");
        QDomElement root = xmlBOM.createElement("latticeboltzmann");
        xmlBOM.appendChild(root);

        QDomElement lbconfig = xmlBOM.createElement("lbconfig");
        root.appendChild(lbconfig);
        traffic2->read_lbconfig_pointer(filename_xml,step,xmlBOM,lbconfig);
    //


    //read out values from the specific node of step=step

        QStringList dimension=lbconfig.attribute("dimension").split("x");
        long M=dimension[0].toLong();
        long N=dimension[1].toLong();
        std::cout<<M<<"   "<<N<<std::endl;
        lvector dimensions=lvector(2);dimensions[0]=M;dimensions[1]=N;

        double time;

        QVector <lvector> grid=QVector <lvector> (N,lvector(9*M,0));
        QVector <lvector> M_f=QVector <lvector> (N,lvector(9*M,0));
        QVector <lvector> M_rho=QVector <lvector> (N,lvector(M,0));
        QVector <lvector> M_u=QVector <lvector> (N,lvector(2*M,0));
        QVector <lvector> M_f_eq=QVector <lvector> (N,lvector(9*M,0));

        traffic2->node2values(dimensions,step,  time, lbconfig,grid,M_f,M_rho,M_u,M_f_eq);
    //
        //create image
            QList <lvector> list_dens=M_rho.toList();
            //QList <lvector> list_velo=M_u.toList();
            QList< QList<lvector> > list_velo=lattice::transform_vector_field_2M_N_to_M_N_2(M_u,M_u[0].size()/2,M_u.size());

            QImage map;
            create_2d_vectorfield_image(list_velo, list_dens,phi_lower_value, phi_upper_value,r_lower_value, r_upper_value, rho_lower_value, rho_upper_value, map, colorscheme);
            traffic->save_image(map, filename_picture, picture_file_format, quality);


        //

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

bool latticeboltzmann::create_2d_vectorfield_image(QList<QList<lvector> >& list_velo_in,QList<lvector>& list_dens_in, double phi_lower_value, double phi_upper_value, double r_lower_value, double r_upper_value, double rho_lower_value, double rho_upper_value, QImage &map, QString colorscheme){

    QList<QList<lvector> > list_velo=list_velo_in;
    QList<lvector> list_dens=list_dens_in;
    long N=list_dens.size();
    long M=list_dens[0].size();


    //rescale values
    double phi,r;
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){
                polarcoordinates(list_velo[b][a],phi,r);
                list_velo[b][a][0]=color_calculator::adjusted_value(phi, phi_lower_value, phi_upper_value);
                list_velo[b][a][1]=color_calculator::adjusted_value(r, r_lower_value, r_upper_value);
                list_dens[b][a]=color_calculator::adjusted_value(list_dens[b][a], rho_lower_value, rho_upper_value);

            }
        }
    //

    //write values into QImage map
        map=QImage(N,M, QImage::Format_RGB32);
        for(long b=0;b<N;b++){
            for(long a=0;a<M;a++){

                if(colorscheme=="color_scheme_polarcoordinates_1"){
                    map.setPixel(b,a,color_scheme_polarcoordinates_1(list_velo[b][a]));
                }else if(colorscheme=="color_scheme_polarcoordinates_plus_1_parameter_1"){
                    map.setPixel(b,a,color_scheme_polarcoordinates_plus_1_parameter_1(list_velo[b][a],list_dens[b][a]));
                }





                //std::cout<<"pixel value: "<<list[b][a]<<"    "<<map.pixel(b,a)<<std::endl;
            }
        }
    //


    //

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

QRgb latticeboltzmann::color_scheme_1(double x){//x in [0,1]

   double blue_raw=x-0.0/3.0; int blue=  255*3.0* ((blue_raw<0)?0:((blue_raw>1.0/3.0)? 1.0/3.0 : blue_raw));
   double green_raw=x-1.0/3.0; int green=  255*3.0* ((green_raw<0)?0:((green_raw>1.0/3.0)? 1.0/3.0 : green_raw));
   double red_raw=x-2.0/3.0; int red=  255*3.0* ((red_raw<0)?0:((red_raw>1.0/3.0)? 1.0/3.0 : red_raw));
    //std::cout<<x<<"  "<<red<<"   "<<green<<"   "<<blue<<std::endl;
 return qRgb(red,green,blue);

}

// ////////////////////////////////////////////////////////////////////////////////////////////////

QRgb latticeboltzmann::color_scheme_2(double x){//x in [0,1]


    //The value of s, l, and a must all be in the range 0-255; the value of h must be in the range 0-359.
    QColor newcolor=QColor::fromHsl(x*359, 255, 150);


    int red,green,blue;
    newcolor.getRgb(&red,&green,&blue);


    return qRgb(red,green,blue);

}

// ////////////////////////////////////////////////////////////////////////////////////////////////

QRgb latticeboltzmann::color_scheme_polarcoordinates_1(lvector polar_coord){//x in [0,1]


    //The value of s, l, and a must all be in the range 0-255; the value of h must be in the range 0-359.
    QColor newcolor=QColor::fromHsl(polar_coord[0]*359, 255, polar_coord[1]*255);


    int red,green,blue;
    newcolor.getRgb(&red,&green,&blue);


    return qRgb(red,green,blue);

}

// ////////////////////////////////////////////////////////////////////////////////////////////////

QRgb latticeboltzmann::color_scheme_polarcoordinates_plus_1_parameter_1(lvector polar_coord, double density){//

    //The value of s, l, and a must all be in the range 0-255; the value of h must be in the range 0-359.
    QColor newcolor=QColor::fromHsl(polar_coord[0]*359, density*255, polar_coord[1]*255);


    int red,green,blue;
    newcolor.getRgb(&red,&green,&blue);


    return qRgb(red,green,blue);

}





////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////tests/////////////////////////////////////////////////////
bool latticeboltzmann::readwritetest(){

    traffic2->readwritetest();
    return true;


}
bool latticeboltzmann::xquerytest(QStringList arguments, inpout *traffic){

    //arguments
        QString filename(arguments[2]);
        QString filename_picture(arguments[3]);
        QString picture_file_format(arguments[4]);
        double lower_value(QString(arguments[5]).toDouble());
        double upper_value(QString(arguments[6]).toDouble());
        int quality(QString(arguments[7]).toInt());
        QString colorscheme(arguments[8]);
        long step(QString(arguments[9]).toLong());
    //


    //read lbconfig-node
        QDomDocument xmlBOM;
        QDomElement lbconfig;
        traffic2->read_lbconfig_via_XMLquery(filename,step,xmlBOM,lbconfig);
    //



    //read out values from the specific node of step=step
        QStringList dimension=lbconfig.attribute("dimension").split("x");
        long M=dimension[0].toLong();
        long N=dimension[1].toLong();
        std::cout<<"dimension: "<<M<<"   "<<N<<std::endl;
        lvector dimensions=lvector(2);dimensions[0]=M;dimensions[1]=N;

        double time;

        QVector <lvector> grid=QVector <lvector> (N,lvector(9*M,0));
        QVector <lvector> M_f=QVector <lvector> (N,lvector(9*M,0));
        QVector <lvector> M_rho=QVector <lvector> (N,lvector(M,0));
        QVector <lvector> M_u=QVector <lvector> (N,lvector(2*M,0));
        QVector <lvector> M_f_eq=QVector <lvector> (N,lvector(9*M,0));

        traffic2->node2values(dimensions,step,  time, lbconfig,grid,M_f,M_rho,M_u,M_f_eq);
    //

    //create image
        QList <lvector> list=M_rho.toList();
        QImage map;
        create_image(list,lower_value, upper_value, map, colorscheme);
        traffic->save_image(map, filename_picture, picture_file_format, quality);
    //


    return true;
}



////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
















