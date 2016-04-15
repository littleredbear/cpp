#ifndef LRB_BASE_TEMPLATE_H
#define LRB_BASE_TEMPLATE_H

#include "base/Mutex.h"

namespace lrb {

	namespace base {

		template<typename T>
			class Singleton {
				public:
					static T* getInstance() {
						if (_instance == NULL) {
							MutexLockGuard lock(_mutex);
							if (_instance == NULL) {
								_instance = new T();
							}
						}
						return _instance;
					};
				protected:
					Singleton(){};
					~Singleton(){};
				private:
					Singleton(const Singleton &);
					const Singleton& operator = (const Singleton &);
					static T* _instance;
					static MutexLock _mutex;
			};
		template<typename T>
			T *Singleton<T>::_instance = NULL;
		template<typename T>
			MutexLock Singleton<T>::_mutex;	

	}
}

#endif
