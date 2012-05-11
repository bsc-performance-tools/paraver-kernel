#/bin/bash


# *****************************************************************************
# 1st LEVEL
# *****************************************************************************

ask()
{
  echo
  echo ">>> $1? (n)"
  read answer
  if [[ "$answer" = S || "$answer" = s || "$answer" = Y || "$answer" = y ]]; then
    return 1
  else
    echo
    echo "Bye!"
    exit
  fi
}

existsDir()
{
  firstDir=$1
  if [[ -d $firstDir && -w $firstDir ]]; then
    return 1
  elif [[ -d $PWD/$firstDir && -w $PWD/$firstDir ]]; then
    return 1
  else
    return 0
  fi
}


# *****************************************************************************
# MAIN
# *****************************************************************************


echo
echo "**********************"
echo "*---[ PARAMETERS ]---*"
echo "**********************"

echo
echo "Usage: $0 <testdir> <PARAVER_HOME> <testtrace>"

if [ $# -eq 3 ]; then

  testdir=${1}
  baseprv=${2}/bin
  export PARAVER_HOME=${2}

  if [ ${3} = ${3%.prv} ]; then
    testtrace=${3%.prv.gz}
  elif [ ${3} = ${3%.prv.gz} ]; then
    testtrace=${3%.prv}
  fi

elif [ $# -eq 2 ]; then

  testdir=${1}
  baseprv=${2}/bin
  export PARAVER_HOME=${2}
  testtrace=qsort_beo3

elif [ $# -eq 1 ]; then

  testdir=${1}
  baseprv="/home/pgonzalez/bin/"
  #PARAVER_HOME defined
  testtrace=qsort_beo3

else 

  testdir=$PWD
  baseprv="/home/pgonzalez/bin/"
  #PARAVER_HOME defined
  testtrace=qsort_beo3

fi

trace[0]=$testtrace.prv
trace[1]=$testtrace.prv.gz

echo
echo "test dir      = $testdir"
echo "PARAVER_HOME  = $PARAVER_HOME"
echo "paramedir     = $baseprv/paramedir"
echo "test trace    = ${trace[0]}"
echo "test trace gz = ${trace[1]}"

ask "Continue" 

echo  
echo "  Checking $testdir..."
existsDir $testdir
if [ $? = 0 ]; then
  echo "  Not found! Created."
  mkdir $testdir
else
  echo "  Found $testdir."
fi
echo

#TODO test existance of both .prv.gz and .prv traces


echo
echo "*******************************"
echo "*---[ EXECUTION : BY TIME ]---*"
echo "*******************************"


for i in `seq 0 1`; do
  for j in `seq 0 1`; do
    for k in `seq 0 1`; do
      for l in `seq 0 1`; do
        # to copy from first xml
        # cp cut.bytime.orig-1.brk-1.rmfirst-0.rmlast-1.xml cut.bytime.orig-$i.brk-$j.rmfirst-$k.rmlast-$l.xml 
        for m in `seq 0 1`; do

          echo
          echo "Original time: $i Break states: $j Remove first: $k Remove last: $l Gzipped: $m"
          echo
          echo "  Executing $baseprv/paramedir ${trace[${m}]} -c cut.bytime.orig-${i}.brk-${j}.rmfirst-${k}.rmlast-${l}.xml... "
          $baseprv/paramedir ${trace[${m}]} -c cut.bytime.orig-${i}.brk-${j}.rmfirst-${k}.rmlast-${l}.xml
          
          mv $testtrace.chop1.prv $testdir/cut.bytime.orig-${i}.brk-${j}.rmfirst-${k}.rmlast-${l}.gz-$m.prv &> /dev/null
          cp $testtrace.pcf       $testdir/cut.bytime.orig-${i}.brk-${j}.rmfirst-${k}.rmlast-${l}.gz-$m.pcf &> /dev/null
          cp $testtrace.row       $testdir/cut.bytime.orig-${i}.brk-${j}.rmfirst-${k}.rmlast-${l}.gz-$m.row &> /dev/null

          echo "  ...done."

        done
      done
    done
  done
done

ask "Analyze with wxparaver"

echo
echo "**********************************"
echo "*---[ ANALYZE WITH WXPARAVER ]---*"
echo "**********************************"


for i in `seq 0 1`; do
  for j in `seq 0 1`; do
    for k in `seq 0 1`; do
      for l in `seq 0 1`; do
        for m in `seq 0 1`; do

          echo "Original time: $i Break states: $j Remove first: $k Remove last: $l Gzipped: $m"
          wxparaver $testdir/cut.bytime.orig-${i}.brk-${j}.rmfirst-${k}.rmlast-${l}.gz-${m}.prv zoom.state_as_is.originaltime-${i}.cfg & 
          sleep 3

        done
      done
    done
  done
done
