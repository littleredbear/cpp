#include "log/LogFile.h"
#include <time.h>
#include <assert.h>

using namespace lrb;

LogFile::LogFile(const char *basename):
	m_basename(basename),
	m_fp(NULL)
{
	this->rollFile();
}

LogFile::~LogFile()
{
	if (m_fp != NULL)
		fclose(m_fp);
}

void LogFile::append(const char *buff, int len)
{
	size_t remain = len;
	size_t n = 0;
	while (remain > 0) {
		size_t x = fwrite(buff + n, len, 1, m_fp);
		if (n == 0) {
			if (ferror(m_fp)) {
				fprintf(stderr, "LogFile append failed!");
			}
			break;	
		}
		n += x;
		remain = len - n;
	}	
	
	m_logSize += len;
	time_t now = time(NULL);
	if (m_logSize > krollSize || now > m_logTime + krollTime)
		this->rollFile();	
}

void LogFile::flush()
{
	fflush(m_fp);
}

std::string LogFile::LogFileName()
{
	std::string filename = m_basename;
	char timebuf[32];
	struct tm tm;
	localtime_r(&m_logTime, &tm);
	strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S", &tm);
	filename += timebuf;
	filename += ".log";
	return std::move(filename);
}

void LogFile::rollFile()
{
	if (m_fp != NULL) {
		fclose(m_fp);	
		m_fp = NULL;
	}
	m_logTime = time(NULL);
	m_fp = fopen(LogFileName().c_str(), "a+");
	assert(m_fp != NULL);
	m_logSize = 0;
}



