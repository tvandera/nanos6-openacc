#ifndef CPU_MANAGER_HPP
#define CPU_MANAGER_HPP


#include <mutex>
#include <atomic>

#include <boost/dynamic_bitset.hpp>

#include "hardware/places/ComputePlace.hpp"
#include "lowlevel/SpinLock.hpp"
#include "lowlevel/FatalErrorHandler.hpp"
#include "hardware/HardwareInfo.hpp"

#include "CPU.hpp"


class CPUManager {
private:
	//! \brief CPU mask of the process
	static cpu_set_t _processCPUMask;
	
	//! \brief per-CPU data indexed by system CPU identifier
	static std::vector<CPU *> _cpus;
	
	//! \brief numer of initialized CPUs
	static size_t _totalCPUs;
	
	//! \brief indicates if the thread manager has finished initializing the CPUs
	static std::atomic<bool> _finishedCPUInitialization;
	
	//! \brief threads blocked due to idleness
	static boost::dynamic_bitset<> _idleCPUs;
	
	static SpinLock _idleCPUsLock;
	
	
public:
	static void preinitialize();
	
	static void initialize();
	
	//! \brief get the CPU object assigned to a given numerical system CPU identifier
	static inline CPU *getCPU(size_t systemCPUId);
	
	//! \brief get the maximum number of CPUs that will be used
	static inline long getTotalCPUs();
	
	//! \brief check if initialization has finished
	static inline bool hasFinishedInitialization();
	
	//! \brief get a reference to the CPU mask of the process
	static inline cpu_set_t const &getProcessCPUMaskReference();
	
	//! \brief get a reference to the list of CPUs
	static inline std::vector<CPU *> const &getCPUListReference();

	//! \brief mark a CPU as idle
	static inline void cpuBecomesIdle(CPU *cpu);

	//! \brief get an idle CPU
	static inline CPU *getIdleCPU();

	//! \brief get an idle CPU given an ideal cache
	static inline CPU *getCacheLocalityIdleCPU(int cache);

	//! \brief get an idle CPU given a NUMA node
	static inline CPU *getNUMALocalityIdleCPU(size_t NUMANodeId);
};


inline CPU *CPUManager::getCPU(size_t systemCPUId)
{
	assert(systemCPUId < _cpus.size());
	return _cpus[systemCPUId];
}

inline long CPUManager::getTotalCPUs()
{
	return _totalCPUs;
}

inline bool CPUManager::hasFinishedInitialization()
{
	return _finishedCPUInitialization;
}


inline cpu_set_t const &CPUManager::getProcessCPUMaskReference()
{
	return _processCPUMask;
}

inline std::vector<CPU *> const &CPUManager::getCPUListReference()
{
	return _cpus;
}


inline void CPUManager::cpuBecomesIdle(CPU *cpu)
{
	std::lock_guard<SpinLock> guard(_idleCPUsLock);
#ifndef NDEBUG
	assert(_idleCPUs[cpu->_systemCPUId] == false);
#endif
	_idleCPUs[cpu->_systemCPUId] = true;
}


inline CPU *CPUManager::getIdleCPU()
{
	std::lock_guard<SpinLock> guard(_idleCPUsLock);
	boost::dynamic_bitset<>::size_type idleCPU = _idleCPUs.find_first();
	if (idleCPU != boost::dynamic_bitset<>::npos) {
		_idleCPUs[idleCPU] = false;
		return _cpus[idleCPU];
	} else {
		return nullptr;
	}
}


inline CPU *CPUManager::getCacheLocalityIdleCPU(int cache)
{
	std::lock_guard<SpinLock> guard(_idleCPUsLock);
	boost::dynamic_bitset<>::size_type idleCPU = _idleCPUs.find_first();
	
	//! Iterate over all the idle CPUs until finding one associated with the given cache.
	while (idleCPU != boost::dynamic_bitset<>::npos) {
		if(_cpus[idleCPU]->getMemoryPlace(cache) != nullptr) {
			//! Idle CPU with access to the best cache found
			_idleCPUs[idleCPU] = false;
			return _cpus[idleCPU];
		}
		
		idleCPU = _idleCPUs.find_next(idleCPU);
	}
	
	return nullptr;
}

inline CPU *CPUManager::getNUMALocalityIdleCPU(size_t NUMANodeId)
{
	std::lock_guard<SpinLock> guard(_idleCPUsLock);
	boost::dynamic_bitset<>::size_type idleCPU = _idleCPUs.find_first();
	
	//! Iterate over all the idle CPUs until finding one from the given NUMA node.
	while (idleCPU != boost::dynamic_bitset<>::npos) {
		if(_cpus[idleCPU]->_NUMANodeId == NUMANodeId) {
			//! Idle CPU from the required NUMA node  found
			_idleCPUs[idleCPU] = false;
			return _cpus[idleCPU];
		}
		
		idleCPU = _idleCPUs.find_next(idleCPU);
	}
	
	return nullptr;
}

#endif // THREAD_MANAGER_HPP
