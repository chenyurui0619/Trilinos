C    Copyright(C) 1999-2020 National Technology & Engineering Solutions
C    of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C    NTESS, the U.S. Government retains certain rights in this software.
C    
C    See packages/seacas/LICENSE for details

C $Id: addsnp.f,v 1.1 1990/11/30 11:03:07 gdsjaar Exp $
C $Log: addsnp.f,v $
C Revision 1.1  1990/11/30 11:03:07  gdsjaar
C Initial revision
C
C
CC* FILE: [.MAIN]ADDSNP.FOR
CC* MODIFIED BY: TED BLACKER
CC* MODIFICATION DATE: 7/6/90
CC* MODIFICATION: COMPLETED HEADER INFORMATION
C
      SUBROUTINE ADDSNP(MSNAP,SNAPDX,NSNAP,INDEX,VALUE,ERR)
C***********************************************************************
C
C   SUBROUTINE ADDSNP = ADDS SNAP GRID LINE DEFINITIONS
C
C***********************************************************************
C
C  VARIABLES USED:
C     MSNAP  = DIMENSION OV SNAP ARRAYS
C     SNAPDX = THE SNAP GRID VALUES ARRAY (X AND Y)
C     NSNAP  = THE NUMBER OF SNAP GRID VALUES IN X AND Y
C     INDEX  = 1 FOR X VALUES, 2 FOR Y VALUES
C     VALUE  = THE GRID VALUE TO BE ADDED
C     KOUNT  = THE LOCATION OF THE SNAPDX VALUE JUST LESS THAN VALUE
C
C***********************************************************************
C
      DIMENSION SNAPDX(2,MSNAP),NSNAP(2)
C
      LOGICAL ERR
C
C  ADD THE SNAP GRID VALUE WHERE IT FITS IN NUMERICAL ORDER
C
      ERR=.FALSE.
C
      IF(NSNAP(INDEX).GT.0)THEN
         KOUNT=0
         DO 100 I=1,NSNAP(INDEX)
            IF(VALUE.LT.SNAPDX(INDEX,I))GOTO 110
            KOUNT=I
C
C  IF THE VALUE IS ALREADY THERE, THEN DON'T ADD IT AGAIN - JUST RETURN
C
            IF(VALUE.EQ.SNAPDX(INDEX,I))RETURN
C
  100    CONTINUE
  110    CONTINUE
         IF(NSNAP(INDEX).EQ.MSNAP)THEN
            CALL MESAGE('** NO MORE ROOM FOR ADDITIONAL GRID LINES **')
            WRITE(*,10000)MSNAP
            ERR=.TRUE.
            RETURN
         ENDIF
         NSNAP(INDEX)=NSNAP(INDEX)+1
         DO 120 I=NSNAP(INDEX),KOUNT+2,-1
            SNAPDX(INDEX,I)=SNAPDX(INDEX,I-1)
  120    CONTINUE
         SNAPDX(INDEX,KOUNT+1)=VALUE
C
C  JUST PUT THE FIRST VALUE WHERE IT BELONGS
C
      ELSE
         NSNAP(INDEX)=1
         SNAPDX(INDEX,1)=VALUE
      ENDIF
C
      RETURN
C
10000 FORMAT(' THE MAXIMUM NUMBER OF GRID LINES IS: ',I10)
C
      END
