/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2015-2022 Barcelona Supercomputing Center (BSC)
*/

#ifndef NANOS6_LOOP_H
#define NANOS6_LOOP_H

#include "major.h"
#include "task-instantiation.h"


#pragma GCC visibility push(default)


// NOTE: The full version depends also on nanos6_major_api
//       That is:   nanos6_major_api . nanos6_loop_api
enum nanos6_loop_api_t { nanos6_loop_api = 6 };


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t lower_bound; // Inclusive
	size_t upper_bound; // Exclusive
	size_t grainsize;
	size_t chunksize;
} nanos6_loop_bounds_t;

//! \brief Allocate space for a taskloop/taskfor/taskloopfor and its parameters
//!
//! This function creates a taskloop/taskfor/taskloopfor and allocates space for its parameters.
//! After calling it, the user code should fill out the block of data stored in args_block_pointer,
//! and call nanos6_submit_task with the contents stored in task_pointer.
//!
//! \param[in] task_info a pointer to the nanos6_task_info_t structure
//! \param[in] task_invocation_info a pointer to the nanos6_task_invocation_info_t structure
//! \param[in] task_label a string that identifies the task
//! \param[in] args_block_size size needed to store the parameters passed to the task call
//! \param[in,out] args_block_pointer a pointer to a location to store the pointer to the block of data that will contain the parameters of the task call. Input if flags contains nanos6_preallocated_args_block, out otherwise
//! \param[out] task_pointer a pointer to a location to store the task handler
//! \param[in] flags the flags of the task
//! \param[in] num_deps the expected number of dependencies of this taskfor or a taskloop child or -1 if undefined
//! \param[in] lower_bound The lower bound of the iteration space (inclusive)
//! \param[in] upper_bound The upper bound of the iteration space (exclusive)
//! \param[in] grainsize The minimum number of iterations which should be executed by a task
//! \param[in] chunksize The minimum number of iterations which should be executed by a chunk
void nanos6_create_loop(
	nanos6_task_info_t *task_info,
	nanos6_task_invocation_info_t *task_invocation_info,
	char const *task_label,
	size_t args_block_size,
	/* OUT */ void **args_block_pointer,
	/* OUT */ void **task_pointer,
	size_t flags,
	size_t num_deps,
	size_t lower_bound,
	size_t upper_bound,
	size_t grainsize,
	size_t chunksize
);

//! \brief Allocate space for a taskiter and its parameters
//!
//! This function creates a taskiter and allocates space for its parameters.
//! After calling it, the user code should fill out the block of data stored in args_block_pointer,
//! and call nanos6_submit_task with the contents stored in task_pointer.
//!
//! \param[in] task_info a pointer to the nanos6_task_info_t structure
//! \param[in] task_invocation_info a pointer to the nanos6_task_invocation_info_t structure
//! \param[in] args_block_size size needed to store the parameters passed to the task call
//! \param[in,out] args_block_pointer a pointer to a location to store the pointer to the block of data that will contain the parameters of the task call. Input if flags contains nanos6_preallocated_args_block, out otherwise
//! \param[out] task_pointer a pointer to a location to store the task handler
//! \param[in] flags the flags of the task
//! \param[in] num_deps the expected number of dependencies of this taskfor or a taskloop child or -1 if undefined
//! \param[in] lower_bound The lower bound of the iteration space (inclusive)
//! \param[in] upper_bound The upper bound of the iteration space (exclusive)
//! \param[in] unroll The number of times the created taskiter construct should be unrolled
void nanos6_create_iter(
	nanos6_task_info_t *task_info,
	nanos6_task_invocation_info_t *task_invocation_info,
	char const *task_label,
	size_t args_block_size,
	/* OUT */ void **args_block_pointer,
	/* OUT */ void **task_pointer,
	size_t flags,
	size_t num_deps,
	size_t lower_bound,
	size_t upper_bound,
	size_t unroll
);

#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop


#endif /* NANOS6_LOOP_H */
