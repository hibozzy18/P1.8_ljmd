#include <compute_force.h>


// =======================================================================


/* static void ekin(mdsys_t *sys) */
void ekin(mdsys_t *sys)
{   
  int i;

  sys->ekin=0.0;
  for (i=0; i<sys->natoms; ++i) {
    sys->ekin += 0.5*mvsq2e*sys->mass*(sys->vx[i]*sys->vx[i] + sys->vy[i]*sys->vy[i] + sys->vz[i]*sys->vz[i]);
  }
  sys->temp = 2.0*sys->ekin/(3.0*sys->natoms-3.0)/kboltz;
  
}


// =======================================================================


/* static void force(mdsys_t *sys)  */
void force(mdsys_t *sys) 
{
  double r,ffac;
  double rx,ry,rz;
  int i, j;

  /* zero energy and forces */
#ifdef USE_MPI
  double epot = 0.0;
  
  azzero( sys->cx, sys->natoms );
  azzero( sys->cy, sys->natoms );
  azzero( sys->cz, sys->natoms );
  
  /* communicate to all the processes previous step update of positions */
  MPI_Bcast( sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->comm );
  MPI_Bcast( sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->comm );
  MPI_Bcast( sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->comm );
#else
  sys->epot=0.0;
  azzero(sys->fx,sys->natoms);
  azzero(sys->fy,sys->natoms);
  azzero(sys->fz,sys->natoms);
#endif //USE_MPI

  /* loop to compute forces */
  for( i = sys->rank; i < (sys->natoms); i += sys->npes ) {
    for(j=0; j < (sys->natoms); ++j) {

      /* particles have no interactions with themselves */
      if (i==j) continue;
            
      /* get distance between particle i and j */
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      r = sqrt(rx*rx + ry*ry + rz*rz);
      
      /* compute force and energy if within cutoff */
      if (r < sys->rcut) {
	ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
				  +6*pow(sys->sigma/r,6.0)/r);

#ifdef USE_MPI
	epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
				      -pow(sys->sigma/r,6.0));
#else
	sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
					   -pow(sys->sigma/r,6.0));
#endif //USE_MPI

#ifdef USE_MPI
        sys->cx[i] += rx/r*ffac;
        sys->cy[i] += ry/r*ffac;
        sys->cz[i] += rz/r*ffac;
#else
	sys->fx[i] += rx/r*ffac;
	sys->fy[i] += ry/r*ffac;
	sys->fz[i] += rz/r*ffac;
#endif //USE_MPI
      }
    }
#ifdef USE_MPI
    MPI_Reduce( sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->comm );
    MPI_Reduce( sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->comm );
    MPI_Reduce( sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->comm );
    MPI_Reduce( &epot, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->comm );
#endif //USE_MPI
  }

  return;
}


// =======================================================================
