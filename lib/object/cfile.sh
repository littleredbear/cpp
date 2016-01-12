#!/usr/bin/env bash

file=$1
if [[ $2 =~ ".h" ]]
then
	file2=${file#*,}
	echo ${file2//,/ }
else
	echo ${file%%,*}
fi
