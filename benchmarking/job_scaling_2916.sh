#!/bin/bash

#PBS -S /bin/bash
#PBS -N scaling_omp
#PBS -l nodes=1:ppn=20
#PBS -l walltime=06:00:00
#PBS -q reserved2
#PBS -o /dev/null
#PBS -e what.e

cd $PBS_O_WORKDIR
cd ../

for nthreads in 1 2 3 4 5 6 7 8 10 12 18 19; do

	for runs in `seq 1 10`; do
	    
		make
		export OMP_NUM_THREADS=${nthreads}
		make check 2>>times_omp_2916.dat
		make clean

	done

done

		