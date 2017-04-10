#include "resolve.h"


void nanos_preinit()
{
	typedef void nanos_preinit_t();
	
	static nanos_preinit_t *symbol = NULL;
	if (__builtin_expect(symbol == NULL, 0)) {
		symbol = (nanos_preinit_t *) _nanos6_resolve_symbol("nanos_preinit", "essential", NULL);
	}
	
	(*symbol)();
}


void nanos_init()
{
	typedef void nanos_init_t();
	
	static nanos_init_t *symbol = NULL;
	if (__builtin_expect(symbol == NULL, 0)) {
		symbol = (nanos_init_t *) _nanos6_resolve_symbol("nanos_init", "essential", NULL);
	}
	
	(*symbol)();
}


void nanos_shutdown()
{
	typedef void nanos_shutdown_t();
	
	static nanos_shutdown_t *symbol = NULL;
	if (__builtin_expect(symbol == NULL, 0)) {
		symbol = (nanos_shutdown_t *) _nanos6_resolve_symbol("nanos_shutdown", "essential", NULL);
	}
	
	(*symbol)();
}

