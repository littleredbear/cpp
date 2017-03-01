#!/usr/bin/env bash

path=`pwd`
dir=${path%/*}'/lrbNet/'
path=${dir}'lrbProtoBuf.h'

txt=`sed -n 's/struct \(.*\) {/\1/p' $path`

reqtxt=
reqptr='void *g_lrb_protobuf_ptrs[] = {'
acktxt=
ackptr='void *s_lrb_protobuf_ptrs[] = {'

for t in $txt
do
	tmp=$t' g_'$t';\n'
	if [[ $t == Req* ]]
	then
		reqtxt=${reqtxt}${tmp}
		reqptr=${reqptr}'&g_'$t','
	else
		acktxt=${acktxt}${tmp}
		ackptr=${ackptr}'&g_'$t','
	fi
	
done

reqptr=${reqptr}'};\n'
ackptr=${ackptr}'};\n'

output='#include "lrbProtoBuf.h"\n\n\nusing namespace lrb::ProtoBuf;\n\n#ifdef LRB_APPSERVER\n'${reqtxt}${reqptr}'#else\n'${acktxt}${ackptr}'#endif\n\n'

txt=`sed -n '/struct/,/};/{s/struct \([^{]*\) {//;p;}' $path`

confs='short g_lrb_protobuf_confs[][5] = {\n{';
tp=0
off0=0
off1=0
off2=0
off3=0
off4=0
for t in $txt
do
	if [[ $t == '};' ]] 
	then
		confs=${confs}${off0}','${off1}','${off2}','${off3}','${off4}
		confs=$confs'},\n{'
		tp=0
		off0=0
		off1=0
		off2=0
		off3=0
		off4=0
	elif [[ $t == 'std::string' ]]
	then
		if [[ $tp == 0 ]]
		then
			off0=$(($off0+24))
		else
			off0=24
		fi
		tp=0
	elif [[ $t == 'int64_t' ]]
	then
		if [[ $tp == 1 ]]
		then
			off1=$(($off1+8))
		else
			off1=8
		fi
		tp=1
	elif [[ $t == 'int' ]]
	then
		if [[ $t == 2 ]]
		then
			off2=$(($off2+4))
		else
			off2=4
		fi
		tp=2
	elif [[ $t == 'short' ]]
	then
		if [[ $t == 3 ]]
		then
			off3=$(($off3+2))
		else
			off3=2
		fi
		tp=3
	elif [[ $t == 'char' ]]
	then
		if [[ $t == 4 ]]
		then
			off4=$(($off4+1))
		else
			off4=1
		fi
		tp=4
	fi

done

confs=${confs%\{};
confs=${confs}'};'

echo ${output}${confs} > ${dir}'lrbProtoBuf.cc'
echo "proto done"
