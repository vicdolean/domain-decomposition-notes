#ifndef WITH_NO_INIT
#include "ff++.hpp"
#endif
#include "AFunction_ext.hpp"

using namespace std;

#include <fstream>

template< char T >
class Data2D {
 public:
  string *file;
  KNM< float > *tab;

  int mo_file_nx, mo_file_ny;
  double mo_file_xend, mo_file_xstart;
  double mo_file_yend, mo_file_ystart;
  double mo_file_dx, mo_file_dy;
  double cfill;

  void init( ) {
    file = 0;
    tab = 0;
  }
  void destroy( ) {
    delete file;
    delete tab;
  }
};

template< char T >
Data2D< T > *init_Data2D(Data2D< T > *const &a, string *const &s) {
  if (verbosity)
    cout << "Reading " << (T == 'M' ? "Marmousi" : "BPSalt") << " Model file " << *s << endl;
  a->file = new string(*s);

  ifstream f((*a->file).c_str( ), ios::in | ios::binary);
  if (!f.is_open( )) {
    cerr << "Error opening " << (*a->file).c_str( ) << ": file does not exist." << endl;
    ffassert(f.is_open( ));
  }

  a->mo_file_xstart = 0.;
  a->mo_file_yend = 0.;
  int sz;
  if (T == 'M') {
    sz = 2301 * 751;
    a->mo_file_nx = 2301;
    a->mo_file_xend = 9.2;
    a->mo_file_ny = 751;
    a->mo_file_ystart = -3.;
    a->tab = new KNM< float >(2301, 751);
  } else {
    sz = 5395 * 1911;
    a->mo_file_nx = 5395;
    a->mo_file_xend = 12.5;
    a->mo_file_ny = 1911;
    a->mo_file_ystart = -6.25;
    a->tab = new KNM< float >(5395, 1911);
  }

  float *buff = new float[sz];

  f.read((char *)buff, sz * sizeof(float));

  f.close( );

  int ix, iy, iz;

  for (iy = 0; iy < a->mo_file_ny; iy++)
    for (ix = 0; ix < a->mo_file_nx; ix++)
      (*a->tab)(ix, a->mo_file_ny - 1 - iy) = buff[a->mo_file_ny * ix + iy];

  delete[] buff;
  if (T == 'S') (*a->tab) /= 1000.0;

  f.close( );
  return a;
}

template< char T >
double Data2D_eval(Data2D< T > *const &a, const double &xi, const double &yi) {
  int ix = a->mo_file_nx * (xi - a->mo_file_xstart) / (a->mo_file_xend - a->mo_file_xstart);
  int iy = a->mo_file_ny * (yi - a->mo_file_ystart) / (a->mo_file_yend - a->mo_file_ystart);
  ix = max(0, min(ix, a->mo_file_nx - 1));
  iy = max(0, min(iy, a->mo_file_ny - 1));

  return (*a->tab)(ix, iy);
}

class Overthrust3d {
 public:
  string *file;
  KNMK< float > *tab;

  int mo_file_nx, mo_file_ny, mo_file_nz;
  double mo_file_xend, mo_file_xstart;
  double mo_file_yend, mo_file_ystart;
  double mo_file_zend, mo_file_zstart;
  double mo_file_dx, mo_file_dy, mo_file_dz;
  double cfill;

  void init( ) {
    file = 0;
    tab = 0;
  }
  void destroy( ) {
    delete file;
    delete tab;
  }
};

Overthrust3d *init_Overthrust3d(Overthrust3d *const &a, string *const &s) {
  if (verbosity) cout << "Reading Overthrust3d Model file " << *s << endl;
  a->file = new string(*s);

  ifstream f((*a->file).c_str( ), ios::in | ios::binary);
  if (!f.is_open( )) {
    cerr << "Error opening " << (*a->file).c_str( ) << ": file does not exist." << endl;
    ffassert(f.is_open( ));
  }

  int sz = 801 * 801 * 187;
  a->mo_file_nx = 801;
  a->mo_file_xstart = 0.;
  a->mo_file_xend = 20.;
  a->mo_file_ny = 801;
  a->mo_file_ystart = 0.;
  a->mo_file_yend = 20.;
  a->mo_file_nz = 187;
  a->mo_file_zstart = -4.65;
  a->mo_file_zend = 0.;

  float *buff = new float[sz];

  f.read((char *)buff, sz * sizeof(float));

  int ix, iy, iz;

  f.close( );

  a->tab = new KNMK< float >(801, 801, 187);

  for (iz = 0; iz < a->mo_file_nz; iz++)
    for (iy = 0; iy < a->mo_file_ny; iy++)
      for (ix = 0; ix < a->mo_file_nx; ix++)
        (*a->tab)(ix, iy, a->mo_file_nz - 1 - iz) =
          buff[a->mo_file_nx * a->mo_file_ny * iz + a->mo_file_nx * iy + ix];

  delete[] buff;

  f.close( );
  return a;
}

double Overthrust3d_eval(Overthrust3d *const &a, const double &xi, const double &yi,
                         const double &zi) {
  int ix = a->mo_file_nx * (xi - a->mo_file_xstart) / (a->mo_file_xend - a->mo_file_xstart);
  int iy = a->mo_file_ny * (yi - a->mo_file_ystart) / (a->mo_file_yend - a->mo_file_ystart);
  int iz = a->mo_file_nz * (zi - a->mo_file_zstart) / (a->mo_file_zend - a->mo_file_zstart);
  ix = max(0, min(ix, a->mo_file_nx - 1));
  iy = max(0, min(iy, a->mo_file_ny - 1));
  iz = max(0, min(iz, a->mo_file_nz - 1));

  return (*a->tab)(ix, iy, iz);
}

static void Load_Init( ) {
  if (verbosity && mpirank == 0) cout << " load: geophysics  " << endl;

  Dcl_Type< Data2D< 'M' > * >(InitP< Data2D< 'M' > >, Destroy< Data2D< 'M' > >);
  zzzfff->Add("Marmousi", atype< Data2D< 'M' > * >( ));
  TheOperators->Add(
    "<-", new OneOperator2_< Data2D< 'M' > *, Data2D< 'M' > *, string * >(&init_Data2D< 'M' >));
  atype< Data2D< 'M' > * >( )->Add(
    "(", "", new OneOperator3_< double, Data2D< 'M' > *, double, double >(Data2D_eval));
  Dcl_Type< Data2D< 'S' > * >(InitP< Data2D< 'S' > >, Destroy< Data2D< 'S' > >);
  zzzfff->Add("BPSalt", atype< Data2D< 'S' > * >( ));
  TheOperators->Add(
    "<-", new OneOperator2_< Data2D< 'S' > *, Data2D< 'S' > *, string * >(&init_Data2D< 'S' >));
  atype< Data2D< 'S' > * >( )->Add(
    "(", "", new OneOperator3_< double, Data2D< 'S' > *, double, double >(Data2D_eval));

  Dcl_Type< Overthrust3d * >(InitP< Overthrust3d >, Destroy< Overthrust3d >);
  zzzfff->Add("Overthrust", atype< Overthrust3d * >( ));
  TheOperators->Add(
    "<-", new OneOperator2_< Overthrust3d *, Overthrust3d *, string * >(&init_Overthrust3d));
  atype< Overthrust3d * >( )->Add(
    "(", "",
    new OneOperator4_< double, Overthrust3d *, double, double, double >(Overthrust3d_eval));
}
LOADFUNC(Load_Init)
