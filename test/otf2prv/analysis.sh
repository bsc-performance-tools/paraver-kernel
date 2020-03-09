#!/usr/bin/env sh

# Test number of parameters
if [ $# -ne 2 ]; then
  echo "Usage: ${0} application_name working_dir"
  echo
  echo "  application_name: Main source name, without .c extension"
  echo "  working_dir : Where the traces are"
  exit 1
fi

APPL_NAME=${1}
ANALYSIS_DIR=${2}

PWD=`pwd`

cd ${ANALYSIS_DIR}
TRACE_EXTRAE=${APPL_NAME}.prv

echo
echo "wxparaver ${TRACE_EXTRAE} &"
#      wxparaver ${TRACE_EXTRAE} &


cd ${PWD}
