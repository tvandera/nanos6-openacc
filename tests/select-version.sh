#!/bin/sh

#	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
#	
#	Copyright (C) 2015-2017 Barcelona Supercomputing Center (BSC)

if test -z ${NANOS6_SCHEDULING_POLICY} ; then
	if [[ "${*}" == *"fibonacci"* ]] || [[ "${*}" == *"task-for-nqueens"* ]]; then
		export NANOS6_SCHEDULING_POLICY=lifo
	fi
fi

if test -z ${NANOS6} ; then
	if test "${*}" = "${*/.debug/}" ; then
		export NANOS6=optimized
		exec "${@}"
	else
		export NANOS6=debug
		
		# Regular execution
		"${@}"
		
		# This is disabled since it faild due to too much memory management fragmentation
# 		# Execution with memory debugging (underflow)
# 		NANOS6_DEBUG_MEMORY=1 NANOS6_DEBUG_MEMORY_PROTECT_AFTER=0 "${@}"
# 		
# 		# Execution with memory debugging (overflow)
# 		NANOS6_DEBUG_MEMORY=1 NANOS6_DEBUG_MEMORY_PROTECT_AFTER=1 "${@}"
	fi
else
	exec "${@}"
fi

