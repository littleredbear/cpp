#include "LogBuffer.h"

using namespace lrb;
using namespace lrb::base;
using namespace lrb::log;

LogBuffer::LogBuffer(const char *basename):
	m_file(basename),
	m_running(false),
	m_mutex(),
	m_cond(m_mutex),
	m_thread(std::bind(&LogBuffer::ThreadFunc, this)),
	m_buffer(new SBuffer())
{

}

void LogBuffer::append(const char *buff, unsigned int len)
{
	MutexLockGuard lock(m_mutex);
	if (m_buffer->append(buff, len) != 0) {
		m_list.push_back(m_buffer);	
		m_buffer.reset(new SBuffer());
		m_buffer->append(buff, len);
		m_cond.notify();
	}	
}

void LogBuffer::flush()
{
	MutexLockGuard lock(m_mutex);
	m_list.push_back(m_buffer);
	m_buffer.reset(new SBuffer());
	m_cond.notify();
}

void LogBuffer::ThreadFunc() 
{
	while(m_running) {
		std::shared_ptr<SBuffer> ptr;
		{
			MutexLockGuard lock(m_mutex);
			while(m_list.empty() && m_buffer->length() == 0) {
				m_cond.waitForSeconds(kFlushSeconds);
			}	
			if (!m_list.empty()) {
				ptr = m_list.front();
				m_list.pop_front();	
			} else if (m_buffer->length() > 0) {
				ptr = m_buffer;
				m_buffer.reset(new SBuffer());
			}
		}
		if ((bool)ptr)
			m_file.append(ptr->c_str(), ptr->length());
	}
}

