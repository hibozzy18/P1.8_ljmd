#include<input.h>

int get_a_line(FILE *fp, char * buf)
{
    char tmp[BLEN], *ptr;
    int i = 0;
	
    /* read a line and cut of comments and blanks */
    if (fgets(tmp, BLEN, fp))
    {

        ptr = strchr(tmp, '#');
        if (ptr)
            *ptr = '\0';
        i = strlen(tmp);
        --i;
        while (isspace(tmp[i]))
        {
            tmp[i] = '\0';
            --i;
        }
        ptr = tmp;
        while (isspace(*ptr))
        {
            ++ptr;
        }
        i = strlen(ptr);
        strcpy(buf, tmp);
        return 0;
    }
    else
    {
        perror("problem reading input");
        return -1;
    }
    
}

/* static  */
int populate_data(FILE * fp, char (*line)[BLEN], char (*restfile)[BLEN],
		  char (*trajfile)[BLEN], char (*ergfile)[BLEN], mdsys_t * sys,
		   int * nprint)
{
  
  // reads atom number from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->natoms = atoi(*line);
  
  // reads mass from fp and saves it in data_struct sys  
  if (get_a_line(fp, *line))
    return 1;
  sys->mass = atof(*line);
    
  // reads epsilon from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->epsilon = atof(*line);
    
  // reads sigma from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->sigma = atof(*line);
    
  // reads cut-off radius from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->rcut = atof(*line);
    
  // reads box-size from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->box = atof(*line);
    
  // reads path/to/restart-file from fp and saves it in output array of char 'restfile'
  if (get_a_line(fp, *restfile))
    return 1;
    
  // reads path/to/trajectories-file from fp and saves it in output array of char 'trajfile'
  if (get_a_line(fp, *trajfile))
    return 1;
    
  // reads path/to/energies-file from fp and saves it in output array of char 'ergfile'
  if (get_a_line(fp, *ergfile))
    return 1;
    
  // reads number of steps from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->nsteps = atoi(*line);
    
  // reads size of time step from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  sys->dt = atof(*line);
    
  // reads box-size from fp and saves it in data_struct sys
  if (get_a_line(fp, *line))
    return 1;
  *nprint=atoi(*line);

  return 0;
  
}

void readRestart(FILE *fp, mdsys_t *ptr_sys, char restfile[BLEN])
{
    mdsys_t sys = *ptr_sys;
    
    fp = fopen(restfile, "r");
    if (fp)
    {
        for (int i = 0; i < sys.natoms; ++i)
        {
            fscanf(fp, "%lf%lf%lf", sys.rx + i, sys.ry + i, sys.rz + i);
        }
        for (int i = 0; i < sys.natoms; ++i)
        {
            fscanf(fp, "%lf%lf%lf", sys.vx + i, sys.vy + i, sys.vz + i);
        }
        fclose(fp);
        azzero(sys.fx, sys.natoms);
        azzero(sys.fy, sys.natoms);
        azzero(sys.fz, sys.natoms);
    }
    else
    {
        perror("cannot read restart file");
        //return 3; I am not sure yet why we have to return 3
    }
}