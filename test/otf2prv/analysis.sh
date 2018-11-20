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
TRACE_OTF2PRV=`find . -name traces.msg_match.otf2.prv -exec ls -1 {} \;`
echo "${TRACE_OTF2PRV}"

echo
echo "wxparaver ${TRACE_EXTRAE} &"
#      wxparaver ${TRACE_EXTRAE} &

echo "wxparaver ${TRACE_OTF2PRV} &"
#      wxparaver ${TRACE_OTF2PRV} &

cd ${PWD}
