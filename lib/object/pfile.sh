#!/usr/bin/env bash


if [ ${#1} -eq 0 ]
then
	exit
fi

cfile=$1
dir_C=${cfile%/*}
dir_H=${dir_C%/*}"/include/"

hlist=()

getAllHfiles () {
	local hfiles=`sed -n 's/.*#include "\(.*\)".*/\1/p' $2$1`
	for hfile in $hfiles
	do
		if [[ !(${hlist[@]} =~ ${hfile}) ]]
		then
			hlist+=(${hfile})
			getAllHfiles ${hfile} ${dir_H}
		else
			continue
		fi	
	done

}


getAllHfiles $1


echo ${hlist[@]}

