C Copyright(C) 1999-2020 National Technology & Engineering Solutions
C of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C NTESS, the U.S. Government retains certain rights in this software.
C 
C See packages/seacas/LICENSE for details

C $Log: rotxyz.f,v $
C Revision 1.2  2009/03/25 12:36:47  gdsjaar
C Add copyright and license notice to all files.
C Permission to assert copyright has been granted; blot is now open source, BSD
C
C Revision 1.1  1994/04/07 20:10:22  gdsjaar
C Initial checkin of ACCESS/graphics/blotII2
C
c Revision 1.2  1990/12/14  08:56:40  gdsjaar
c Added RCS Id and Log to all files
c
C=======================================================================
      SUBROUTINE ROTXYZ (XYZ, ANG, ROTMAT)
C=======================================================================

C   --*** ROTXYZ *** (MESH) Multiply rotation matrix by XYZ rotation
C   --   Written by Amy Gilkey - revised 05/23/86
C   --
C   --ROTXYZ multiplies the current rotation matrix by the given X or Y
C   --or Z axis rotation.  Thus the XYZ axes are in viewing space, not
C   --object space.
C   --
C   --Parameters:
C   --   XYZ - IN - the axis of rotation (X,Y,Z)
C   --   ANG - IN - the angle of rotation (in radians)
C   --   ROTMAT - IN/OUT - the rotation matrix

      CHARACTER XYZ
      REAL ROTMAT(3,3)

      REAL BY(3,3), RES(3,3)

      IF (XYZ .EQ. 'X') THEN
         N1 = 2
         N2 = 3
         N3 = 1
      ELSE IF (XYZ .EQ. 'Y') THEN
         N1 = 3
         N2 = 1
         N3 = 2
      ELSE IF (XYZ .EQ. 'Z') THEN
         N1 = 1
         N2 = 2
         N3 = 3
      END IF

      COSANG = COS (ANG)
      SINANG = SIN (ANG)
      BY(N1,N1) = COSANG
      BY(N2,N1) = -SINANG
      BY(N1,N3) = 0.0
      BY(N1,N2) = SINANG
      BY(N2,N2) = COSANG
      BY(N2,N3) = 0.0
      BY(N3,N1) = 0.0
      BY(N3,N2) = 0.0
      BY(N3,N3) = 1.0

      DO 110 I = 1, 3
         DO 100 J = 1, 3
            RES(I,J) = ROTMAT(I,1)*BY(1,J) + ROTMAT(I,2)*BY(2,J)
     &         + ROTMAT(I,3)*BY(3,J)
  100    CONTINUE
  110 CONTINUE

      CALL CPYREA (3*3, RES, ROTMAT)

      RETURN
      END
