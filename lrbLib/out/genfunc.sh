#!/usr/bin/env bash


path=$1

outname=${path%.h}
mname=${outname##*lrb}

outname=${outname}'Func'


txt=`sed -n '/struct/,/};/p' $path`

hfuncs=
cfuncs=

count=0
function genPackFunc()
{
	local funcName='void pack'$1'(lrb::NetWork::DataPacker *packer,'
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

	hfuncs=${hfuncs}${funcName}';\n'

	cfuncs=${cfuncs}${funcName}'\n{\n\t'$1' tmpdata;\n'${funcBody}'}\n\n'
}

function genBindReqFunc()
{
	if [[ $1 != Req* ]]
	then
		return
	fi

	local funcName='void bind'$1'Func(const std::function<void(lrb::NetWork::DataPacker *)> &func)'
	hfuncs=${hfuncs}${funcName}';\n'

	local funcBody='#ifdef LRB_'$mname'_SERVER\n\tg_lrb_'$mname'_reqFuncs['$((count/2))'] = func;\n#endif\n'
	cfuncs=${cfuncs}${funcName}'\n{\n'${funcBody}'}\n\n'
}

function genBindAckFunc()
{
	local funcName='void bindAckFunc(AckFuncType acktype, const std::function<void()> &func)'
	hfuncs=${hfuncs}${funcName}';\n'
	
	local funcBody='#ifndef LRB_'$mname'_SERVER\n\tif (acktype > AckFuncType::AFT_BOT && acktype < AckFuncType::AFT_TOP)\n\t\tg_lrb_'$mname'_ackFuncs[(int)acktype] = func;\n#endif\n'
	cfuncs=${cfuncs}${funcName}'\n{\n'${funcBody}'}\n\n'
}

function genOtherFunc()
{
	local execReqFunc='void execReqFunc(int protoId, lrb::NetWork::DataPacker *packer)\n{\n#ifdef LRB_'$mname'_SERVER\n\tif (protoId > 0 && protoId < '$count' && !(protoId & 1))\n\t\tg_lrb_'$mname'_reqFuncs[protoId >> 1](packer);\n#endif\n}\n\n'
	cfuncs=${cfuncs}${execReqFunc}

	local execAckFunc='void execAckFunc()\n{\n#ifndef LRB_'$mname'_SERVER\n\tint verify = g_lrb_'$mname'_AckAckFuncType.acktype;\n\tif(verify > (int)AckFuncType::AFT_BOT && verify < (int)AckFuncType::AFT_TOP)\n\t\tRunLoop::runInLoop(g_lrb_'$mname'_ackFuncs[verify], RunLoopType::RLT_LOGIC);\n#endif\n}\n\n'
	cfuncs=${cfuncs}${execAckFunc}
}

function processStruct()
{
	name=${1#*struct}
	name=${name%\{*}
	name=${name// /}

	values=${1#*\{}
	values=${values//;/,}
	genPackFunc $name $values
	genBindReqFunc $name

	count=$((count+1))
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

genBindAckFunc
genOtherFunc

hhead='#ifndef LRB_'$mname'_H\n#define LRB_'$mname'_H\n\n#include "lrb'$mname'.h"\n\nnamespace lrb {\n\nnamespace NetWork {\n\n\tclass DataPacker;\n}\n\nnamespace '$mname' {\n\n'

chead='#include "lrb'$mname'Func.h"\n#include "lrbNetWork.h"\n\nusing namespace lrb::'$mname';\n\n#ifdef LRB_'$mname'_SERVER\nstd::function<void(lrb::NetWork::DataPacker *)> g_lrb_'$mname'_reqFuncs['$((count/2))'];\n#else\nstd::function<void()> g_lrb_'$mname'_ackFuncs[(int)AckFuncType::AFT_TOP];\nextern AckVerifyData g_lrb_'$mname'_AckVerifyData;\n#endif\n\nnamespace lrb {\n\nnamespace '$mname' {\n\n'

hout=${hhead}${hfuncs}'\n}\n\n}\n\n#endif'
cout=${chead}${cfuncs}'\n}\n\n}\n'

echo ${hout} > ${outname}'.h'
echo ${cout} > ${outname}'.cc'

