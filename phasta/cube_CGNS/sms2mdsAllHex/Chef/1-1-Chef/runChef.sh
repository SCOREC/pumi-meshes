#!/bin/bash


#mpirun -np $1 /projects/tools/SCOREC-core/build16_Opt/test/chef 2>&1 | tee chef.log
#mpirun -np $1 /users/mabl7046/SCOREC-core/build-16.0-210606dev/test/chef 2>&1 | tee chef.log
mpirun -np $1 /projects/tools/SCOREC-core/buildCGNS_OneBase/test/chef 2>&1 | tee chef.log

