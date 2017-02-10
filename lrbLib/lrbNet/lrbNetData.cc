#include "lrbNetData.h"
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>


namespace {

#ifdef LRB_APPSERVER

ReqVerifyData g_ReqVerifyData;

static void *s_ptrs[] = {&g_ReqVerifyData,};

static std::function<void(int, int)> s_reqFuncs[1];

#else

AckVerifyData g_AckVerifyData;

static void *s_ptrs[] = {&g_AckVerifyData,};

static std::unordered_map<int, std::function<void()> > s_ackFuncs;

#endif


const static short s_confs[][5] = {
	{0,0,4,0,0,},
	{0,0,4,0,0,},
};

static DataParser s_parser;

}


using namespace lrb::NetData;

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
	char *ptr = data;
	if (m_dataCache == NULL)
	{
		int frameLen;
		int total = size;
		do {
			memcpy(&frameLen, ptr, sizeof(int));
			ptr += sizeof(int);
			total -= sizeof(int);

			int leftLen = total - sizeof(int);
			if (frameLen > leftLen)
			{
				m_dataCache = (char *)calloc(frameLen, sizeof(char));
				if (m_dataCache)
				{
					memcpy(m_dataCache, ptr, leftLen);
					m_frameLen = frameLen;
					m_cacheLen = leftLen;
				}
				break;
			} else
			{
				parseNetFrame(ptr, frameLen);
				ptr += frameLen;
				total -= frameLen;
			}

		}while(1);
	} else 
	{
		if (m_frameLen > size + m_cacheLen)
		{
			memcpy(m_dataCache, ptr, size);
			m_cacheLen += size;
		} else
		{

		}
	}

	free(data);
}

void DataParser::parseNetFrame(char *frame, int len)
{

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

	char *dst = s_ptrs[uuid >> 1];
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





