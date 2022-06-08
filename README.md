# 2D_Lattice_Boltzmann_Simulator

It is a simple D2Q9 Lattice Boltzmann simulator.
The program's main functionality is to calculate the next time step when given a system snapshot via an XML-file.
In the same file the new system snapshot is appended, i.e. it is an input and output file.
The algorithm is described in [Yuanxun Bill Bao and Justin Meskas. Lattice boltzmann method for 
uid simulations. Department of Mathematics, Courant Institute of Mathematical Sciences, New York University, 44, 2011.]
It involves on grid and periodic boundary conditions. More are to come ...
The grid is provided via a black and white png image file.


A possible compile script is in; src/compile


\< program file\> <option\>

W.r.t. <option\> the following is supported:


    "calculate_next_2D_lattice_boltzmann_timestep"
        function:
            - reads snapshot number <step> from the <filename_xml>-file and appends a snapshot
              with  <step>+1 in the <filename_xml>-file.
            - simulation details: <speed>, <tau>, <delta_t>, <BC_cond>
        requires: 
            <speed (double)> denoted as c in the article
            <tau (double)> relaxation time
            <delta_t (double)> time step delta
            <BC_cond (QString)> type of boundary conditions: "periodic" or "on_grid_bounce_back"
            <filename_xml (QString)> xml file for reading/writing
            <step (long)>
        
    "calculate_grid_vicinity"
        function:
            - from a ablack and white image file it creates a grid file wheras each point is acosiated with 
              a 9 point vector
        requires: 
            <filename_grid (QString)> 
            <filename_vicinity_grid (QString)>
    
    "create_input_fields"
        function:
            - creates an xml-snapshot element and saves it in <filename_output_xml> from a vicinity grid given 
              by <filename input vicinity grid>
        requires: 
            <filename_input_vicinity_grid (QString)> more than a simple grid since it explains in which direction
            a surface is oriented
            <speed (double)> lattice speed
            <filename_output_xml (QString)> 
            <step (long)> step number of this so created single snapshot xml file
            <time (double)> time stamp of this so created single snapshot xml file
        
    "save_xml_schema"
        function:
            - can export the xml schema used to read and write system snapshots
            - it is stored in the program
        requires: 
            <filename_xml_schema (QString)>
            <filename_xml (QString)>
            <step0 (long)>

    "check_xml_with_schema"
	function:
            - can check the xml-file against the schema file
        requires: 
            <filename_xml_file (QString)>
            <filename_xml_schema_file (QString)>

    "set_pointer"
        function:
            - in the beginning of the xml file is a number "lastpos=..." that says from which numer
              you should start searching fo a snapshot number
            - the pointer can be set to a certain integer value referring to a snapshot or reset to one
        requires: 
            <filename_xml (QString)>
            <step (long)>
            <reset_pointer (QString)> takes the values "reset" or "set"


GRAPHICS:

        
    "create_image"
        function:
            - creates an image file <filename picture> from an xml file <filename_xml> containing system snapshots
        requires: 
            <filename_xml (QString)> xml file for reading
            <filename_picture (QString)> 
            <picture_file_format (QString)> e.g., "PNG"
            <lower_value (double)> 
            <upper_value (double)> 
            <quality (int)> e.g., 100
            <colorscheme (QString)> e.g., color_scheme_2 
            <step (long)>
        
    "create_2d_vectorfield_image"
        function:
            - creates a 2d vectorfield image as used for velocity fields
            - direction and strength of the vectorfield have an influence on the color depending on the provided color scheme
        requires: 
            <filename_xml (QString)>
            <filename_picture (QString)>
            <picture_file_format (QString)>
            <phi_lower_value (double)>
            <phi_upper_value (double)>
            <r_lower_value (double)>
            <r_upper_value (double)>
            <rho_lower_value (double)>
            <rho_upper_value (double)>
            <quality (int)> e.g., 100
            <colorscheme (QString)> e.g., color_scheme_2
            <step (long)>

TESTS:

    "readwritetest"
        function: one can play around with xml file parsing ...
            - 
        requires: 	
            none
        
    "xquerytest"
        function:
            - can visualize a snapshot at step <step>; it contains many picture parameters
        requires: 
            <filename (QString)>
            <filename_picture (QString)>
            <picture_file_format (QString)>
            <lower_value (double)>
            <upper_value (double)>
            <quality (int)>
            <colorscheme (String)>
            <step (long)>
        

