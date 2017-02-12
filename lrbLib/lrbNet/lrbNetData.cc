#include "lrbNetData.h"
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>


using namespace lrb::NetData;


#ifdef LRB_APPSERVER

ReqVerifyData g_ReqVerifyData;

static void *s_ptrs[] = {&g_ReqVerifyData,};

static std::function<void(DataPacker *)> s_reqFuncs[1];

#else

AckVerifyData g_AckVerifyData;

static void *s_ptrs[] = {&g_AckVerifyData,};

static std::unordered_map<int, std::function<void()> > s_ackFuncs;

#endif

const static short s_confs[][5] = {
	{0,0,4,0,0,},
	{0,0,4,0,0,},
};

static DataCenter s_center;
static DataParser s_parser;

//-----------------------------------------Data Packer--------------------------------------

DataPacker::DataPacker():
m_doneVal(0),
m_curVal(0),
m_last(NULL),
m_next(NULL)
{

}

DataPacker::~DataPacker()
{

}

void DataPacker::packData(void *data, int size, bool verify)
{

}

void DataPacker::bindLaskPacker(DataPacker *packer)
{

}

void DataPacker::bindNextPacker(DataPacker *packer)
{

}

DataPacker *DataPacker::lastPacker()
{
	if (m_last == NULL)
		return this - 1;

	return m_last;
}

DataPacker *DataPacker::nextPacker()
{
	if (m_next == NULL)
		return this + 1;

	return m_next;
}

//------------------------------------------Data Center-------------------------------------

DataCenter::DataCenter():
m_size(s_defaultNum)
{
	m_head = m_packers;
	m_able = m_head;
	m_back = m_packers + m_size - 1;;
}

DataCenter::~DataCenter()
{

}

DataPacker *DataCenter::getAvailablePacker()
{
	if (m_able == m_back)
	{
		DataPacker *ptr = (DataPacker *)calloc(m_size, sizeof(DataPacker));
		if (ptr == NULL)
			return NULL;

		m_back->bindNextPacker(ptr);
		ptr->bindLaskPacker(m_back);
		m_back = ptr + m_size - 1;
		m_size = m_size << 1;
	}
	
	m_able = m_able->nextPacker();

	return m_able->lastPacker();
}

void DataCenter::reusePacker(DataPacker *packer)
{
	DataPacker *next = packer->nextPacker();
	if (next == m_able)
	{
		m_able = packer;
		return;
	}

	if (packer == m_head)
	{
		m_head = next;
	} else
	{
		DataPacker *last = packer->lastPacker();
		last->bindNextPacker(next);
		next->bindLastPacker(last);
	}

	packer->bindNextPacker(NULL);
	m_back->bindNextPacker(packer);
	packer->bindLastPacker(m_back);
	m_back = packer;

}

//------------------------------------------Data Parser---------------------------------------

DataParser::DataParser():
m_dataCache(NULL),
m_cacheLen(0),
m_frameLen(0)
{

}

DataParser::~DataParser()
{

}

void DataParser::parseNetData(char *data, int size)
{
	if (m_dataCache == NULL)
	{
		parseFirstData(data, size);
	} else 
	{
		int needLen = m_frameLen - m_cacheLen;
		if (needLen > size)
		{
			memcpy(m_dataCache + m_cacheLen, data, size);
			m_cacheLen += size;
		} else
		{
			memcpy(m_dataCache + m_cacheLen, data, needLen);
			parseNetFrame(m_dataCache, m_frameLen);
			free(m_dataCache);
			m_dataCache = NULL;
		
			parseFirstData(data + needLen, size-needLen);

		}
	}

	free(data);
}

void DataParser::parseFirstData(char *data, int size)
{
	while(size > 0) 
	{
		int frameLen;
		memcpy(&frameLen, data, sizeof(int));
		data += sizeof(int);
		size -= sizeof(int);

		int leftLen = size - sizeof(int);
		if (frameLen > leftLen)
		{
			m_dataCache = (char *)calloc(frameLen, sizeof(char));
			if (m_dataCache)
			{
				memcpy(m_dataCache, data, leftLen);
				m_frameLen = frameLen;
				m_cacheLen = leftLen;
			}
			break;
		} else
		{
			parseNetFrame(data, frameLen);
			data += frameLen;
			size -= frameLen;
		}
	}

}

