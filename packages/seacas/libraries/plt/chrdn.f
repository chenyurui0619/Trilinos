C Copyright(C) 1999-2020 National Technology & Engineering Solutions
C of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C NTESS, the U.S. Government retains certain rights in this software.
C 
C See packages/seacas/LICENSE for details

C=======================================================================
      SUBROUTINE CHRDN(LINE1,LINE2)
      CHARACTER*(*) LINE1,LINE2

      L = MIN(LEN(LINE1),LEN(LINE2))
      I = 1
 2000 IF (.NOT. (I.LE.L)) GO TO 2020
      ICH = ICHAR(LINE1(I:I))
      IF (ICH.GE.65 .AND. ICH.LE.90) THEN
         ICH = ICH + 32
      END IF

      LINE2(I:I) = CHAR(ICH)
      I = I + 1
      GO TO 2000

 2020 CONTINUE
      RETURN

      END
