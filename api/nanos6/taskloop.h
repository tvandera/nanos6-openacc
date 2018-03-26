/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
	
	Copyright (C) 2015-2017 Barcelona Supercomputing Center (BSC)
*/

#ifndef NANOS6_TASKLOOP_H
#define NANOS6_TASKLOOP_H

#pragma GCC visibility push(default)

enum nanos6_taskloop_api_t { nanos6_taskloop_api = 1 };


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t lower_bound; // Inclusive
	size_t upper_bound; // Exclusive
	size_t step;
	size_t chunksize;
} nanos6_taskloop_bounds_t;


//! \brief Register the bounds of a taskloop
//! 
//! This function registers and initializes the loop bounds of a taskloop. This should be called
//! after the creation of the task (see nanos_create_task) and before the submission of the task
//! (see nanos_submit_task).
//! 
//! \param[in] task The task handler
//! \param[in] lower_bound The lower bound of the iteration space (inclusive)
//! \param[in] upper_bound The upper bound of the iteration space (exclusive)
//! \param[in] step The step between iterations
//! \param[in] chunksize The minimum number of iterations which should be executed by a task
void nanos_register_taskloop_bounds(
	void *task,
	size_t lower_bound,
	size_t upper_bound,
	size_t step,
	size_t chunksize
);


#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop


#endif /* NANOS6_TASKLOOP_H */