#ifndef _LRB_THREAD_H
#define _LRB_THREAd_H

	
typedef _lrbThreadFuncBlock {
	void (*m_func)();
	uint8_t m_state; //0 待处理，1.处理完成
} lrbTFB;


typedef _lrbThreadControlBlock {
	lrbTFB *m_write;
	uint32_t m_size[3];
	
}lrbTCB;
	


void *lrb_threadFunc(void *arg);
void lrb_createTCB();
void lrb_addCallback();



#endif
