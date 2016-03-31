#ifndef LRB_H
#define LRB_H



namespace lrb {

	class noncopyable {
		protected:
			noncopyable(){};
			~noncopyable(){};
		private:
			noncopyable(const noncopyable &);
			const noncopyable & operator = (const noncopyable &);

	};


}


#endif
