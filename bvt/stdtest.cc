// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// \file stdtest.cc
// \brief Standard BVT harness.
//

#define _GNU_SOURCE 1
#include <signal.h>
#include "stdtest.h"

/// Called when a signal is received.
static void OnSignal (int sig, siginfo*, void*)
{
    cout.flush();
    cerr << "Signal " << sig;
    #if HAVE_STRSIGNAL
	cerr << " (" << strsignal(sig) << ')';
    #endif
    cerr << " received. Aborting." << endl;
    exit (sig);
}

/// Called by the framework on unrecoverable exception handling errors.
static void Terminate (void)
{
    assert (!"Unrecoverable exception handling error detected.");
    raise (SIGABRT);
    exit (EXIT_FAILURE);
}

/// Called when an exception violates a throw specification.
static void OnUnexpected (void)
{
    cerr << "Unexpected exception caught. This is an internal error. Aborting." << endl;
    Terminate();
}

/// Installs OnSignal as handler for signals.
static void InstallCleanupHandlers (void)
{
    static const int c_Signals[] = {
	SIGINT, SIGQUIT, SIGILL,  SIGTRAP, SIGABRT,
	SIGIOT, SIGBUS,  SIGFPE,  SIGSEGV, SIGTERM,
	SIGIO,  SIGPWR,  SIGCHLD
    };
    struct sigaction sa;
    sigemptyset (&sa.sa_mask);
    sa.sa_sigaction = OnSignal;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    for (size_t i = 0; i < VectorSize(c_Signals); ++ i)
	sigaction (c_Signals[i], &sa, NULL);
    std::set_terminate (Terminate);
    std::set_unexpected (OnUnexpected);
}

int StdTestHarness (stdtestfunc_t testFunction)
{
    InstallCleanupHandlers();
    int rv = EXIT_FAILURE;
    try {
	(*testFunction)();
	rv = EXIT_SUCCESS;
    } catch (exception& e) {
	cout.flush();
	cerr << "Error: " << e << endl;
    } catch (...) {
	cout.flush();
	cerr << "Unexpected error." << endl;
    }
    return (rv);
}

