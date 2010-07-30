#include "Tpetra_MatrixMatrix.hpp"

#ifdef HAVE_TPETRA_EXPLICIT_INSTANTIATION

// #include "Tpetra_ExplicitInstantiationHelpers.hpp"

#include "Tpetra_MatrixMatrix_def.hpp"

#include <Kokkos_SerialNode.hpp>
#if defined(HAVE_KOKKOS_TBB)
#  include <Kokkos_TBBNode.hpp>
#endif
#if defined(HAVE_KOKKOS_THREADPOOL)
#  include <Kokkos_TPINode.hpp>
#endif
#if defined(HAVE_KOKKOS_THRUST)
#  include <Kokkos_ThrustGPUNode.hpp>
#endif

namespace Tpetra {

#if defined(HAVE_TPETRA_INST_FLOAT)
  TPETRA_MATRIXMATRIX_INSTANT(float,int,int,Kokkos::SerialNode)
#if defined(HAVE_KOKKOS_TBB)
  TPETRA_MATRIXMATRIX_INSTANT(float,int,int,Kokkos::TBBNode)
#endif
#if defined(HAVE_KOKKOS_THREADPOOL)
  TPETRA_MATRIXMATRIX_INSTANT(float,int,int,Kokkos::TPINode)
#endif
#if defined(HAVE_KOKKOS_THRUST) && defined(HAVE_KOKKOS_CUDA_FLOAT)
    TPETRA_MATRIXMATRIX_INSTANT(float,int,int,Kokkos::ThrustGPUNode)
#endif
#endif

#if defined(HAVE_TPETRA_INST_DOUBLE)
  typedef Kokos::DefaultKernels<double,int,Kokkos::SerialNode>::SparseOps SerialSpMatOps
  TPETRA_MATRIXMATRIX_INSTANT(double,int,int,Kokkos::SerialNode,SerialSpMatOps);
#if defined(HAVE_KOKKOS_TBB)
  typedef Kokos::DefaultKernels<double,int,Kokkos::TBBNode>::SparseOps TBBSpMatOps
  TPETRA_MATRIXMATRIX_INSTANT(double,int,int,Kokkos::TBBNode,TBBSpMatOps);
#endif
#if defined(HAVE_KOKKOS_THREADPOOL)
  typedef Kokos::DefaultKernels<double,int,Kokkos::TPINode>::SparseOps TPISpMatOps
  TPETRA_MATRIXMATRIX_INSTANT(double,int,int,Kokkos::TPINode,TPISpMatOps);
#endif
#if defined(HAVE_KOKKOS_THRUST) && defined(HAVE_KOKKOS_CUDA_DOUBLE)
  typedef Kokos::DefaultKernels<double,int,Kokkos::ThrustGPUNode>::SparseOps ThrustGPUSpMatOps
  TPETRA_MATRIXMATRIX_INSTANT(double,int,int,Kokkos::ThrustGPUNode,ThrustGPUSpMatOps);
#endif
#endif

#if defined(HAVE_TPETRA_INST_COMPLEX_FLOAT)
  TPETRA_MATRIXMATRIX_INSTANT(std::complex<float>,int,int,Kokkos::SerialNode)
#if defined(HAVE_KOKKOS_TBB)
  TPETRA_MATRIXMATRIX_INSTANT(std::complex<float>,int,int,Kokkos::TBBNode)
#endif
#if defined(HAVE_KOKKOS_THREADPOOL)
    TPETRA_MATRIXMATRIX_INSTANT(std::complex<float>,int,int,Kokkos::TPINode)
#endif
// no complex on GPU support for now
//#if defined(HAVE_KOKKOS_THRUST) && defined(HAVE_KOKKOS_CUDA_DOUBLE)
//    TPETRA_MATRIXMATRIX_INSTANT(double,int,int,Kokkos::ThrustGPUNode)
//#endif
#endif

#if defined(HAVE_TPETRA_INST_COMPLEX_DOUBLE)
  TPETRA_MATRIXMATRIX_INSTANT(std::complex<double>,int,int,Kokkos::SerialNode)
#if defined(HAVE_KOKKOS_TBB)
  TPETRA_MATRIXMATRIX_INSTANT(std::complex<double>,int,int,Kokkos::TBBNode)
#endif
#if defined(HAVE_KOKKOS_THREADPOOL)
    TPETRA_MATRIXMATRIX_INSTANT(std::complex<double>,int,int,Kokkos::TPINode)
#endif
// no complex on GPU support for now
//#if defined(HAVE_KOKKOS_THRUST) && defined(HAVE_KOKKOS_CUDA_DOUBLE)
//    TPETRA_MATRIXMATRIX_INSTANT(double,int,int,Kokkos::ThrustGPUNode)
//#endif
#endif


} // namespace Tpetra

#endif // HAVE_TPETRA_EXPLICIT_INSTANTIATION
