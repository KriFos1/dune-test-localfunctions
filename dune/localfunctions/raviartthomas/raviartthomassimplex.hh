// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_LOCALFUNCTIONS_RAVIARTTHOMAS_SIMPLEX_HH
#define DUNE_LOCALFUNCTIONS_RAVIARTTHOMAS_SIMPLEX_HH

#include <dune/localfunctions/utility/localfiniteelement.hh>
#include "raviartthomassimplex/raviartthomassimplexbasis.hh"

/**
 * \file
 * \brief Raviart-Thomas local finite elements of arbitrary order
 *        for simplices of arbitrary dimension.
 */

namespace Dune
{
  /**
   * \brief Raviart-Thomas local finite elements of arbitrary order
   *        for simplices of arbitrary dimension.
   *
   * These generic local finite elements are only available for
   * simplex geometry types. The extension to cube and prism
   * elements could be added.
   *
   * \tparam dimDomain dimension of reference elements
   * \tparam D domain for basis functions
   * \tparam R range for basis functions
   * \tparam SF storage field for basis matrix
   * \tparam CF compute field for basis matrix
   */
  template<unsigned int dimDomain, class D, class R,
      class SF=R, class CF=SF>
  class RaviartThomasSimplexLocalFiniteElement
    : public GenericLocalFiniteElement<RaviartThomasBasisFactory<dimDomain, SF, CF>,
          RaviartThomasCoefficientsFactory<dimDomain>,
          RaviartThomasL2InterpolationFactory<dimDomain, SF> >
  {
    typedef GenericLocalFiniteElement<RaviartThomasBasisFactory<dimDomain, SF, CF>,
        RaviartThomasCoefficientsFactory<dimDomain>,
        RaviartThomasL2InterpolationFactory<dimDomain, SF> > Base;
  public:
    using typename Base::Traits;

    /** \todo Please doc me */
    RaviartThomasSimplexLocalFiniteElement(const GeometryType &gt, unsigned int order)
      : Base(gt, order)
    {}
  };

  /**
   * \copydoc Dune::RaviartThomasSimplexLocalFiniteElement
   *
   * \deprecated This class is deprecated and will be removed after Dune 2.3.
   *             Use RaviartThomasSimplexLocalFiniteElement instead.
   **/
  template< unsigned int dimDomain, class D, class R,
      class SF=R, class CF=SF >
  class
  DUNE_DEPRECATED_MSG("Use RaviartThomasSimplexLocalFiniteElement instead")
  RaviartThomasLocalFiniteElement
    : public GenericLocalFiniteElement< RaviartThomasBasisFactory< dimDomain, SF, CF >,
          RaviartThomasCoefficientsFactory< dimDomain >,
          RaviartThomasL2InterpolationFactory< dimDomain, SF > >
  {
  public:
    /** \todo Please doc me */
    RaviartThomasLocalFiniteElement(const GeometryType &gt, unsigned int order)
      : RaviartThomasSimplexLocalFiniteElement<dimDomain, D, R, SF, CF>::
        RaviartThomasSimplexLocalFiniteElement(gt, order)
    {}
  };
} // namespace Dune

#endif // #ifndef DUNE_LOCALFUNCTIONS_RAVIARTTHOMAS_SIMPLEX_HH