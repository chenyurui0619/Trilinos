C    Copyright(C) 1999-2020 National Technology & Engineering Solutions
C    of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C    NTESS, the U.S. Government retains certain rights in this software.
C    
C    See packages/seacas/LICENSE for details

C $Id: mmschm.f,v 1.2 1998/07/14 18:19:26 gdsjaar Exp $
C $Log: mmschm.f,v $
C Revision 1.2  1998/07/14 18:19:26  gdsjaar
C Removed unused variables, cleaned up a little.
C
C Changed BLUE labels to GREEN to help visibility on black background
C (indirectly requested by a couple users)
C
C Revision 1.1.1.1  1990/11/30 11:12:21  gdsjaar
C FASTQ Version 2.0X
C
c Revision 1.1  90/11/30  11:12:20  gdsjaar
c Initial revision
c
C
CC* FILE: [.QMESH]MMSCHM.FOR
CC* MODIFIED BY: TED BLACKER
CC* MODIFICATION DATE: 7/6/90
CC* MODIFICATION: COMPLETED HEADER INFORMATION
C
      SUBROUTINE MMSCHM (NPER, KKK, LLL, NNN, ML, MS, NSPR, ISLIST,
     &   NINT, IFLINE, NLPS, ILLIST, LINKL, LINKS, MXNPER, MAXPRM, MAX3,
     &   MXND, X, Y, NID, NNPS, ANGLE, XN, YN, NUID, LXK, KXL, NXL, LXN,
     &   IAVAIL, NAVAIL, CCW, REAL, SCHSTR, M1, ERR)
C***********************************************************************
C
C  MMSCHM - "M" MESH SCHEME; CALCULATE A REGULAR RECTANGULAR MESH
C
C***********************************************************************
C
      DIMENSION ISLIST(NSPR), NINT(ML), IFLINE(MS), NLPS(MS)
      DIMENSION ILLIST(MS*3), LINKL(2, ML), LINKS(2, MS)
      DIMENSION X(MXNPER), Y(MXNPER), NID(MXNPER*MAXPRM), NNPS(MAX3)
      DIMENSION ANGLE(MXNPER), XN(MXND), YN(MXND), NUID(MXND)
      DIMENSION LXK(4, MXND), KXL(2, 3*MXND), NXL(2, 3*MXND)
      DIMENSION LXN(4, MXND)
C
      CHARACTER*72 SCHSTR
C
      LOGICAL CCW, ERR, NORM, REAL
C
C  CALCULATE THE BASE OF THE RECTANGLE FOR THE REGION
C
      CALL GETM1 (ML, MS, MAX3, NSPR, ISLIST, NINT, IFLINE, NLPS,
     &   ILLIST, LINKL, LINKS, X, Y, NID, NNPS, ANGLE, NPER, SCHSTR, M1,
     &   CCW, NORM, REAL, ERR)
      IF (NORM) THEN
         CALL MESAGE ('FORCED RECTANGLE PRIMITIVE PROCESSING USED')
      ELSE
         CALL MESAGE ('GENERAL RECTANGLE PRIMITIVE PROCESSING USED')
      END IF
      M2 = NPER/2 - M1
C
C  CALCULATE A REGULAR MAPPED "RECTANGULAR" MESH
C
      KKKOLD = KKK
      LLLOLD = LLL
      NNNOLD = NNN
      CALL RMESH (NPER, MXND, X, Y, NID, XN, YN, NUID, LXK, KXL, NXL,
     &   LXN, M1, M2, KKK, KKKOLD, NNN, NNNOLD, LLL, LLLOLD, IAVAIL,
     &   NAVAIL, ERR)
C
      RETURN
      END
