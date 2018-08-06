#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include <vector>

using namespace std;


int main(int argc, char** argv) {
      if(argc<3) {
            std::cerr << "Usage: " << argv[0] 
                      << " i. number of particles ii. packing fraction (e.g. 0.5) iii. dim (optional, default 2)" 
                      << std::endl;
            exit(EXIT_FAILURE);
      }

      srand48(time(NULL));
      FILE *fp     = fopen("initconf.d" , "w"); // stdout;
      int carg     = 1;
      int npart    = atoi(argv[carg++]);  // number of particles
      double pfrac = atof(argv[carg++]);  // packing fraction
      int dim      = 2;
      if(argc==4) dim=atoi(argv[carg++]);
      cerr << "# " << npart << "\t" << pfrac << "\t" << dim << endl;  
      cerr << "# Writing initconf.d... ";

      /*************************************************************************/
      // set diameters with polydispersity 0.1
      /*************************************************************************/
      double Vpart = 0; // volume occupied by particles
      vector<double> diams(npart);
      for(int i=0; i<npart; i++) {
            // diameters: uniform distribution [0.95,1.05)
            double di = (0.95 + drand48()*(1.05-0.95));
            diams[i]  = di;
            if(dim==2)
                  Vpart += M_PI*di*di/4.0;
            else
                  Vpart += M_PI*di*di*di/6.0;
      }
      /*************************************************************************/

      double L   = sqrt(Vpart/pfrac);
      if(dim==3)
            L    = pow(Vpart/pfrac, 1/3.0);
      
      int i,j;
      vector<double> x(npart), y(npart), z(npart), dirx(npart), diry(npart), dirz(npart);
      
      if(dim==2) {
            int nr = 0, nc = 0;
            // ceil(x): rounds x upward, returning the smallest integral value that is not less than x. 2.3 --> 3, -2.3 --> -2
            int m = ceil(sqrt((double)npart)); 
            double a = L/m;
            double dx;

            for (i=0; i<npart; ++i) {
                  
                  // position: square grid
                  x[i] = nc*a;
                  y[i] = nr*a;
                  z[i] = 0.0;
                  
                  nc++;
                  if (nc == m) {
                        nc = 0;
                        nr++;
                  }

                  // set directors [-1 to 1)
                  dx   = 2.0*(drand48()-0.5);
                  dirx[i]     = dx;
                  dx          = 2.0*(drand48()-0.5);
                  diry[i]     = dx;
            }

            // normalize
            double norm;
            for (i=0; i<npart; ++i) {
                  norm = sqrt(dirx[i]*dirx[i] + diry[i]*diry[i]);
                  dirx[i] /= norm;
                  diry[i] /= norm;
            }
      }
      
      if(dim==3) {
            int nx = 0, ny = 0, nz = 0;
            // ceil(x): rounds x upward, returning the smallest integral value that is not less than x. 2.3 --> 3, -2.3 --> -2
            int m = ceil(pow(npart, 1/3.0)); 
            double a = L/m;
            double dx;

            for (i=0; i<npart; ++i) {
                  
                  // position: square grid
                  x[i] = ny*a;
                  y[i] = nx*a;
                  z[i] = nz*a;
                  
                  ny++;
                  if (ny == m) {
                        ny = 0;
                        nx++;
                        if (nx == m) {
                              nx = 0;
                              nz++;
                        }
                  }

                  // set directors [-1 to 1)
                  dx          = 2.0*(drand48()-0.5);
                  dirx[i]     = dx;
                  dx          = 2.0*(drand48()-0.5);
                  diry[i]     = dx;
                  dx          = 2.0*(drand48()-0.5);
                  dirz[i]     = dx;
            }

            // normalize
            double norm;
            for (i=0; i<npart; ++i) { 
                  norm = sqrt(dirx[i]*dirx[i] + diry[i]*diry[i] + dirz[i]*dirz[i]);
                  dirx[i] /= norm;
                  diry[i] /= norm;
                  dirz[i] /= norm;
            }
      }

      fprintf(fp, "# initial configuration\n");
      fprintf(fp, "\n");
      fprintf(fp, "%d atoms\n", npart);
      fprintf(fp, "1 atom types\n");
      fprintf(fp, "\n");
      fprintf(fp, "%15.6f %15.6f xlo xhi\n", 0.0, L);
      fprintf(fp, "%15.6f %15.6f ylo yhi\n", 0.0, L);
      if(dim==2) 
            fprintf(fp, "%15.6f %15.6f zlo zhi\n", -0.1, 0.1);
      if(dim==3) 
            fprintf(fp, "%15.6f %15.6f zlo zhi\n", 0.0, L);
      fprintf(fp, "\n");
      //fprintf(fp, "Masses\n");
      //fprintf(fp, "\n");
      //fprintf(fp, "1 1.0\n");
      //fprintf(fp, "\n");
      fprintf(fp, "Atoms\n");
      fprintf(fp, "\n");

      for (i=0; i<npart; ++i) 
            fprintf(fp, "%6d %5d %15.6e %5d %15.6e %15.6e %15.6e\n", i+1, 1, diams[i], 1, x[i], y[i], z[i]);    
      
      fprintf(fp, "\n");
      fprintf(fp, "Directors\n");
      fprintf(fp, "\n");
      
      for (i=0; i<npart; ++i) { 
            if(dim==2) 
                  fprintf(fp, "%6d %15.6e %15.6e %15.6e %15.6e %15.6e %15.6e\n", i+1, dirx[i], diry[i], 0.0, 0.0, 0.0, 0.0);
            if(dim==3) 
                  fprintf(fp, "%6d %15.6e %15.6e %15.6e %15.6e %15.6e %15.6e\n", i+1, dirx[i], diry[i], dirz[i], 0.0, 0.0, 0.0);
      }

      fprintf(fp, "\n");
      cerr << "Done" << endl;
      return 0;
}