void DataParser::parseNetFrame(char *frame, int len)
{
#ifdef LRB_APPSREVER


#else
	while(len > 0)
	{
		int plen;
		memcpy(&plen, frame, sizeof(int));
		frame += sizeof(int);
		len -= sizeof(int);

		unpackData(frame, plen);
		frame += plen;
		len -= plen;
	}

	auto iter = s_ackFuncs.find(g_AckVerifyData.verify);
	if (iter != s_ackFuncs.end())
		iter->second();

#endif
}


namespace lrb {

namespace NetData {

int packData(const char *src, int uuid, void **res)
{
	int len = 5;
	for (int i=1;i<5;++i)
	{
		if (s_confs[uuid][i] > 0)
			len += (s_confs[uuid][i] + 3);
	}

	int c = 0;
	while(c < s_confs[uuid][0])
	{
		std::string *ptr = (std::string *)(src + c);
		len += (ptr->size() + 1);
		c += sizeof(std::string);
	}

	char *ptr = (char *)calloc(len, sizeof(char));
	if (ptr == NULL)
		return -1;

	*res = ptr;
	*(int *)ptr = uuid;
	ptr += sizeof(int);

	int off = 0;
	short offs[5] = {-1,-1,-1,-1,-1};
	for (int i=0;i<5;++i)
	{
		if (s_confs[uuid][i] > 0)
		{
			offs[i] = off;
			off += s_confs[uuid][i];
		}
	}

	for (char i=1;i<5;++i)
	{
		short vlen = s_confs[uuid][i];
		if (vlen > 0)
		{
			*ptr = i;
			ptr += sizeof(char);

			memcpy(ptr, &vlen, sizeof(short));
			ptr += sizeof(short);

			memcpy(ptr, src + offs[i], vlen);
			ptr += vlen;
		}
	}

	ptr += sizeof(char);
	c = 0;
	while(c < s_confs[uuid][0])
	{
		std::string *sptr = (std::string *)(src + c);
		memcpy(ptr, sptr->c_str(), sptr->size());
		ptr += (sptr->size() + 1);
		
		c += sizeof(std::string);
	}

	return len;
}

int unpackData(const char *src, int size)
{
	int uuid;
	memcpy(&uuid, src, sizeof(int));
	src += sizeof(int);
	size -= sizeof(int);

	int off = 0;
	short offs[5] = {-1,-1,-1,-1,-1};
	for (int i=0;i<5;++i)
	{
		if (s_confs[uuid][i] > 0)
		{
			offs[i] = off;
			off += s_confs[uuid][i];
		}
	}

	char *dst = (char *)s_ptrs[uuid >> 1];
	memset(dst + s_confs[uuid][0], 0, off-s_confs[uuid][0]);

	while(size > 0)
	{
		char type = *(char *)src;
		src += sizeof(char);
		size -= sizeof(char);
		
		if (type == 0)
		{
			int c = 0;
			while(c < s_confs[uuid][0])
			{
				std::string *ptr = (std::string *)(dst + c);
				if (size > 0)
					*ptr = src;
				else
					ptr->clear();

				src += (ptr->size() + 1);
				size -= (ptr->size() + 1);

				c += sizeof(std::string);
			}

			break;
		} else 
		{
			short len;
			memcpy(&len, src, sizeof(short));
			src += sizeof(short);
			size -= sizeof(short);
			
			if (offs[type] != -1)
				memcpy(dst + offs[type], src, std::min(len, s_confs[uuid][type]));
			
			src += len;
			size -= len;
		}
	}
	
	return uuid;
}


#ifdef LRB_APPSERVER
void bindReqFunc(int uuid, const std::function<void(int, int)> &func)
{
	s_reqFuncs[uuid >> 1] = func;
}

#else
void bindAckFunc(int verify, const std::function<void()> &func)
{
	s_ackFuncs[verify] = func;
}
#endif

void parseNetData(char *data, int size)
{
	s_parser.parseNetData(data, size);
}


}

}





