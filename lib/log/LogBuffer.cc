#include "log/LogBuffer.h"

using namespace lrb;
using namespace lrb::log;

LogBuffer::LogBuffer():
	m_file("lrb"),
	m_running(false),
	m_mutex(),
	m_cond(m_mutex),
	m_thread(std::bind(&LogBuffer::ThreadFunc, this)),
	m_buffer(new SBuffer())
{

}

void LogBuffer::append(const char *buff)
{
	MutexLockGuard lock(m_mutex);
	if (m_buffer->append(buff) != 0) {
		m_list.push_back(m_buffer);	
		m_buffer.reset(new SBuffer());
		m_buffer->append(buff);
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
			while(m_list.empty()) {
				m_cond.waitForSeconds(kFlushSeconds);
			}	
			ptr = m_list.front();
			m_list.pop_front();	
		}
		m_file.append(ptr->c_str(), ptr->length());
	}
}

