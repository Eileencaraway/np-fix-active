#/bin/bash

#PBS -P 12000235
#PBS -N va0.5phi0.64
#PBS -j oe
#PBS -V
#PBS -m e 
#PBS -l select=1:ncpus=4
#PBS -l walltime=24:00:00 

cd $PBS_O_WORKDIR 
NCPUS=`cat $PBS_NODEFILE | wc -l` 
echo $NCPUS 

module load intelmpi

cd $HOME/scratch/7.np_fix_active/
cd dr0.001/va0.5/phi0.64/mu1.0/
mpirun -np 4 ../../lmp_mpi -in in.active 

exit 
