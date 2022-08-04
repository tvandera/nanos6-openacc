/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2020-2022 Barcelona Supercomputing Center (BSC)
*/

#ifndef INSTRUMENT_OVNI_BLOCKING_API_HPP
#define INSTRUMENT_OVNI_BLOCKING_API_HPP


#include "instrument/api/InstrumentBlockingAPI.hpp"
#include "OvniTrace.hpp"

namespace Instrument {
	inline void enterBlockCurrentTask(
		task_id_t taskId,
		__attribute__((unused)) bool taskRuntimeTransition,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		Ovni::taskBlock(taskId._taskId, Ovni::TaskBlockingReason::BLOCKING_API);
	}

	inline void exitBlockCurrentTask(
		__attribute__((unused)) task_id_t taskId,
		__attribute__((unused)) bool taskRuntimeTransition,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		Ovni::taskUnblock(taskId._taskId, Ovni::TaskBlockingReason::BLOCKING_API);
	}

	inline void enterUnblockTask(
		task_id_t taskId,
		__attribute__((unused)) bool taskRuntimeTransition,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		Ovni::unblockEnter(taskId._taskId);
	}

	inline void exitUnblockTask(
		task_id_t taskId,
		__attribute__((unused)) bool taskRuntimeTransition,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		Ovni::unblockExit(taskId._taskId);
	}

	inline void enterWaitFor(
		task_id_t taskId,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		Ovni::taskBlock(taskId._taskId, Ovni::TaskBlockingReason::WAITFOR);
	}

	inline void exitWaitFor(
		task_id_t taskId,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		Ovni::taskUnblock(taskId._taskId, Ovni::TaskBlockingReason::WAITFOR);
	}
}


#endif // INSTRUMENT_OVNI_BLOCKING_HPP
