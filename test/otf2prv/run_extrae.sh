#!/usr/bin/env sh

# Environment
export INSTR_PKG=extrae
export BIN_SUFFIX=${INSTR_PKG}.bin

export EXTRAE_HOME=/home/pgonzalez/bsccns/install/extrae
export EXTRAE_CONFIG_FILE=./extrae.xml


# Test number of parameters
if [ $# -lt 1 ] || [ $# -gt 3 ]; then
  echo "Usage: ${0} application_name [ 'mpi'|'openmp' [tmp_working_dir]]"
  echo
  echo "  application_name: Main source name, without .c extension"
  echo "  'mpi'|'openmp'  : Select mode" 
  echo "  tmp_working_dir : If missing, creates a /tmp/appl-name.${INSTR_PKG}-xxx dir."
  exit 1
fi


# Get parameters: application_name (source without .c)
APPL_NAME=${1}
APPL_SRC=${APPL_NAME}.c
APPL_BIN=${APPL_NAME}.${BIN_SUFFIX}
if [ ! -e "${APPL_SRC}" ]; then
  echo "[ ] ERROR: ${APPL_SRC} not found!"
  exit 1
else
  echo "[x] Source is ${APPL_SRC}"
fi

IS_OPENMP=${2}

# Get parameters: tmp_working_dir
if [ $# -lt 3 ]; then
  TMP_DIR=`mktemp -d /tmp/${APPL_NAME}.${INSTR_PKG}-XXXXX`
else
  TMP_DIR=${3}
  if [ ${TMP_DIR} = ${PWD} ] || [ ${TMP_DIR} = "." ]; then
    echo "[x] Warning: Directory ${TMP_DIR} is the current working dir!"
    echo "    Creating different one inside /tmp"
    TMP_DIR=`mktemp -d /tmp/${APPL_NAME}.${INSTR_PKG}-XXXXX`
  fi
fi

if [ ! -e "${TMP_DIR}" ]; then
  echo "[ ] ERROR: ${TMP_DIR} not found!"
  exit 1
else
  echo "[x] Temporal dir is ${TMP_DIR}"
fi


# EXTRAE_CONFIG_FILE
if [ ! -e "${EXTRAE_CONFIG_FILE}" ]; then
  echo "[ ] ERROR: ${EXTRAE_CONFIG_FILE} not found!"
  exit 1
else
  echo "[x] ${EXTRAE_CONFIG_FILE} found"
fi


# Compile
if [ "${APPL_NAME}" = "mdlbrot" ]; then
  echo "[x] Compiling as MPI"
  mpicc ${APPL_SRC} -o ${APPL_BIN} -L/usr/lib -lSDL
elif [ "${IS_OPENMP}" = "openmp" ]; then
  echo "[x] Compiling as OpenMP"
  gcc -fopenmp -o ${APPL_BIN} ${APPL_SRC}
else
  mpicc ${APPL_SRC} -o ${APPL_BIN}
fi

if [ ! -e "${APPL_BIN}" ]; then
  echo "[ ] ERROR: ${APPL_BIN} not compiled!"
  exit 1
else
  echo "[x] ${APPL_SRC} succesfully compiled (${APPL_BIN})"
fi


# Move binary and copy ./extrae.xml (with some changes) to tmp dir 
echo "[x] Moving binary to tmp dir"
mv ./${APPL_BIN} ${TMP_DIR}

echo "[x] Tuning copy of extrae.xml"
cat ./${EXTRAE_CONFIG_FILE} | sed "s|mpi_ping.prv|${APPL_NAME}.prv|g;s|mpi_ping|${APPL_NAME}|g" > ${TMP_DIR}/extrae.xml


# Run application and merge trace
PWD=`pwd`
cd ${TMP_DIR}

if [ "${IS_OPENMP}" = "openmp" ]; then
  export OMP_NUM_THREADS=4
  export LD_PRELOAD=${EXTRAE_HOME}/lib/libomptrace.so
  ./${APPL_BIN} &> ./${APPL_NAME}.${INSTR_PKG}.bin.log
else
  export LD_PRELOAD=${EXTRAE_HOME}/lib/libmpitrace.so
  mpirun -np 4 ./${APPL_BIN} &> ./${APPL_NAME}.${INSTR_PKG}.bin.log
fi
${EXTRAE_HOME}/bin/mpi2prv -f TRACE.mpits -o ${APPL_NAME}.prv &> ./${APPL_NAME}.mpi2prv.log

if [ ! -e "${APPL_NAME}.prv" ]; then
  echo "[ ] ERROR: ${APPL_NAME}.prv hasn't been generated"
  echo "    -> Check run log      : ./${APPL_NAME}.${INSTR_PKG}.bin.log"
  echo "    -> Check prv merge log: ./${APPL_NAME}.mpi2prv.log"
  exit 1
else
  echo "[x] ${APPL_NAME}.prv succesfully generated"
fi

cd ${PWD}

