#include <iostream>

#include <assert.h>
#include <dlfcn.h>
#include <signal.h>

#include "LeaderThread.hpp"

#include "api/nanos6_rt_interface.h"
#include "executors/threads/ThreadManager.hpp"
#include "executors/threads/ThreadManagerPolicy.hpp"
#include "lowlevel/EnvironmentVariable.hpp"
#include "scheduling/Scheduler.hpp"

#include <InstrumentInitAndShutdown.hpp>


static std::atomic<int> shutdownDueToSignalNumber(0);


static void signalHandler(int signum)
{
	// By default set up the termination flag
	shutdownDueToSignalNumber.store(signum);
	nanos_notify_ready_for_shutdown();
}


static void programSignal(int signum) {
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESETHAND;
	
	int rc = sigaction(signum, &sa, nullptr);
	FatalErrorHandler::handle(rc, "Programming signal handler for signal number ", signum);
}


void nanos_preinit(void) {
	Scheduler::initialize();
	ThreadManagerPolicy::initialize();
	Instrument::initialize();
}

void nanos_init(void) {
	ThreadManager::initialize();
	
	EnvironmentVariable<bool> handleSigInt("NANOS_HANDLE_SIGINT", 0);
	if (handleSigInt) {
		programSignal(SIGINT);
	}
	EnvironmentVariable<bool> handleSigTerm("NANOS_HANDLE_SIGTERM", 0);
	if (handleSigTerm) {
		programSignal(SIGTERM);
	}
	EnvironmentVariable<bool> handleSigQuit("NANOS_HANDLE_SIGQUIT", 0);
	if (handleSigQuit) {
		programSignal(SIGQUIT);
	}
}

void nanos_wait_until_shutdown(void) {
	LeaderThread::maintenanceLoop();
	Instrument::shutdown();
	
	if (shutdownDueToSignalNumber.load() != 0) {
		raise(shutdownDueToSignalNumber.load());
	}
	
	ThreadManager::shutdown();
}

void nanos_notify_ready_for_shutdown(void) {
	LeaderThread::notifyMainExit();
}

