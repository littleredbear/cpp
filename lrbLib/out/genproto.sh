#!/usr/bin/env bash

path=`pwd`
dir=${path%/*}'/lrbProto/'

protos=`ls $dir`

for proto in $protos
do
	if [[ $ptoro == *.cc || $proto == 'lrbProtocol.h' ]]
	then
		continue
	fi

	path=${dir}${proto}

mname=${proto%.h}
mname=${mname#lrb}

txt=`sed -n 's/struct \(.*\) {/\1/p' $path`

reqtxt=
reqptr='void *g_lrb_'$mname'_ptrs[] = {\n'
acktxt=
ackptr='void *g_lrb_'$mname'_ptrs[] = {\n'
pre='g_lrb_'$mname'_'

protonum=0
for t in $txt
do
	tmp=$t' '${pre}${t}';\n'
	if [[ $t == Req* ]]
	then
		reqtxt=${reqtxt}${tmp}
		reqptr=${reqptr}'&'${pre}$t',\n'
	else
		acktxt=${acktxt}${tmp}
		ackptr=${ackptr}'&'${pre}$t',\n'
	fi
	protonum=$(($protonum+1))
	
done

reqptr=${reqptr}'};\n'
ackptr=${ackptr}'};\n'

output='#include "'$proto'"\n\n\n\n#ifdef LRB_'$mname'_SERVER\n'${reqtxt}${reqptr}'#else\n'${acktxt}${ackptr}'#endif\n\n'

txt=`sed -n '/struct/,/};/{s/struct \([^{]*\) {//;p;}' $path`

confs='short g_lrb_'$mname'_confs[][5] = {\n{';
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
		confs=${confs}${off0}'*sizeof(std::string),'${off1}','${off2}','${off3}','${off4}
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
			off0=$(($off0+1))
		else
			off0=1
		fi
		tp=0
	elif [[ $t == *int64_t || $t == 'double' ]]
	then
		if [[ $tp == 1 ]]
		then
			off1=$(($off1+8))
		else
			off1=8
		fi
		tp=1
	elif [[ $t == *int32_t || $t == 'float' ]]
	then
		if [[ $t == 2 ]]
		then
			off2=$(($off2+4))
		else
			off2=4
		fi
		tp=2
	elif [[ $t == *int16_t ]]
	then
		if [[ $t == 3 ]]
		then
			off3=$(($off3+2))
		else
			off3=2
		fi
		tp=3
	elif [[ $t == *int8_t ]]
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
confs=${confs}'};\n\n'
confs=${confs}'int g_lrb_'$mname'_protoNum = '$protonum';\n\n'


echo ${output}${confs} > ${dir}${proto/.h/.cc}

done

echo "proto done"
