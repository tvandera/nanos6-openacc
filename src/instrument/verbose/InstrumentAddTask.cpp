/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2015-2022 Barcelona Supercomputing Center (BSC)
*/

#include <atomic>
#include <cassert>

#include "InstrumentAddTask.hpp"
#include "InstrumentVerbose.hpp"
#include "tasks/Task.hpp"
#include "tasks/TaskImplementation.hpp"

#include <InstrumentInstrumentationContext.hpp>


using namespace Instrument::Verbose;


namespace Instrument {
	task_id_t enterCreateTask(
		nanos6_task_info_t *taskInfo,
		nanos6_task_invocation_info_t *taskInvokationInfo,
		__attribute__((unused)) size_t flags,
		__attribute__((unused)) bool taskRuntimeTransition,
		InstrumentationContext const &context
	) {
		static std::atomic<task_id_t::inner_type_t> _nextTaskId(0);

		task_id_t taskId = _nextTaskId++;

		if (_verboseAddTask) {
			LogEntry *logEntry = getLogEntry(context);
			assert(logEntry != nullptr);

			logEntry->appendLocation(context);

			logEntry->_contents << " --> AddTask " << taskId;
			if (taskInfo && taskInfo->implementations[0].task_type_label) {
				logEntry->_contents << " " << taskInfo->implementations[0].task_type_label;
			}
			if (taskInvokationInfo && taskInvokationInfo->invocation_source) {
				logEntry->_contents << " " << taskInvokationInfo->invocation_source;
			}

			addLogEntry(logEntry);
		}

		return taskId;
	}

	void exitCreateTask(
		__attribute__((unused)) bool taskRuntimeTransition
	) {
	}

	void createdArgsBlock(
		task_id_t taskId,
		void *argsBlockPointer,
		size_t originalArgsBlockSize,
		size_t argsBlockSize,
		InstrumentationContext const &context
	) {
		if (!_verboseAddTask) {
			return;
		}

		LogEntry *logEntry = getLogEntry(context);
		assert(logEntry != nullptr);

		logEntry->appendLocation(context);
		logEntry->_contents << " --- AddTask: created " << taskId << " argsblock:" << argsBlockPointer;
		logEntry->_contents << " " << originalArgsBlockSize << " bytes (before alignment fixup), ";
		logEntry->_contents << " " << argsBlockSize << " bytes (after alignment fixup)";

		addLogEntry(logEntry);
	}


	void createdTask(
		void *taskObject,
		task_id_t taskId,
		InstrumentationContext const &context
	) {
		if (!_verboseAddTask) {
			return;
		}

		Task *task = (Task *) taskObject;

		LogEntry *logEntry = getLogEntry(context);
		assert(logEntry != nullptr);

		logEntry->appendLocation(context);
		logEntry->_contents << " --- AddTask: created " << taskId << " object:" << task;
		if (task->getParent() != nullptr) {
			logEntry->_contents << " parent:" << task->getParent()->getInstrumentationTaskId();
		}

		addLogEntry(logEntry);
	}

	void enterSubmitTask(
		__attribute__((unused)) bool taskRuntimeTransition
	) {
	}

	void exitSubmitTask(
		task_id_t taskId,
		__attribute__((unused)) bool taskRuntimeTransition,
		InstrumentationContext const &context
	) {
		if (!_verboseAddTask) {
			return;
		}

		LogEntry *logEntry = getLogEntry(context);
		assert(logEntry != nullptr);

		logEntry->appendLocation(context);
		logEntry->_contents << " <-- AddTask " << taskId;

		addLogEntry(logEntry);
	}

	task_id_t enterInitTaskforCollaborator(
		__attribute__((unused)) task_id_t taskforId,
		__attribute__((unused)) nanos6_task_info_t *taskInfo,
		__attribute__((unused)) nanos6_task_invocation_info_t *taskInvokationInfo,
		__attribute__((unused)) size_t flags,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		// Verbose instrumentation does not instrument task fors
		task_id_t taskId = 0;

		return taskId;
	}

	void exitInitTaskforCollaborator(
		__attribute__((unused)) task_id_t taskforId,
		__attribute__((unused)) task_id_t collaboratorId,
		__attribute__((unused)) InstrumentationContext const &context
	) {
		// Verbose instrumentation does not instrument task fors
	}

	void registeredNewSpawnedTaskType(
		__attribute__((unused)) nanos6_task_info_t *taskInfo
	) {
	}

	void enterSpawnFunction(
		__attribute__((unused)) bool taskRuntimeTransition
	) {
	}

	void exitSpawnFunction(
		__attribute__((unused)) bool taskRuntimeTransition
	) {
	}
}
