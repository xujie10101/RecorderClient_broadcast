#ifndef LINET_ATOM_HH
#define LINET_ATOM_HH

#include "os_define.h"

//自增
uint32_t AtomSelfAdd(void * var);

//自减
uint32_t AtomSelfDec(void * var);

//加一个值
uint32_t AtomAdd(void * var, const uint32_t value);

//减一个值
uint32_t AtomDec(void * var, uint32_t value);

//赋值,windows下返回新值，linux下返回旧值
uint32_t AtomSet(void * var, const uint32_t value);

//取值
uint32_t AtomGet(void * var);

#endif //MDK_ATOM_H
