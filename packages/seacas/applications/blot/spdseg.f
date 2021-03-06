C Copyright(C) 1999-2020 National Technology & Engineering Solutions
C of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C NTESS, the U.S. Government retains certain rights in this software.
C 
C See packages/seacas/LICENSE for details

C $Log: spdseg.f,v $
C Revision 1.3  2009/03/25 12:36:48  gdsjaar
C Add copyright and license notice to all files.
C Permission to assert copyright has been granted; blot is now open source, BSD
C
C Revision 1.2  1999/03/09 19:41:42  gdsjaar
C Fixed missing parameter definition of MSHBOR in blotII2.f
C
C Cleaned up parameters and common blocks in other routines.
C
C Revision 1.1  1994/04/07 20:14:06  gdsjaar
C Initial checkin of ACCESS/graphics/blotII2
C
CRevision 1.2  1990/12/14  08:58:13  gdsjaar
CAdded RCS Id and Log to all files
C
C=======================================================================
      SUBROUTINE SPDSEG (A, NENUM, IE2ELB, ISEVOK,
     &   NSEGV, IXSEGV, KSEGEL)
C=======================================================================

C   --*** SPDSEG *** (SPLOT) Identify variables with undefined elements
C   --   Written by Amy Gilkey - revised 05/05/88
C   --
C   --SPDSEG identifies plot variables that must be drawn in segments.
C   --These are element variables whose value is not defined for some
C   --selected elements.  For each segmented variable, a list of the
C   --defined element indices is returned.
C   --
C   --Parameters:
C   --   A - IN/OUT - the dynamic memory base array
C   --   NENUM - IN - the list of nodes/elements
C   --   IE2ELB - IN - the element block for each element
C   --   ISEVOK - IN - the element block variable truth table;
C   --      variable i of block j exists iff ISEVOK(j,i)
C   --   NSEGV - OUT - the number of "segmented" variables
C   --   IXSEGV - OUT - the index into ISEGEL for "segmented" variables;
C   --      0 if all selected elements are defined
C   --   KSEGEL - OUT - the dynamic memory index of ISEGEL - the NENUM indices
C   --      of the defined elements
C   --
C   --Common Variables:
C   --   Uses NNENUM of /SELNE/
C   --   Uses NSPVAR of /SPVARS/

      include 'params.blk'
      include 'dbnums.blk'
      include 'selne.blk'
      include 'spvars.blk'

      DIMENSION A(*)
      INTEGER NENUM(*)
      INTEGER IE2ELB(*)
      LOGICAL ISEVOK(NELBLK,NVAREL)
      INTEGER IXSEGV(*)

      CHARACTER CDUM

      IF (NODVAR) THEN
         NSEGV = 0
         CALL INIINT (NSPVAR, 0, IXSEGV)
         KSEGEL = 1

      ELSE
         CALL MDRSRV ('ISEGEL', KSEGEL, 0)
         CALL MDSTAT (NERR, MEM)
         IF (NERR .GT. 0) GOTO 110
         NSEGV = 0

         DO 100 N = 1, NSPVAR
            IXSEGV(N) = 0

            CALL DBVTYP_BL (ISVID(N), CDUM, ID)
            IF (NUMEQL (.FALSE., NELBLK, ISEVOK(1,ID)) .GT. 0) THEN
               CALL MDLONG ('ISEGEL', KSEGEL, (NSEGV+1) * (1+NNENUM))
               CALL MDSTAT (NERR, MEM)
               IF (NERR .GT. 0) GOTO 110

               CALL SPDSG1 (NNENUM, NENUM, IE2ELB, ISEVOK(1,ID),
     &            NEL, A(KSEGEL))

               IF (NEL .LT. NNENUM) THEN
                  NSEGV = NSEGV + 1
                  IXSEGV(N) = NSEGV
               END IF
            END IF
  100    CONTINUE

         IF (NSEGV .LE. 0) THEN
            CALL MDDEL ('ISEGEL')
            CALL MDSTAT (NERR, MEM)
            IF (NERR .GT. 0) GOTO 110
            KSEGEL = 1
         END IF
      END IF

  110 CONTINUE
      RETURN
      END
