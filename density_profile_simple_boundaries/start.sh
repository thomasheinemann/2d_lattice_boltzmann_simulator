#!/bin/bash


#######################################
#######################################
######### SETTINGS ####################
#######################################
#######################################

# Determine operation system

case "$OSTYPE" in
  solaris*) systemtype="SOLARIS" ;;
  darwin*)  systemtype="OSX" ;; 
  linux*)   systemtype="LINUX" ;;
  bsd*)     systemtype="BSD" ;;
  msys*)    systemtype="WINDOWS" ;;
  cygwin*)  systemtype="ALSO WINDOWS" ;;
  *)        systemtype="unknown: $OSTYPE" ;;
esac

echo "Operation system "$systemtype

# Prepare folder settings

if [ "$systemtype" == "WINDOWS" ] || [ "$systemtype" == "ALSO WINDOWS" ]; then
	scripts=2d_lattice_boltzmann_sim.exe
elif  [ "$systemtype" == "LINUX" ]; then
	scripts="2d_lattice_boltzmann_sim"
else
    echo "Operation system not supported"
    exit 1
fi

rm -f $scripts; cp ../bin/$scripts ./

#folder
foldername="calculatedfiles"
folderschema="schemata"
gridfolder="grids"
imagefolder="images"

#files
gridimagefile="pipe3.png"
gridfile="grid.tho"
prefix="X"

# Simulation settings

#input image indices from start_index to max_index
start_index=0;max_index=50

#image indices for video creation
start_index_movie=0;max_index_movie=50

#time interpretation
starttime=0;timestep=0.1

#######################################
#######################################
######### SETTINGS ####################
#######################################
#######################################

#############create grid file from image

./$scripts calculate_grid_vicinity $gridfolder/$gridimagefile $gridfolder/$gridfile

#############create input fields

./$scripts create_input_fields $gridfolder/$gridfile 1 $foldername/$prefix"_xml".tho $start_index $starttime

#############create schema file using the attributes from step 0

./$scripts save_xml_schema $folderschema/schema.xsd $foldername/$prefix"_xml".tho 0

#######################################
#######################################
############## MAIN LOOP ##############
#######################################
#######################################

for((iterator=$start_index;iterator<=$max_index;iterator++)); do

    #new LB step No
    iterator2=$(($iterator +1))

    echo -e "LB-step: "$iterator "to" $iterator2

    #pure LB step
    ./$scripts \
    \
    calculate_next_2D_lattice_boltzmann_timestep \
    1 2 0.1 \
    on_grid_bounce_back \
    $foldername/$prefix"_xml".tho \
    $iterator 
    echo -e ""
    #on_grid_bounce_back \
done

#######################################
#######################################
#### Validate XML filewith schema #####
#######################################
#######################################

echo  "compare with schema:"

# xmllint --noout --schema $folderschema/schema.xsd $foldername/$prefix"_xml".tho
./$scripts \
\
check_xml_with_schema \
$foldername/$prefix"_xml".tho \
$folderschema/schema.xsd 

#######################################
#######################################
# Creating pictures from XML file entries ##
#######################################
#######################################


setorreset="reset"

for((iterator=$start_index_movie;iterator<=$max_index_movie;iterator++)); do

    echo $setorreset

    ./$scripts \
    \
    set_pointer \
    $foldername/$prefix"_xml".tho \
    $iterator \
    $setorreset

    setorreset="set"

    
    #create;PNG image of rho
    
    ./$scripts \
    \
    create_image \
    $foldername/$prefix"_xml".tho \
    $imagefolder/$prefix"_rho_"$iterator.png \
    PNG \
    0 2 \
    100 \
    color_scheme_2 \
    $iterator &

    
    #create;PNG image of u
    
    ./$scripts \
    \
    create_2d_vectorfield_image \
    $foldername/$prefix"_xml".tho \
    $imagefolder/$prefix"_u_"$iterator.png \
    PNG \
    -3.14159 3.14159 \
    -5 1 \
    0 0.6 \
    100 \
    color_scheme_polarcoordinates_1 \
    $iterator &
    

    #create;PNG image of u and rho
    
    ./$scripts \
    \
    create_2d_vectorfield_image \
    $foldername/$prefix"_xml".tho \
    $imagefolder/$prefix"_u_rho_"$iterator.png \
    PNG \
    -3.14159 3.14159 \
    -5 1 \
    0 0.6 \
    100 \
    color_scheme_polarcoordinates_plus_1_parameter_1 \
    $iterator &


    if (( $iterator % 10 == 0 )); then wait; fi 
    wait

done

#######################################
#######################################
###### Create animated GIFs ###########
#######################################
#######################################


if  [ "$systemtype" == "LINUX" ]; then

    #create GIF density

    convert $(for ((a=$start_index_movie; a<=$max_index_movie; a++)); do printf -- "-delay 1 $imagefolder/$prefix"_rho_"%s.png " $a; done;) video_rho.gif

    #create GIF u

    convert $(for ((a=$start_index_movie; a<=$max_index_movie; a++)); do printf -- "-delay 1 $imagefolder/$prefix"_u_"%s.png " $a; done;) video_u.gif

    #create GIF u and rho

    convert $(for ((a=$start_index_movie; a<=$max_index_movie; a++)); do printf -- "-delay 1 $imagefolder/$prefix"_u_rho_"%s.png " $a; done;) video_u_rho.gif

    # remove all image files
    rm images/*.png
fi





exit 0
