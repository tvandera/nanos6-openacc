#include "CPU.hpp"

#include <cassert>

#include <pthread.h>
#include <unistd.h>


namespace threaded_executor_internals {

CPU::CPU(size_t systemCPUId)
	: _enabled(false), _mustExit(false), _runningThread(nullptr), _systemCPUId(systemCPUId)
{
	CPU_ZERO_S(sizeof(cpu_set_t), &_cpuMask);
	CPU_SET_S(systemCPUId, sizeof(cpu_set_t), &_cpuMask);
	
	int rc = pthread_attr_init(&_pthreadAttr);
	assert(rc == 0);
	
	rc = pthread_attr_setaffinity_np(&_pthreadAttr, sizeof(cpu_set_t), &_cpuMask);
	assert(rc == 0);
	
	rc = pthread_attr_setdetachstate(&_pthreadAttr, PTHREAD_CREATE_DETACHED);
	assert(rc == 0);
}


void CPU::bindThread(pthread_t *internalPThread)
{
	int rc = pthread_setaffinity_np(*internalPThread, CPU_ALLOC_SIZE(_systemCPUId), &_cpuMask);
	assert(rc == 0);
}


} // namespace threaded_executor_internals