#!/usr/bin/env bash


path=$1

outname=${path%.h}
mname=${outname##*lrb}

outname=${outname}'Func'


txt=`sed -n '/struct/,/};/p' $path`

hfuncs='#ifndef LRB_'$mname'_H\n#define LRB_'$mname'_H\n\n#include <string>\n\nnamespace lrb {\n\nnamespace NetWork {\n\n\tclass DataPacker;\n}\n\nnamespace '$mname' {\n\n'

cfuncs='#include "lrb'$mname'Func.h"\n#include "lrbNetWork.h"\n#include "lrb'$mname'.h"\n\nusing namespace lrb::'$mname';\n\nnamespace lrb {\n\nnamespace '$mname' {\n\n'

count=0
function genFunc()
{
	funcName='void pack'$1'(lrb::NetWork::DataPacker *packer,'
	local funcBody=
	IFS=$oldIFS
	for v in $2
	do
		if [[ $v == *, ]]
		then
			p=${v//,/}
			p=${p//\*/}
			funcBody=${funcBody}'\ttmpdata.'$p' = '$p';\n'
		elif [[ $v == 'std::string' ]]
		then
			v='const std::string&'
		fi

		funcName=${funcName}' '$v
	done
	IFS='}'

	funcName=${funcName%,}
	funcName=${funcName}')'
	funcBody=${funcBody}'\tpacker->packData(&tmpdata, '$count', lrb::NetWork::ProtoType::PT_'$mname');\n'
	count=$((count+1))

	hfuncs=${hfuncs}${funcName}';\n'

	cfuncs=${cfuncs}${funcName}'\n{\n\t'$1' tmpdata;\n'${funcBody}'}\n\n'
}

function processStruct()
{
	name=${1#*struct}
	name=${name%\{*}
	name=${name// /}

	values=${1#*\{}
	values=${values//;/,}
	genFunc $name $values
	#content=`echo $1 | sed -n 's/struct \(.*\) {/\1/p'`
	#values=`echo $1 | sed -n 's/\([^ ]*\);/\1/p'`
	

}

oldIFS=$IFS
IFS='}'

for t in $txt
do
	if [[ $t != ";" ]] 
	then
		processStruct $t
	fi

done

IFS=$oldIFS

hfuncs=${hfuncs}'\n}\n\n}\n\n#endif'
cfuncs=${cfuncs}'\n}\n\n}\n'

echo ${hfuncs} > ${outname}'.h'
echo ${cfuncs} > ${outname}'.cc'

