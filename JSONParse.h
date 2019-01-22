#ifndef __JSONPARSE__H__
#define __JSONPARSE__H__
#include "cJSON.h"
#include <string>
#include <vector>
#include <assert.h>
#include <map>


class CJSONParse
{
public:
	CJSONParse(const char* pJson = NULL);
	~CJSONParse();
	bool Load(const char* pJson);
	bool Release();

	cJSON* GetRootItem() { return m_pJsonRoot; }
	cJSON* GetFirstItem(const std::string &strName);
	cJSON* GetNextSiblingItem(const cJSON* pItem);
	bool GetValueStr(const cJSON* pJson, const std::string &strName, std::string &strValue);

	void printJson(cJSON * root, std::map<std::string, std::string> &KeyValue);//�Եݹ�ķ�ʽ��ӡjson�����ڲ��ֵ��

private:
	// ��ȡ·���е��ַ���
	void split(const std::string str,const std::string strPattern, std::vector<std::string> &strResultVec);

private:
	cJSON*			m_pJsonRoot;
};

#endif