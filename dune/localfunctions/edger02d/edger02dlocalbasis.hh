// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_EDGER02DLOCALBASIS_HH
#define DUNE_EDGER02DLOCALBASIS_HH

#include "../common/localbasis.hh"

namespace Dune
{
  /**@ingroup LocalBasisImplementation
     \brief Lowest order edge shape functions of on the reference rectangle.

     These are from Jianming Jin: "The Finite Element Method in
     Electromagnetics" 2nd edition, section 8.1.1 "Rectangular Elements".

     These will only work when the transformation from reference element to
     grid element only involves stretching and shifting.  In particular,
     rotation, shearing and second order transformations will destroy
     important properties of the basis.

     - <tt>D</tt>: Type to represent the field in the domain.
     - <tt>R</tt>: Type to represent the field in the range.

     \nosubgrouping
   */
  template<class D, class R>
  class EdgeR02DLocalBasis
#ifdef DUNE_VIRTUAL_SHAPEFUNCTIONS
    : public C1LocalBasisInterface<
          C1LocalBasisTraits<
              D, 2,
              Dune::FieldVector<D,2>,
              R, 2,
              Dune::FieldVector<R,3>,
              Dune::FieldVector<Dune::FieldVector<R,2>, 1>
              > >
#endif
  {
  public:
    typedef C1LocalBasisTraits<
        D, 2,
        Dune::FieldVector<D,2>,
        R, 2,
        Dune::FieldVector<R,3>,
        Dune::FieldVector<Dune::FieldVector<R,2>, 1>
        > Traits;

    //! \brief number of shape functions
    unsigned int size () const
    {
      return 4;
    }

    //! \brief Evaluate all shape functions
    inline void evaluateFunction (const typename Traits::DomainType& in,
                                  std::vector<typename Traits::RangeType>& out) const
    {
      out.resize(4);
      out[0] = 0; out[0][0] = 1-in[1];
      out[1] = 0; out[1][0] =   in[1];
      out[2] = 0; out[2][1] = 1-in[0];
      out[3] = 0; out[3][1] =   in[0];
    }

    //! \brief Evaluate Jacobian of all shape functions
    inline void
    evaluateJacobian (const typename Traits::DomainType& in,         // position
                      std::vector<typename Traits::JacobianType>& out) const      // return value
    {
      out.resize(4);
      out[0] = 0; out[0][0][1] = -1;
      out[1] = 0; out[1][0][1] = +1;
      out[2] = 0; out[2][1][0] = -1;
      out[3] = 0; out[3][1][0] = +1;
    }

    //! \brief Polynomial order of the shape functions
    unsigned int order () const
    {
      return 1;
    }
  };
}
#endif // DUNE_EDGER02DLOCALBASIS_HH