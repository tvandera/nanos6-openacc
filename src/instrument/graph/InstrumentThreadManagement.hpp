/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2015-2022 Barcelona Supercomputing Center (BSC)
*/

#ifndef INSTRUMENT_GRAPH_THREAD_MANAGEMENT_HPP
#define INSTRUMENT_GRAPH_THREAD_MANAGEMENT_HPP


#include "instrument/api/InstrumentThreadManagement.hpp"
#include "instrument/generic_ids/GenericIds.hpp"

#include <InstrumentComputePlaceId.hpp>
#include <InstrumentThreadId.hpp>


namespace Instrument {
	inline void enterThreadCreation(/* OUT */ thread_id_t &threadId, __attribute__((unused)) compute_place_id_t const &computePlaceId)
	{
		threadId = GenericIds::getNewThreadId();
	}

	inline void exitThreadCreation(__attribute__((unused)) thread_id_t threadId)
	{
	}

	inline void createdThread(__attribute__((unused)) thread_id_t threadId, __attribute__((unused)) compute_place_id_t const &computePlaceId)
	{
	}

	inline void precreatedExternalThread(/* OUT */ external_thread_id_t &threadId)
	{
		threadId = GenericIds::getCommonPoolNewExternalThreadId();
	}

	template<typename... TS>
	void createdExternalThread(__attribute__((unused)) external_thread_id_t &threadId, __attribute__((unused)) TS... nameComponents)
	{
	}

	inline void threadSynchronizationCompleted(
		__attribute((unused)) thread_id_t threadId
	) {
	}

	inline void threadWillSuspend(
		__attribute__((unused)) thread_id_t threadId,
		__attribute__((unused)) compute_place_id_t cpu,
		__attribute__((unused)) bool afterSynchronization
	) {
	}

	inline void threadSuspending(__attribute__((unused)) thread_id_t threadId)
	{
	}

	inline void threadBindRemote(__attribute__((unused)) thread_id_t threadId, __attribute__((unused)) compute_place_id_t cpu)
	{
	}

	inline void threadHasResumed(
		__attribute__((unused)) thread_id_t threadId,
		__attribute__((unused)) compute_place_id_t cpu,
		__attribute__((unused)) bool afterSynchronization
	) {
	}

	inline void threadWillSuspend(__attribute__((unused)) external_thread_id_t threadId)
	{
	}

	inline void threadHasResumed(__attribute__((unused)) external_thread_id_t threadId)
	{
	}

	inline void threadWillShutdown()
	{
	}

	inline void threadWillShutdown(__attribute__((unused)) external_thread_id_t threadId)
	{
	}
}


#endif // INSTRUMENT_GRAPH_THREAD_MANAGEMENT_HPP
