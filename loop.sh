#!/bin/sh
for ((i=150; i<170; ++i )) ; 
do
    echo "$i"
    sh ./getFileListRunNumberSum.sh $i $1
done
