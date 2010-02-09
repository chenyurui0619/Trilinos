#!/bin/bash

CTEST_EXE=/home/trilinos/bin/ctest
EG_EXE=/home/trilinos/bin/eg
BASEDIR=/home/jmwille/TrilinosTestHarness
BASEDATADIR=/home/jmwille/TrilinosTestHarness/nightly
DRIVER_SCRIPT_DIR=$BASEDIR/Trilinos/cmake/ctest/drivers/trilinos-test2
TRILINOS_REPOSITORY_LOCATION="software.sandia.gov:/space/git/Trilinos"

export CMAKE_LIBRARY_PATH="/home/trilinos/tpl/gcc4.1.2/exodusII_4.84/lib:/home/trilinos/tpl/gcc4.1.2/netcdf_4.0/lib"
export CMAKE_INCLUDE_PATH="/home/trilinos/tpl/gcc4.1.2/exodusII_4.84/include:/home/trilinos/tpl/gcc4.1.2/netcdf_4.0/include"
export TRILINOSDATADIRECTORY=$BASEDATADIR/TrilinosData
export PYTHONPATH=/home/trilinos/tpl/gcc4.1.2/numpy1.4.0/lib64/python2.4/site-packages/
export LD_LIBRARY_PATH=/home/jmwille/TrilinosTestHarness/MPI_OPT_DEV_SHARED/BUILD/packages/PyTrilinos/src

#get the date for use in log files
DATE=`date "+%m-%d-%Y"`

echo
echo "Starting nightly Trilinos development testing on trilinos-test2: `date`"
echo

echo
echo "Checking out just the drivers: `date`"
echo


cd $BASEDATADIR
#checkout the trilinos data directory. Needed for a few tests to run
cvs -q -d :ext:software:/space/CVS co TrilinosData

cd $BASEDIR
#checkout the bits of trilinos needed for running the nightly test scripts
if [ -d Trilinos ]; then
  echo Doing an update of existing directory
  cd Trilinos
  $EG_EXE pull
  cd ..
else
  echo Cloning the repository because none exists yets
  $EG_EXE clone $TRILINOS_REPOSITORY_LOCATION
fi


echo
echo "Doing mpi optimized Development build: `date`"
echo

time ${CTEST_EXE} -S $DRIVER_SCRIPT_DIR/ctest_linux_nightly_mpi_development_opt_trilinos-test2.cmake -VV &> "MPI_OPT_DEV_$DATE.log"

echo
echo "Doing mpi debug Development build: `date`"
echo

time ${CTEST_EXE} -S $DRIVER_SCRIPT_DIR/ctest_linux_nightly_mpi_development_debug_trilinos-test2.cmake -VV &> "MPI_DEBUG_DEV_$DATE.log"

echo
echo "Doing mpi optimized development shared library build: `date`"
echo 

time ${CTEST_EXE} -S $DRIVER_SCRIPT_DIR/ctest_linux_nightly_mpi_development_opt_shared_trilinos-test2.cmake -VV &> "MPI_OPT_DEV_SHARED_$DATE.log"

echo
echo "Doing serial debug build: `date`"
echo

time ${CTEST_EXE} -S $DRIVER_SCRIPT_DIR/ctest_linux_nightly_serial_development_debug_trilinos-test2.cmake -VV &> "SERIAL_DEBUG_DEV_$DATE.log"

echo
echo "Ending nightly Trilinos testing on trilinos-test2: `date`"
echo
