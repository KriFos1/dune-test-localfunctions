// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_P1_LOCALBASIS_HH
#define DUNE_P1_LOCALBASIS_HH

#include "../common/localbasis.hh"

namespace Dune
{
  /**@ingroup LocalBasisImplementation
         \brief Linear Lagrange shape functions on the simplex.

         Defines the linear shape functions on the simplex.

         \tparam D Type to represent the field in the domain.
         \tparam R Type to represent the field in the range.
     \tparam dim The dimension of the simplex

         \nosubgrouping
   */
  template<class D, class R, int dim>
  class P1LocalBasis :
    public C1LocalBasisInterface<
        C1LocalBasisTraits<D,dim,Dune::FieldVector<D,dim>,R,1,Dune::FieldVector<R,1>,
            Dune::FieldVector<Dune::FieldVector<R,dim>,1> >
#ifndef DUNE_VIRTUAL_SHAPEFUNCTIONS
        , P1LocalBasis<D,R,dim>
#endif
        >
  {
  public:
    //! \brief export type traits for function signature
    typedef C1LocalBasisTraits<D,dim,Dune::FieldVector<D,dim>,R,1,Dune::FieldVector<R,1>,
        Dune::FieldVector<Dune::FieldVector<R,dim>,1> > Traits;

    //! \brief number of shape functions
    unsigned int size () const
    {
      return dim+1;
    }

    //! \brief Evaluate all shape functions
    inline void evaluateFunction (const typename Traits::DomainType& in,
                                  std::vector<typename Traits::RangeType>& out) const
    {
      out.resize(size());
      out[0] = 1.0;
      for (size_t i=0; i<dim; i++) {
        out[0]  -= in[i];
        out[i+1] = in[i];
      }
    }

    //! \brief Evaluate Jacobian of all shape functions
    inline void
    evaluateJacobian (const typename Traits::DomainType& in,         // position
                      std::vector<typename Traits::JacobianType>& out) const      // return value
    {
      out.resize(size());

      for (int i=0; i<dim; i++)
        out[0][0][i] = -1;

      for (int i=0; i<dim; i++)
        for (int j=0; j<dim; j++)
          out[i+1][0][j] = (i==j);

    }

    //! \brief Polynomial order of the shape functions
    unsigned int order () const
    {
      return 1;
    }
  };
}
#endif