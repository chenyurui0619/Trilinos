// @HEADER
// ************************************************************************
//
//                           Intrepid2 Package
//                 Copyright (2007) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Kyungjoo Kim  (kyukim@sandia.gov), or
//                    Mauro Perego  (mperego@sandia.gov)
//
// ************************************************************************
// @HEADER

/** \file   Intrepid_BasisDef.hpp
    \brief  Implementation file for the abstract base class Intrepid2::Basis.
    \author Created by P. Bochev and D. Ridzal.
            Kokkorized by Kyungjoo Kim
*/

#ifndef __INTREPID2_BASIS_DEF_HPP__
#define __INTREPID2_BASIS_DEF_HPP__

namespace Intrepid2 {

  //--------------------------------------------------------------------------------------------//
  //                                                                                            //
  //                            Helper functions of the Basis class                             //
  //                                                                                            //
  //--------------------------------------------------------------------------------------------//

  template<typename ouputValueViewType,
           typename inputPointViewType>
  void getValues_HGRAD_Args( /**/  outputValueViewType   outputValues,
                             const intputPointViewType   inputPoints,
                             const EOperator             operatorType,
                             const shards::CellTopology &cellTopo,
                             const ordinal_tyoe          basisCard ) {
    const auto spaceDim = cellTopo.getDimension();

    // Verify inputPoints array
    INTREPID2_TEST_FOR_EXCEPTION( !(inputPoints.rank() == 2), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) rank = 2 required for inputPoints array");

    INTREPID2_TEST_FOR_EXCEPTION(  (inputPoints.dimension(0) <= 0), std::invalid_argument,
                                   ">>> ERROR (Intrepid2::getValues_HGRAD_Args): dim 0 (number of points) > 0 required for inputPoints array");

    INTREPID2_TEST_FOR_EXCEPTION( !(static_cast<index_type>(inputPoints.dimension(1)) == static_cast<index_type>(spaceDim)), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) dim 1 (spatial dimension) of inputPoints array  does not match cell dimension");


    // Verify that all inputPoints are in the reference cell
    /*
      INTREPID2_TEST_FOR_EXCEPTION( !CellTools<Scalar>::checkPointSetInclusion(inputPoints, cellTopo), std::invalid_argument,
      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) One or more points are outside the "
      << cellTopo <<" reference cell");
    */


    // Verify that operatorType is admissible for HGRAD fields
    INTREPID2_TEST_FOR_EXCEPTION( ( (spaceDim == 2) && (operatorType == OPERATOR_DIV) ), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) DIV is invalid operator for rank-0 (scalar) fields in 2D.");

    INTREPID2_TEST_FOR_EXCEPTION( ( (spaceDim == 3) && ( (operatorType == OPERATOR_DIV) ||
                                                         (operatorType == OPERATOR_CURL) ) ), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) DIV and CURL are invalid operators for rank-0 (scalar) fields in 3D.");


    // Check rank of outputValues (all operators are admissible in 1D) and its dim 2 when operator is
    // GRAD, CURL (only in 2D), or Dk.

    if(spaceDim == 1) {
      switch(operatorType){
      case OPERATOR_VALUE:
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 2), std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) rank = 2 required for outputValues when operator = VALUE.");
        break;
      case OPERATOR_GRAD:
      case OPERATOR_CURL:
      case OPERATOR_DIV:
      case OPERATOR_D1:
      case OPERATOR_D2:
      case OPERATOR_D3:
      case OPERATOR_D4:
      case OPERATOR_D5:
      case OPERATOR_D6:
      case OPERATOR_D7:
      case OPERATOR_D8:
      case OPERATOR_D9:
      case OPERATOR_D10:
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 3), std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) rank = 3 required for outputValues in 1D when operator = GRAD, CURL, DIV, or Dk.");

        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(2) == 1 ),
                                      std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) dim 2 of outputValues must equal 1 when operator = GRAD, CURL, DIV, or Dk.");
        break;
      default:
        INTREPID2_TEST_FOR_EXCEPTION( (true), std::invalid_argument, ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) Invalid operator");
      }
    }
    else if(spaceDim > 1) {
      switch(operatorType){
      case OPERATOR_VALUE:
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 2), std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) rank = 2 required for outputValues when operator = VALUE.");
        break;
      case OPERATOR_GRAD:
      case OPERATOR_CURL:
      case OPERATOR_D1:
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 3), std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) rank = 3 required for outputValues in 2D and 3D when operator = GRAD, CURL (in 2D), or Dk.");

        INTREPID2_TEST_FOR_EXCEPTION( !(static_cast<index_type>(outputValues.dimension(2)) == static_cast<index_type>(spaceDim) ),
                                      std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) dim 2 of outputValues must equal cell dimension when operator = GRAD, CURL (in 2D), or D1.");
        break;
      case OPERATOR_D2:
      case OPERATOR_D3:
      case OPERATOR_D4:
      case OPERATOR_D5:
      case OPERATOR_D6:
      case OPERATOR_D7:
      case OPERATOR_D8:
      case OPERATOR_D9:
      case OPERATOR_D10:
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 3), std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) rank = 3 required for outputValues in 2D and 3D when operator = GRAD, CURL (in 2D), or Dk.");

        INTREPID2_TEST_FOR_EXCEPTION( !(static_cast<index_type>(outputValues.dimension(2)) == static_cast<index_type>(Intrepid2::getDkCardinality(operatorType, spaceDim)) ),
                                      std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) dim 2 of outputValues must equal cardinality of the Dk multiset.");
        break;
      default:
        INTREPID2_TEST_FOR_EXCEPTION( (true), std::invalid_argument, ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) Invalid operator");
      }
    }


    // Verify dim 0 and dim 1 of outputValues
    INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(1) == inputPoints.dimension(0) ),
                                  std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) dim 1 (number of points) of outputValues must equal dim 0 of inputPoints.");

    INTREPID2_TEST_FOR_EXCEPTION( !(static_cast<index_type>(outputValues.dimension(0)) == static_cast<index_type>(basisCard) ),
                                  std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HGRAD_Args) dim 0 (number of basis functions) of outputValues must equal basis cardinality.");
  }


  template<typename outputValueViewType,
           typename inputPointViewType>
  void getValues_HCURL_Args( /**/  outputValueViewType   outputValues,
                             const intputPointViewType   inputPoints,
                             const EOperator             operatorType,
                             const shards::CellTopology &cellTopo,
                             const ordinal_type          basisCard ) {

    const auto spaceDim = cellTopo.getDimension();

    // Verify that cell is 2D or 3D (this is redundant for default bases where we use correct cells,
    //  but will force any user-defined basis for HCURL spaces to use 2D or 3D cells
    INTREPID2_TEST_FOR_EXCEPTION( !( (spaceDim == 2) || (spaceDim == 3) ), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HCURL_Args) cell dimension = 2 or 3 required for HCURL basis");


    // Verify inputPoints array
    INTREPID2_TEST_FOR_EXCEPTION( !(inputPoints.rank() == 2), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HCURL_Args) rank = 2 required for inputPoints array");
    INTREPID2_TEST_FOR_EXCEPTION(  (inputPoints.dimension(0) <= 0), std::invalid_argument,
                                   ">>> ERROR (Intrepid2::getValues_HCURL_Args): dim 0 (number of points) > 0 required for inputPoints array");

    INTREPID2_TEST_FOR_EXCEPTION( !(static_cast<index_type>(inputPoints.dimension(1)) == static_cast<index_type>(spaceDim)), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HCURL_Args) dim 1 (spatial dimension) of inputPoints array  does not match cell dimension");

    // Verify that all inputPoints are in the reference cell
    /*
      INTREPID2_TEST_FOR_EXCEPTION( !CellTools<Scalar>::checkPointSetInclusion(inputPoints, cellTopo), std::invalid_argument,
      ">>> ERROR: (Intrepid2::getValues_HCURL_Args) One or more points are outside the "
      << cellTopo <<" reference cell");
    */

    // Verify that operatorType is admissible for HCURL fields
    INTREPID2_TEST_FOR_EXCEPTION( !( (operatorType == OPERATOR_VALUE) || (operatorType == OPERATOR_CURL) ), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HCURL_Args) operator = VALUE or CURL required for HCURL fields.");


    // Check rank of outputValues: for VALUE should always be rank-3 array with (F,P,D) layout
    switch(operatorType) {

    case OPERATOR_VALUE:
      INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 3), std::invalid_argument,
                                    ">>> ERROR: (Intrepid2::getValues_HCURL_Args) rank = 3 required for outputValues when operator is VALUE");
      INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(2) == spaceDim ),
                                    std::invalid_argument,
                                    ">>> ERROR: (Intrepid2::getValues_HCURL_Args) dim 2 of outputValues must equal cell dimension when operator is VALUE.");
      break;

    case OPERATOR_CURL:

      // in 3D we need an (F,P,D) container because CURL gives a vector field:
      if(spaceDim == 3) {
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 3 ) ,
                                      std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HCURL_Args) rank = 3 required for outputValues in 3D when operator is CURL");
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(2) == spaceDim),
                                      std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HCURL_Args) dim 2 of outputValues must equal cell dimension in 3D when operator is CURL.");
      }
      // In 2D we need an (F,P) container because CURL gives a scalar field
      else if(spaceDim == 2) {
        INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 2 ) ,
                                      std::invalid_argument,
                                      ">>> ERROR: (Intrepid2::getValues_HCURL_Args) rank = 2 required for outputValues in 2D when operator is CURL");
      }
      break;

    default:
      INTREPID2_TEST_FOR_EXCEPTION( (true), std::invalid_argument, ">>> ERROR: (Intrepid2::getValues_HCURL_Args) Invalid operator");
    }


    // Verify dim 0 and dim 1 of outputValues
    INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(1) == inputPoints.dimension(0) ),
                                  std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HCURL_Args) dim 1 (number of points) of outputValues must equal dim 0 of inputPoints.");

    INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(0) == basisCard ),
                                  std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HCURL_Args) dim 0 (number of basis functions) of outputValues must equal basis cardinality.");

  }



  template<typename outputValueViewType,
           typename inputPointViewType>
  void getValues_HDIV_Args(  /**/  outputValueViewType   outputValues,
                             const intputPointViewType   inputPoints,
                             const EOperator             operatorType,
                             const shards::CellTopology &cellTopo,
                             const ordinal_type          basisCard ) {

    const auto spaceDim = cellTopo.getDimension();

    // Verify inputPoints array
    INTREPID2_TEST_FOR_EXCEPTION( !(inputPoints.rank() == 2), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HDIV_Args) rank = 2 required for inputPoints array");
    INTREPID2_TEST_FOR_EXCEPTION(  (inputPoints.dimension(0) <= 0), std::invalid_argument,
                                   ">>> ERROR (Intrepid2::getValues_HDIV_Args): dim 0 (number of points) > 0 required for inputPoints array");

    INTREPID2_TEST_FOR_EXCEPTION( !(static_cast<index_type>(inputPoints.dimension(1)) == static_cast<index_type>(spaceDim)), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HDIV_Args) dim 1 (spatial dimension) of inputPoints array  does not match cell dimension");

    // Verify that all inputPoints are in the reference cell
    /*
      INTREPID2_TEST_FOR_EXCEPTION( !CellTools<Scalar>::checkPointSetInclusion(inputPoints, cellTopo), std::invalid_argument,
      ">>> ERROR: (Intrepid2::getValues_HDIV_Args) One or more points are outside the "
      << cellTopo <<" reference cell");
    */

    // Verify that operatorType is admissible for HDIV fields
    INTREPID2_TEST_FOR_EXCEPTION( !( (operatorType == OPERATOR_VALUE) || (operatorType == OPERATOR_DIV) ), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HDIV_Args) operator = VALUE or DIV required for HDIV fields.");


    // Check rank of outputValues
    switch(operatorType) {
    case OPERATOR_VALUE:
      INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 3), std::invalid_argument,
                                    ">>> ERROR: (Intrepid2::getValues_HDIV_Args) rank = 3 required for outputValues when operator is VALUE.");

      INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(2) == spaceDim ),
                                    std::invalid_argument,
                                    ">>> ERROR: (Intrepid2::getValues_HDIV_Args) dim 2 of outputValues must equal cell dimension for operator VALUE.");
      break;
    case OPERATOR_DIV:
      INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.rank() == 2), std::invalid_argument,
                                    ">>> ERROR: (Intrepid2::getValues_HDIV_Args) rank = 2 required for outputValues when operator is DIV.");
      break;

    default:
      INTREPID2_TEST_FOR_EXCEPTION( (true), std::invalid_argument, ">>> ERROR: (Intrepid2::getValues_HDIV_Args) Invalid operator");
    }


    // Verify dim 0 and dim 1 of outputValues
    INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(1) == inputPoints.dimension(0) ),
                                  std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HDIV_Args) dim 1 (number of points) of outputValues must equal dim 0 of inputPoints.");

    INTREPID2_TEST_FOR_EXCEPTION( !(outputValues.dimension(0) == basisCard ),
                                  std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::getValues_HDIV_Args) dim 0 (number of basis functions) of outputValues must equal basis cardinality.");
  }

}

#endif
