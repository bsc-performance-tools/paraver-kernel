#!/bin/bash  

APPS="hello mdlbrot pi ring"

for APP in ${APPS}; do

  ./validation_single_app.sh ${APP} mpi

done

./validation_single_app.sh openmp_hello_world openmp

