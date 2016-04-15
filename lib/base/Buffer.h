#ifndef LRB_BASE_BUFFER_H
#define LRB_BASE_BUFFER_H


namespace lrb {

	namespace base {

		template<int SIZE>
			class Buffer : public noncopyable {
				public:
					Buffer():m_cur(m_data){};
					int avail() {return static_cast<int>(end()-m_cur);};
					int length() {return static_cast<int>(m_cur-m_data);};
					const char *c_str() {return m_data;};
					int append(const char * buff, unsigned int len) {
						if (len < avail()) {
							memcpy(m_cur, buff, len);
							m_cur += len; 
							return 0;
						}
						return -1;
					};

				private:
					const char *end() const {return m_data+SIZE;};
					char m_data[SIZE];
					char *m_cur;
			};
	}
}


#endif
