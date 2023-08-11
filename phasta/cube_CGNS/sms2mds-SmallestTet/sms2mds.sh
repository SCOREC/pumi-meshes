#/bin/bash

# your environment has to be setup right for this exec.  In the terminal
# you plan to run this script in copy the following lines to 
# your command line (without the # comment of course

#DON"T USE WRONG VERSION HERE soft add  +simmodsuite-12.0-180517dev
# IF the one you want to use is not in soft don't rely on soft and setup env manually
# soft add  +simmodsuite-18.0-230111
# soft add +gcc-6.3.0
# soft add +openmpi-gnu-1.10.6-gnu49-thread



#mpirun -np 1 /projects/tools/SCOREC-core/buildExtruWithTag/test/convert --model-face-root=4321 --native-model=geom.xmt_txt geom.smd geom.sms mdsMesh/
#mpirun -np 1 /projects/tools/SCOREC-core/buildTestMergetEWT/test/convert --model-face-root=ExruRootID.txt --native-model=geom.xmt_txt geom.smd geom.sms mdsMesh/
mpirun -np 1 /projects/tools/SCOREC-core/buildCGNS_OneBase/test/convert  --no-pyramid-fix  geom.smd geom.sms mdsMesh/
