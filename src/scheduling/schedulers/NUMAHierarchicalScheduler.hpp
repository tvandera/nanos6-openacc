#ifndef NUMA_HIERARCHICAL_SCHEDULER_HPP
#define NUMA_HIERARCHICAL_SCHEDULER_HPP

#include <boost/dynamic_bitset.hpp>

#include "hardware/HardwareInfo.hpp"

#include "../SchedulerInterface.hpp"


class Task;


class NUMAHierarchicalScheduler: public SchedulerInterface {
	std::vector<SchedulerInterface *> _NUMANodeScheduler;
	
	// Use atomics to avoid using a lock
	// Be careful, as std::atomic does not have a copy operation, many vector
	// operations are not usable
	std::vector<std::atomic<int>> _readyTasks;

	std::vector<std::atomic<int>> _enabledCPUs;

public:
	NUMAHierarchicalScheduler();
	~NUMAHierarchicalScheduler();

	ComputePlace *addReadyTask(Task *task, ComputePlace *hardwarePlace, ReadyTaskHint hint, bool doGetIdle = true);
	
	void taskGetsUnblocked(Task *unblockedTask, ComputePlace *hardwarePlace);
	
	Task *getReadyTask(ComputePlace *hardwarePlace, Task *currentTask = nullptr, bool canMarkAsIdle = true);
	
	ComputePlace *getIdleComputePlace(bool force=false);
	
	void disableComputePlace(ComputePlace *hardwarePlace);
	
	void enableComputePlace(ComputePlace *hardwarePlace);
	
	bool requestPolling(ComputePlace *computePlace, polling_slot_t *pollingSlot);
	
	bool releasePolling(ComputePlace *computePlace, polling_slot_t *pollingSlot);

	static inline bool canBeCollapsed()
	{
		return (HardwareInfo::getMemoryNodeCount() == 1);
	}
};


#endif // NUMA_HIERARCHICAL_SCHEDULER_HPP
