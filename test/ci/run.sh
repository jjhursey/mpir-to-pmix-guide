#!/bin/sh
./launch_test
if [ $? -ne 0 ] ; then
    echo "ERROR: shim launch test failed"
    exit 1
fi
./attach_test
if [ $? -ne 0 ] ; then
    echo "ERROR: shim attach test failed"
    exit 1
fi
echo MPIR shim tests successful
exit 0
