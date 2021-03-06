C    Copyright(C) 1999-2020 National Technology & Engineering Solutions
C    of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C    NTESS, the U.S. Government retains certain rights in this software.
C    
C    See packages/seacas/LICENSE for details

C $Id: spiral.f,v 1.1 1990/11/30 11:16:24 gdsjaar Exp $
C $Log: spiral.f,v $
C Revision 1.1  1990/11/30 11:16:24  gdsjaar
C Initial revision
C
C
CC* FILE: [.MAIN]SPIRAL.FOR
CC* MODIFIED BY: TED BLACKER
CC* MODIFICATION DATE: 7/6/90
CC* MODIFICATION: COMPLETED HEADER INFORMATION
C
      REAL FUNCTION SPIRAL  (XA,  XK,  X,  XCEN,  YCEN,  ANGLE)
C***********************************************************************
C
C  FUNCTION SPIRAL = CALCULATES THE Y VALUUE GIVEN THE SPIRAL AND X
C
C***********************************************************************
C
      SPIRAL  =  XA * EXP (XK * ANGLE) * COS (ANGLE) - (X - XCEN)
C
      RETURN
C
      END
