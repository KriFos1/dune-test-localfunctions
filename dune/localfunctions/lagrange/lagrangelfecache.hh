// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_LOCALFUNCTIONS_LAGRANGE_LAGRANGELFECACHE_HH
#define DUNE_LOCALFUNCTIONS_LAGRANGE_LAGRANGELFECACHE_HH

#include <vector>
#include <optional>
#include <tuple>
#include <utility>
#include <type_traits>

#include <dune/common/std/type_traits.hh>
#include <dune/common/exceptions.hh>
#include <dune/common/typelist.hh>
#include <dune/common/hybridutilities.hh>

#include <dune/geometry/type.hh>
#include <dune/geometry/typeindex.hh>

#include <dune/localfunctions/lagrange/p0.hh>
#include <dune/localfunctions/lagrange/pk.hh>
#include <dune/localfunctions/lagrange/qk.hh>
#include <dune/localfunctions/lagrange/prismp1.hh>
#include <dune/localfunctions/lagrange/prismp2.hh>
#include <dune/localfunctions/lagrange/pyramidp1.hh>
#include <dune/localfunctions/lagrange/pyramidp2.hh>
#include <dune/localfunctions/common/localfiniteelementvariant.hh>



namespace Dune {



namespace Impl {

  // Provide implemented langrange local finite elements

  template<class D, class R, std::size_t dim, std::size_t order>
  struct ImplementedLagrangeFiniteElements
  {
    static auto get()
    {
      return std::make_tuple(
        std::make_pair(GeometryTypes::simplex(dim), []() { return PkLocalFiniteElement<D,R,dim,order>(); }),
        std::make_pair(GeometryTypes::cube(dim), []() { return QkLocalFiniteElement<D,R,dim,order>(); })
      );
    }
  };

  template<class D, class R, std::size_t dim>
  struct ImplementedLagrangeFiniteElements<D,R,dim,0>
  {
    static auto get()
    {
      return std::make_tuple(
        std::make_pair(GeometryTypes::simplex(dim), []() { return P0LocalFiniteElement<D,R,dim>(GeometryTypes::simplex(dim)); }),
        std::make_pair(GeometryTypes::cube(dim), []() { return P0LocalFiniteElement<D,R,dim>(GeometryTypes::cube(dim)); }),
        std::make_pair(GeometryTypes::none(dim), []() { return P0LocalFiniteElement<D,R,dim>(GeometryTypes::none(dim)); })
      );
    }
  };

  template<class D, class R>
  struct ImplementedLagrangeFiniteElements<D,R,3,0>
  {
    static auto get()
    {
      return std::make_tuple(
        std::make_pair(GeometryTypes::tetrahedron, []() { return P0LocalFiniteElement<D,R,3>(GeometryTypes::tetrahedron); }),
        std::make_pair(GeometryTypes::hexahedron, []() { return P0LocalFiniteElement<D,R,3>(GeometryTypes::hexahedron); }),
        std::make_pair(GeometryTypes::prism, []() { return P0LocalFiniteElement<D,R,3>(GeometryTypes::prism); }),
        std::make_pair(GeometryTypes::pyramid, []() { return P0LocalFiniteElement<D,R,3>(GeometryTypes::pyramid); })
      );
    }
  };

  template<class D, class R>
  struct ImplementedLagrangeFiniteElements<D,R,3,1>
  {
    static auto get()
    {
      return std::make_tuple(
        std::make_pair(GeometryTypes::tetrahedron, []() { return PkLocalFiniteElement<D,R,3,1>(); }),
        std::make_pair(GeometryTypes::hexahedron, []() { return QkLocalFiniteElement<D,R,3,1>(); }),
        std::make_pair(GeometryTypes::prism, []() { return PrismP1LocalFiniteElement<D,R>(); }),
        std::make_pair(GeometryTypes::pyramid, []() { return PyramidP1LocalFiniteElement<D,R>(); })
      );
    }
  };

  template<class D, class R>
  struct ImplementedLagrangeFiniteElements<D,R,3,2>
  {
    static auto get()
    {
      return std::make_tuple(
        std::make_pair(GeometryTypes::tetrahedron, []() { return PkLocalFiniteElement<D,R,3,2>(); }),
        std::make_pair(GeometryTypes::hexahedron, []() { return QkLocalFiniteElement<D,R,3,2>(); }),
        std::make_pair(GeometryTypes::prism, []() { return PrismP2LocalFiniteElement<D,R>(); }),
        std::make_pair(GeometryTypes::pyramid, []() { return PyramidP2LocalFiniteElement<D,R>(); })
      );
    }
  };



  template<template<class...> class Target, class ToDoList, class... Processed>
  struct UniqueTypesHelper;

  template<template<class...> class Target, class... Processed>
  struct UniqueTypesHelper<Target, TypeList<>, Processed...>
  {
    using type = Target<Processed...>;
  };

  template<template<class...> class Target, class T0, class... T, class... Processed>
  struct UniqueTypesHelper<Target, TypeList<T0, T...>, Processed...>
  {
    using type = std::conditional_t<
      Dune::Std::disjunction<std::is_same<T0, Processed>...>::value,
      typename UniqueTypesHelper<Target, TypeList<T...>, Processed...>::type,
      typename UniqueTypesHelper<Target, TypeList<T...>, T0, Processed...>::type>;
  };

  // For a given list of types T... instantiate Target<S...> where
  // S... is generated by removing duplicate types from T... . This
  // is useful for std::variant which does not like to be instantiated
  // with duplicate types.
  template<template<class...> class Target, class... T>
  using UniqueTypes_t = typename UniqueTypesHelper<Target, TypeList<T...>>::type;

} // namespace Impl



/** \brief A cache that stores all available Pk/Qk like local finite elements for the given dimension and order
 *
 * An interface for dealing with different vertex orders is currently missing.
 *
 * \tparam D Type used for domain coordinates
 * \tparam R Type used for shape function values
 * \tparam dim Element dimension
 * \tparam order Element order
 */
template<class D, class R, std::size_t dim, std::size_t order>
class LagrangeFiniteElementCache
{

  template<class LFEImplTuple>
  struct GenerateLFEVariant;

  template<class... LFEImpl>
  struct GenerateLFEVariant<std::tuple<std::pair<GeometryType, LFEImpl>...>>
  {
    using type = Impl::UniqueTypes_t<LocalFiniteElementVariant, decltype(std::declval<LFEImpl>()())...>;
  };

  static auto implementedFiniteElements()
  {
    return Impl::ImplementedLagrangeFiniteElements<D,R,dim, order>::get();
  }

public:

  using FiniteElementType = typename GenerateLFEVariant<decltype(implementedFiniteElements())>::type;

  LagrangeFiniteElementCache()
  {
    cache_.resize(LocalGeometryTypeIndex::size(dim));
    Dune::Hybrid::forEach(implementedFiniteElements(), [&](auto feImpl) {
      cache_[LocalGeometryTypeIndex::index(feImpl.first)].emplace(feImpl.second());
    });
  }

  LagrangeFiniteElementCache(const LagrangeFiniteElementCache& other) = default;

  const auto& get(const GeometryType& gt) const
  {
    if (not(cache_[LocalGeometryTypeIndex::index(gt)]))
      DUNE_THROW(Dune::NotImplemented,"Trying to access LagrangeFiniteElementCache with unsupported GeometryType " << gt);
    return *cache_[LocalGeometryTypeIndex::index(gt)];
  }

private:
  std::vector<std::optional<FiniteElementType>> cache_;
};



} // namespace Dune




#endif // DUNE_LOCALFUNCTIONS_LAGRANGE_LAGRANGELFECACHE_HH