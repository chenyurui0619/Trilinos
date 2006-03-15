// @HEADER
// ***********************************************************************
// 
//    Thyra: Interfaces and Support for Abstract Numerical Algorithms
//                 Copyright (2004) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ***********************************************************************
// @HEADER

#ifndef THYRA_PRECONDITIONER_FACTORY_BASE_DECL_HPP
#define THYRA_PRECONDITIONER_FACTORY_BASE_DECL_HPP

#include "Thyra_SolveSupportTypes.hpp"
#include "Thyra_PreconditionerBase.hpp"
#include "Teuchos_Describable.hpp"
#include "Teuchos_ParameterListAcceptor.hpp"

namespace Thyra {

/** \brief Factory interface for creating precondtioner objects from
 * <tt>LinearOpBase</tt> objects.
 *
 * ToDo: Finish documentation!
 */
template <class RangeScalar, class DomainScalar = RangeScalar>
class PreconditionerFactoryBase
  : virtual public Teuchos::Describable
  , virtual public Teuchos::ParameterListAcceptor
{
public:

  /** @name Pure virtual public functions that must be overridden in subclasses */
  //@{

  /** \brief Check that a <tt>LinearOpBase</tt> object is compatible with
   * <tt>*this</tt> factory object.
   */
  virtual bool isCompatible( const LinearOpBase<RangeScalar,DomainScalar> &fwdOp ) const = 0;

  /** \brief Create an (uninitialized) <tt>LinearOpBase</tt> object to be
   * initalized as the preconditioner later in
   * <tt>this->initializePrecOp()</tt>.
   *
   * Note that on output <tt>return->domain().get()==NULL</tt> may be true
   * which means that the operator is not fully initialized.  In fact, the
   * output operator object is not guaranteed to be fully initialized until
   * after it is passed through <tt>this->initializePrecOp()</tt>.
   */
  virtual Teuchos::RefCountPtr<PreconditionerBase<DomainScalar,RangeScalar> > createPrec() const = 0;

  /** \brief Initialize a pre-created <tt>LinearOpBase</tt> preconditioner
   * object given a "compatible" <tt>LinearOpBase</tt> object.
   *
   * \param  fwdOp  [in] The forward linear operator that will be used to create
   *                the output <tt>LinearOpBase</tt> preconditioner object.
   *                Note that this object is remembered by the <tt>*Op</tt> object on output.
   * \param  prec   [in/out] The output <tt>PreconditionerBase</tt> preconditioner object.  This object must have
   *                be created first by <tt>this->createPrec()</tt>.  The object may have also
   *                already been passed through this function several times.  Note that subclasses
   *                should always first strip off the transpose and scaling by calling <tt>unwrap()</tt>
   *                before attempting to dynamic cast the object.
   * \param  supportSolveUse
   *                [in] Determines if <tt>apply()</tt> and/or <tt>applyTranspose()</tt> will
   *                be called on the initialized preconditioner operators.  This allows <tt>*this</tt>
   *                factory object determine how to best initialize the <tt>*prec</tt> object.
   *                Default <tt>supportSolveUse=SUPPORT_SOLVE_UNSPECIFIED</tt>
   *
   * <b>Preconditions:</b><ul>
   * <li><tt>fwdOp.get()!=NULL</tt>
   * <li><tt>this->isCompatible(*fwdOp)==true</tt>
   * <li><tt>prec!=NULL</tt>
   * <li><tt>*prec</tt> must have been created by <tt>this->createPrec()</tt> prior to calling
   *     this function.
   * <li>[<tt>supportSolveUse==SUPPORT_SOLVE_FORWARD_ONLY<tt>]
   *     <tt>this->applySupportsConj(conj)==true</tt> for any value of <tt>conj</tt>
   * <li>[<tt>supportSolveUse==SUPPORT_SOLVE_TRANSPOSE_ONLY<tt>]
   *     <tt>this->applyTransposeSupportsConj(conj)==true</tt> for any value of <tt>conj</tt>
   * <li>[<tt>supportSolveUse==SUPPORT_SOLVE_FORWARD_AND_TRANSPOSE<tt>]
   *     <tt>this->applySupportsConj(conj)==true && this->applyTransposeSupportsConj(conj)==true</tt>
   *     for any value of <tt>conj</tt>
   * </ul>
   *
   * <b>Postconditions:</b><ul>
   * <li>Throws <tt>CatastrophicSolveFailure</tt> if the preconditioner could
   *     not be created successfully (e.g. due to a factorization failure or some other cause).
   * <li><tt>precOp->range()->isCompatible(*fwdOp->domain())==true</tt>
   * <li><tt>precOp->domain()->isCompatible(*fwdOp->range())==true</tt>
   * <li><t>precOp->applySupportsConj(conj)==this->applySupportsConj(conj)</tt>
   * <li><t>precOp->applyTransposeSupportsConj(conj)==this->applyTransposeSupportsConj(conj)</tt>
   * <li><tt>fwdOp.count()</tt> after output is greater than <tt>fwdOp.count()</tt>
   *     just before this call and therefore the client can assume that the <tt>*fwdOp</tt> object will 
   *     be remembered by the <tt>*prec</tt> object.  The client must be careful
   *     not to modify the <tt>*fwdOp</tt> object or else the <tt>*precOp</tt> object may also
   *     be modified and become invalid.
   * </ul>
   */
  virtual void initializePrec(
    const Teuchos::RefCountPtr<const LinearOpBase<RangeScalar,DomainScalar> >    &fwdOp
    ,PreconditionerBase<DomainScalar,RangeScalar>                                *precOp
    ,const ESupportSolveUse                                                      supportSolveUse = SUPPORT_SOLVE_UNSPECIFIED
    ) const = 0;

  /** \brief Uninitialize a <tt>LinearOpBase</tt> preconditioner object and
   * return its remembered forward linear operator.
   *
   * \param  prec   [in/out] On input, <tt>*precOp</tt> is an initialized or uninitialized
   *                object and on output is uninitialized.  Note that "uninitialized"
   *                does not mean that <tt>precOp</tt> is completely stateless.  It may still
   *                remember some aspect of the matrix <tt>fwdOp</tt> that will allow
   *                for a more efficient initialization next time through
   *                <tt>this->initializePrecOp()</tt>.
   * \param  fwdOp  [in/out] If <tt>fwdOp!=NULL</tt> on input, then on output this is set to
   *                same forward operator passed into <tt>this->initializePrecOp()</tt>.
   * \param  ESupportSolveUse
   *                [in/out] If <tt>precOpType!=NULL</tt> on input, then on output this is set to
   *                same option value passed to <tt>this->initializePrecOp()</tt>.
   *                
   *
   * <b>Preconditions:</b><ul>
   * <li><tt>*precOp</tt> must have been created by <tt>this->createPrecOp()</tt> prior to calling
   *     this function.
   * <li><tt>precOp</tt> may or may not have been passed through a call to
   *     <tt>this->initializePrecOp()</tt>.
   * </ul>
   *
   * <b>Postconditions:</b><ul>
   * <li>If <tt>*precOp</tt> on input was initialized through a call to <tt>this->initializePrecOp()</tt>
   *     then <tt>return.get()!=NULL</tt>.
   * <li>If <tt>*precOp</tt> was uninitialized on input and <tt>fwdOp!=NULL</tt> then <tt>fwdOp->get()==NULL</tt> on output.
   * <li>On output, <tt>*precOp</tt> can be considered to be uninitialized and
   *     it is safe to modify the forward operator object <tt>*(*fwdOp)</tt> returned in <tt>fwdOp</tt>.
   *     The default is <tt>fwdOp==NULL</tt> in which case the forward operator will not be returned in <tt>*fwdOp</tt>.
   * </ul>
   *
   * This function should be called before the forward operator passed in to
   * <tt>this->initializePrecOp()</tt> is modified.  Otherwise, <tt>*this</tt>
   * could be left in an inconsistent state.  However, this is not required.
   */
  virtual void uninitializePrec(
    PreconditionerBase<DomainScalar,RangeScalar>                          *prec
    ,Teuchos::RefCountPtr<const LinearOpBase<RangeScalar,DomainScalar> >  *fwdOp           = NULL
    ,ESupportSolveUse                                                     *supportSolveUse = NULL
    ) const = 0;
  
  //@}

  /** @name Virtual public functions with default implementations */
  //@{

  /** \brief Return if <tt>precOp->apply()</tt> supports the argument <tt>conj</tt>.
   *
   * The default implementation returns <tt>true</tt> for real valued scalar types
   * or when <tt>conj==NONCONJ_ELE</tt> for complex valued types.
   */
  virtual bool applySupportsConj(EConj conj) const;

  /** \brief Return if <tt>precOp->solveTranspose()</tt> supports the argument
   * <tt>conj</tt>.
   *
   * The default implementation returns <tt>false</tt>.
   */
  virtual bool applyTransposeSupportsConj(EConj conj) const;

  //@}

};

// /////////////////////////////////////
// Implementations

template <class RangeScalar, class DomainScalar>
bool PreconditionerFactoryBase<RangeScalar,DomainScalar>::applySupportsConj(EConj conj) const
{
  return true;
}

template <class RangeScalar, class DomainScalar>
bool PreconditionerFactoryBase<RangeScalar,DomainScalar>::applyTransposeSupportsConj(EConj conj) const
{
  return false;
}

} // namespace Thyra

#endif // THYRA_PRECONDITIONER_FACTORY_BASE_DECL_HPP
