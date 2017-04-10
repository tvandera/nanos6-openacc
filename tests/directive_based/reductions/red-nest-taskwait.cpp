/*
 * Test whether a single nested reduction handles intermediate taskwait clauses properly
 *
 */

#include <atomic>

#include <assert.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#include <nanos6/debug.h>

#include "TestAnyProtocolProducer.hpp"


#define NUM_TASKS_PER_CPU 500
#define BRANCHING_FACTOR 3
#define SUSTAIN_MICROSECONDS 1000000L


TestAnyProtocolProducer tap;
static std::atomic_int numTasks(0);
static int totalTasks;
static int finalDepth;
static int branchingFactor;
static double delayMultiplier = 1.0;

void recursion(int& x, int depth) {
	if (depth < finalDepth) {
		#pragma omp task reduction(+: x)
		{
			int id = ++numTasks; // Id from next reduction task
			x++;
			
			for (int i = 0; i < branchingFactor; ++i) {
				recursion(x, depth + 1);
			}
			
			#pragma omp taskwait
			
			tap.emitDiagnostic("Task ", id, "/", totalTasks,
				" (REDUCTION) is executed");
		}
	}
}

int main() {
	long activeCPUs = nanos_get_num_cpus();
	delayMultiplier = sqrt(activeCPUs);
	
	totalTasks = NUM_TASKS_PER_CPU*activeCPUs; // Maximum, it gets rounded to closest complete level
	branchingFactor = BRANCHING_FACTOR;
	
	assert(totalTasks > 1);
	assert(branchingFactor > 1);
	
	// Compute depth required to instantiate at max 'totalTasks' (lower bound)
	finalDepth = log(totalTasks*(branchingFactor - 1) + 1)/log(branchingFactor);
	// Compute real aggregate total number of tasks (for any branching factor >= 2)
	totalTasks = (pow(branchingFactor, finalDepth) - 1)/(branchingFactor - 1);
	
	int x = 0;
	
	tap.registerNewTests(1);
	tap.begin();
	
	recursion(x, 0);
	
	#pragma omp taskwait
	
	std::ostringstream oss;
	oss << "Expected reduction computation when all " << totalTasks << " tasks are executed, taks nesting depth = " << finalDepth;
	tap.evaluate(x == totalTasks, oss.str());
	
	tap.end();
}