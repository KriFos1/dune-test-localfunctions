// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include <config.h>
#include <dune/localfunctions/raviartthomas/raviartthomassimplex/raviartthomassimplexbasis.hh>
#include <dune/localfunctions/utility/field.hh>
#include <dune/localfunctions/utility/basisprint.hh>

/**
 * \file
 * \brief Performs some tests for the generic Raviart-Thomas
 *        shape functions on simplices.
 *
 * The topology can be chosen at compile time by setting TOPOLOGY
 * to a string like
 * \code
 * Pyramid<Pyramid<Point> > >
 * \endcode
 * which generates a 2d simplex. If TOPOLOGY is not set, all
 * topologies up to 4d are tested. Note, this may lead to prolonged
 * compiler runs.
 *
 * For debugging purpuse the functions and the derivatives can be
 * printed. You have to remove some comments to activate this function.
 */

#if HAVE_GMP
typedef Dune::GMPField< 128 > StorageField;
typedef Dune::GMPField< 512 > ComputeField;
#else
typedef double StorageField;
typedef double ComputeField;
#endif

template <class Topology>
bool test(unsigned int order)
{
  bool ret = true;

  for (unsigned int o = 0; o <= order; ++o)
  {
    std::cout << "Testing " << Topology::name() << " in dimension " << Topology::dimension << " with order " << o << std::endl;
    typedef Dune::RaviartThomasBasisFactory<Topology::dimension,StorageField,ComputeField> BasisFactory;
    const typename BasisFactory::Object &basis = *BasisFactory::template create<Topology>(o);

    // uncomment the following lines to get files containing functions and
    // derivatives in a human readabible form (aka LaTeX source)
    //std::stringstream name;
    //name << "rt_" << Topology::name() << "_p" << o << ".basis";
    //std::ofstream out(name.str().c_str());
    //Dune::basisPrint<0,BasisFactory,typename BasisFactory::StorageField>(out,basis);
    //Dune::basisPrint<1,BasisFactory,typename BasisFactory::StorageField>(out,basis);

    // test interpolation
    typedef Dune::RaviartThomasL2InterpolationFactory<Topology::dimension,StorageField> InterpolationFactory;
    const typename InterpolationFactory::Object &interpol = *InterpolationFactory::template create<Topology>(o);
    Dune::LFEMatrix<StorageField> matrix;
    interpol.interpolate(basis,matrix);
    for (unsigned int i=0; i<matrix.rows(); ++i)
      matrix(i,i)-=1;
    for (unsigned int i=0; i<matrix.rows(); ++i)
      for (unsigned int j=0; j<matrix.cols(); ++j)
        if (matrix(i,j)<Dune::Zero<StorageField>() ||
            Dune::Zero<StorageField>()<matrix(i,j))
          std::cout << "  non-zero entry in interpolation matrix: "
                    << "(" << i << "," << j << ") = " << Dune::field_cast<double>(matrix(i,j))
                    << std::endl;

    BasisFactory::release(&basis);
  }
  if (!ret) {
    std::cout << "   FAILED !" << std::endl;
  }
  return ret;
}
int main ( int argc, char **argv )
{
  using namespace Dune;
  using namespace GenericGeometry;

  const unsigned int order = (argc < 2) ? 5 : atoi(argv[1]);

  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[ 0 ] << " <p>" << std::endl
              << "Using default order of " << order << std::endl;
  }
#ifdef TOPOLOGY
  return (test<TOPOLOGY>(order) ? 0 : 1 );
#else
  bool tests = true;
  tests &= test<Pyramid<Point> > (order);

  tests &= test<Pyramid<Pyramid<Point> > >(order);

  tests &= test<Pyramid<Pyramid<Pyramid<Point> > > >(order);

  tests &= test<Pyramid<Pyramid<Pyramid<Pyramid<Point> > > > >(order);
  return (tests ? 0 : 1);
#endif // TOPOLOGY
}