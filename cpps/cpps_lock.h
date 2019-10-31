#ifndef CPPS_LOCK_CPPS_HEAD_
#define CPPS_LOCK_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2018/09/26 (yy/mm/dd)
//@Module		:	CPPS_LOCK
//@Description	:	�̻߳�������
//@website		:	http://cpps.wiki
//==================================

#ifndef WIN32

#include <netinet/in.h>
#include <pthread.h>

#endif

namespace cpps
{

	class  cpps_lock
	{
#ifdef WIN32
		//��������
	private:
		CRITICAL_SECTION					thread_lock;					//�ٽ����

																			//��������
	public:
		//���캯��
		inline cpps_lock() { ::InitializeCriticalSection(&thread_lock); }
		//��������
		inline ~cpps_lock() { ::DeleteCriticalSection(&thread_lock); }

		//���ܺ���
	public:
		//��������
		virtual inline void lock() { ::EnterCriticalSection(&thread_lock); }
		//�������� 
		virtual inline void unlock() { ::LeaveCriticalSection(&thread_lock); }
#else
		//��������
	private:
		pthread_mutex_t						thread_lock;					//�ٽ����

																			//��������
	public:
		//���캯��
		inline cpps_lock() { pthread_mutex_init(&thread_lock,NULL); }
		//��������
		inline ~cpps_lock() { ::pthread_mutex_destroy(&thread_lock); }

		//���ܺ���
	public:
		//��������
		virtual inline void lock() { ::pthread_mutex_lock(&thread_lock); }
		//�������� 
		virtual inline void unlock() { ::pthread_mutex_unlock(&thread_lock); }
#endif
	};

	struct C;
	void	cpps_reglock(C *c);

}

#endif // CPPS_LOCK_CPPS_HEAD_