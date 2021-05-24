#!/bin/bash -e

echo "=========================="
echo "Detect wrapper compiler"
echo "=========================="
set +e
PCC=`which pmixcc`
if [ $? != 0 ] ; then
    PCC=`which pcc`
    if [ $? != 0 ] ; then
        echo "ERROR: Failed to find a wrapper compiler"
        exit 1
    fi
fi
echo "Compiler: $PCC"
echo ${PCC} --showme
set -e

  # Build MPIR shim tests
${PCC} -Wall -g -o hello hello.c
if [ $? -ne 0 ] ; then
    echo "Compilation of hello failed"
    exit 1
fi
for program in attach_test launch_test 
do
    echo "=========================="
    echo "Building PMIx ${program}"
    echo "=========================="
    ${PCC} -Wall -g -o ${program} -I../../src/include ${program}.c -L../../src/.libs -lmpirshimtest
    if [ $? -ne 0 ] ; then
        echo "Compilation of $program failed"
        exit 1
    fi
done
