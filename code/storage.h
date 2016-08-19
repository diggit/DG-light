#ifndef _STORAGE_LIB
	#define _STORAGE_LIB

	#include <inttypes.h>

	#define E8(A)	((uint8_t*)A)
	#define E16(A)	((uint16_t*)A)

	void storage_all_store(void);
	void storage_all_load(void);
	void storage_config_store(void);


#endif
