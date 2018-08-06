#!/bin/bash

#Job queues
#BSUB -J dr0.001va0.5
#BSUB -q normal
#BSUB -m "spms-limeb-c02"

#Specify walltime in HH:MM
#BSUB -W 120:00

#Requesting for 8 cores
#BSUB -n 4

#BSUB -o %J.out
#BSUB -e %J.err

#EMAIL 
#BSUB -u pnie001@e.ntu.edu.sg

# load module enviroment 

cd /home/pnie001/4.np_fix_active/
cd dr0.001/va0.5/phi0.58/mu0.0/
mpirun -np 4 ../../lmp_mpi -in in.active

