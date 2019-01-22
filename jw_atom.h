#ifndef LINET_ATOM_HH
#define LINET_ATOM_HH

#include "os_define.h"

//����
uint32_t AtomSelfAdd(void * var);

//�Լ�
uint32_t AtomSelfDec(void * var);

//��һ��ֵ
uint32_t AtomAdd(void * var, const uint32_t value);

//��һ��ֵ
uint32_t AtomDec(void * var, uint32_t value);

//��ֵ,windows�·�����ֵ��linux�·��ؾ�ֵ
uint32_t AtomSet(void * var, const uint32_t value);

//ȡֵ
uint32_t AtomGet(void * var);

#endif //MDK_ATOM_H
