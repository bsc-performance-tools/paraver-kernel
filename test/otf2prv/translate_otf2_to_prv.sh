#!/usr/bin/env sh

# Test number of parameters
if [ $# -lt 1 -o $# -gt 2 ]; then
  echo "Usage: ${0} otf2_trace_dir [ 'mpi'|'openmp' ]"
  echo
  echo "  otf_trace_dir   : Directory where is located the file traces.oft2"
  echo "  'mpi'|'openmp' : Select mode (default 'mpi')" 
  exit 1
fi


# Get parameters: Trace OTF2
OTF2_TRACE_DIR=${1}
if [ ! -d "${OTF2_TRACE_DIR}" ]; then
  echo "[ ] ERROR: ${OTF2_TRACE_DIR} not found!"
  exit 1
else
  echo "[x] ${OTF2_TRACE_DIR} found"
fi

OTF2_TRACE=${OTF2_TRACE_DIR}/traces.otf2 
#OTF2_TRACE=`echo "${1}" | sed 's/.*\///'` 
#OTF2_TRACE=$(echo "$1" | sed 's/.*\///')

if [ ! -e "${OTF2_TRACE}" ]; then
  echo "[ ] ERROR: ${OTF2_TRACE} not found!"
  exit 1
else
  echo "[x] ${OTF2_TRACE} found"
fi


IS_OPENMP="mpi"
if [ $# -eq 2 ]; then
  IS_OPENMP=${2}
fi

# Create working dir
TMP_DIR=`mktemp -d /tmp/translation.otf2prv-XXXXX`

if [ ! -e "${TMP_DIR}" ]; then
  echo "[ ] ERROR: ${TMP_DIR} not found!"
  exit 1
else
  echo "[x] Temporal dir is ${TMP_DIR}"
fi


# Test if binaries exist
BIN_OTF2PRV=`which otf2prv.bin`
if [ ! -e "${BIN_OTF2PRV}" ]; then
  echo "[ ] ERROR: otf2prv.bin not found!"
  exit 1
else
  echo "[x] Using ${BIN_OTF2PRV}"
fi

if [ "${IS_OPENMP}" = "mpi" ]; then
  BIN_OTF2_MSG_MATCH=`which otf2_msg_match`
  if [ ! -e "${BIN_OTF2_MSG_MATCH}" ]; then
    echo "[ ] Warning: otf2_msg_match not found. PRV trace won't have communications."
  else
    echo "[x] Using ${BIN_OTF2_MSG_MATCH}"
  fi
fi

BIN_OTF2_PRINT=`which otf2_print`
if [ ! -e "${BIN_OTF2_PRINT}" ]; then
  echo "[ ] Warning: otf2_print not found. No dump will be generated."
else
  echo "[x] Using ${BIN_OTF2_PRINT}"
fi


# Add communications message matching info using otf2_msg_match 
if [ "${IS_OPENMP}" = "mpi" ]; then
  if [ -e "${BIN_OTF2_MSG_MATCH}" ]; then
    echo "$ mpiexec -n 4 ${BIN_OTF2_MSG_MATCH} ${OTF2_TRACE} ${TMP_DIR} traces.msg_match &> traces.msg_match.log"
          mpiexec -n 4 ${BIN_OTF2_MSG_MATCH} ${OTF2_TRACE} ${TMP_DIR} traces.msg_match &> traces.msg_match.log

    OTF2_TRACE_MATCHED=${TMP_DIR}/traces.msg_match.otf2

    if [ ! -e ${OTF2_TRACE_MATCHED} ]; then
      echo "[ ] ERROR: ${OTF2_TRACE_MATCHED} hasn't been generated!"
      exit 1
    else
      echo "[x] Generated ${OTF2_TRACE_MATCHED}"
    fi
  fi
fi


# Dump input otf2 trace using otf2_print
if [ -e "${BIN_OTF2_PRINT}" ]; then
  echo "${BIN_OTF2_PRINT} -G ${OTF2_TRACE} > ${TMP_DIR}/traces.DEF.log"
        ${BIN_OTF2_PRINT} -G ${OTF2_TRACE} > ${TMP_DIR}/traces.DEF.log

  echo "${BIN_OTF2_PRINT}    ${OTF2_TRACE} > ${TMP_DIR}/traces.EVT.log"
        ${BIN_OTF2_PRINT}    ${OTF2_TRACE} > ${TMP_DIR}/traces.EVT.log


  if [ "${IS_OPENMP}" = "mpi" ]; then   
# Dump input otf2 trace (with message match info) using otf2_print
    echo "${BIN_OTF2_PRINT} -G ${OTF2_TRACE_MATCHED} > ${OTF2_TRACE_MATCHED}.DEF.log"
          ${BIN_OTF2_PRINT} -G ${OTF2_TRACE_MATCHED} > ${OTF2_TRACE_MATCHED}.DEF.log

    echo "${BIN_OTF2_PRINT}    ${OTF2_TRACE_MATCHED} > ${OTF2_TRACE_MATCHED}.EVT.log"
          ${BIN_OTF2_PRINT}    ${OTF2_TRACE_MATCHED} > ${OTF2_TRACE_MATCHED}.EVT.log

# Log comparison
#   Using otf2_print, between output for input original otf2 trace and the same one but matched.
#   Compares DEF and EVT output.
    echo "diff ${TMP_DIR}/${OTF2_TRACE}.DEF.log ${OTF2_TRACE_MATCHED}.DEF.log > ${TMP_DIR}/diffs.DEF.log"
          diff ${TMP_DIR}/traces.DEF.log ${OTF2_TRACE_MATCHED}.DEF.log > ${TMP_DIR}/diffs.DEF.log

    echo "diff ${TMP_DIR}/${OTF2_TRACE}.EVT.log ${OTF2_TRACE_MATCHED}.EVT.log > ${TMP_DIR}/diffs.EVT.log"
          diff ${TMP_DIR}/traces.EVT.log ${OTF2_TRACE_MATCHED}.EVT.log > ${TMP_DIR}/diffs.EVT.log
  fi
fi


# Translate otf2 trace with message match info
if [ "${IS_OPENMP}" = "mpi" ]; then   
  echo "${BIN_OTF2PRV} -l ${OTF2_TRACE_MATCHED} > ${OTF2_TRACE_MATCHED}.PRV.log"
        ${BIN_OTF2PRV} -l ${OTF2_TRACE_MATCHED} > ${OTF2_TRACE_MATCHED}.PRV.log
else
  echo "${BIN_OTF2PRV} -l ${OTF2_TRACE} > ${TMP_DIR}/${OTF2_TRACE}.PRV.log"
        ${BIN_OTF2PRV} -l ${OTF2_TRACE} > ${TMP_DIR}/${OTF2_TRACE}.PRV.log
fi


# Move back to initial directory
mv ${TMP_DIR} ${OTF2_TRACE_DIR}
if [ ! -d "${TMP_DIR}" ]; then
  echo "[x] ${TMP_DIR} moved to current working dir"
else
  echo "[ ] Unable to move ${TMP_DIR} to current working dir"
fi


