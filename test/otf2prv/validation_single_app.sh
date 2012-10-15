#!/bin/bash  

echo
echo "    VALIDATION ${1} ${2}"
echo


# Test number of parameters
if [ $# -lt 1 -o $# -gt 2 ]; then
  echo "Usage: ${0} application_name ['mpi'|'openmp']"
  echo
  echo "  application_name: Main source name, without .c extension"
  echo "  'mpi'/'openmp'  : Select mode"
  exit 1
fi


# Get parameters: application_name (source without .c)
APPL_NAME=${1}

TMP_DIR=`mktemp -d /tmp/${APPL_NAME}.validation-XXXXX`

if [ ! -e "${TMP_DIR}" ]; then
  echo "[ ] ERROR: ${TMP_DIR} not found!"
  exit 1
else
  echo "[x] Created temporal dir: ${TMP_DIR}"
fi

IS_OPENMP=${2}

# BRANCH 1 : Execution Extrae
echo
echo "./run_extrae.sh ${APPL_NAME} ${IS_OPENMP} ${TMP_DIR}"
      ./run_extrae.sh ${APPL_NAME} ${IS_OPENMP} ${TMP_DIR}

# BRANCH2 : Execution branch 2: Score-P, complement communications and translate
echo
echo "./run_scorep.sh ${APPL_NAME} ${IS_OPENMP} ${TMP_DIR}"
      ./run_scorep.sh ${APPL_NAME} ${IS_OPENMP} ${TMP_DIR}

OTF2_DIR=`find ${TMP_DIR} -type d -name "scorep*"`
echo
echo "./translate_otf2_to_prv.sh ${OTF2_DIR} ${IS_OPENMP}"
      ./translate_otf2_to_prv.sh ${OTF2_DIR} ${IS_OPENMP}

# Move back to initial directory
mv ${TMP_DIR} .
if [ ! -d "${TMP_DIR}" ]; then
  echo "[x] ${TMP_DIR} moved to current working dir"
else
  echo "[ ] Unable to move ${TMP_DIR} to current working dir"
fi

# VALIDATION
OUTPUT_DIR=$(echo "${TMP_DIR}" | sed 's/.*\///')
echo "OUTPUT DIR ${OUTPUT_DIR}"
./analysis.sh ${APPL_NAME} ${OUTPUT_DIR}




