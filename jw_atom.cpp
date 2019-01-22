#include "jw_atom.h"

//自增
uint32_t AtomSelfAdd(void * var) 
{
#ifdef WIN32
	return InterlockedIncrement((long *)(var)); // NOLINT
#else
	return __sync_add_and_fetch((uint32_t *)(var), 1); // NOLINT
#endif
}

//自减
uint32_t AtomSelfDec(void * var) 
{
#ifdef WIN32
	return InterlockedDecrement((long *)(var)); // NOLINT
#else
	return __sync_add_and_fetch((uint32_t *)(var), -1); // NOLINT
#endif
}

//加一个值
uint32_t AtomAdd(void * var, const uint32_t value) 
{
#ifdef WIN32
	return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
	return __sync_fetch_and_add((uint32_t *)(var), value);  // NOLINT
#endif
}

//减一个值
uint32_t AtomDec(void * var, uint32_t value) 
{
	value = value * -1;
#ifdef WIN32
	return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
	return __sync_fetch_and_add((uint32_t *)(var), value);  // NOLINT
#endif
}

//赋值,windows下返回新值，linux下返回旧值
uint32_t AtomSet(void * var, const uint32_t value) 
{
#ifdef WIN32
	::InterlockedExchange((long *)(var), (long)(value)); // NOLINT
#else
	__sync_lock_test_and_set((uint32_t *)(var), value);  // NOLINT
#endif
	return value;
}

//取值
uint32_t AtomGet(void * var) 
{
#ifdef WIN32
	return InterlockedExchangeAdd((long *)(var), 0); // NOLINT
#else
	return __sync_fetch_and_add((uint32_t *)(var), 0);  // NOLINT
#endif
}
