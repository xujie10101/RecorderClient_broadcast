#include "JSONParse.h"

using namespace std;

CJSONParse::CJSONParse(const char* pJson/* = NULL*/)
{
	if (!pJson)
	{
		m_pJsonRoot = NULL;
	}
	else
	{
		m_pJsonRoot = cJSON_Parse(pJson);
	}
}

CJSONParse::~CJSONParse()
{
	if (m_pJsonRoot)
	{
		cJSON_Delete(m_pJsonRoot);
		m_pJsonRoot = NULL;
	}
}

bool CJSONParse::Load(const char* pJson)
{
	Release();
	if (!pJson)
	{
		m_pJsonRoot = NULL;
	}
	else
	{
		m_pJsonRoot = cJSON_Parse(pJson);
	}

	return true;
}

bool CJSONParse::Release()
{
	if (m_pJsonRoot)
	{
		cJSON_Delete(m_pJsonRoot);
		m_pJsonRoot = NULL;
	}
	return true;
}

cJSON* CJSONParse::GetFirstItem(const std::string &strName)
{
	if (!m_pJsonRoot)
		return NULL;

	cJSON* pJson = m_pJsonRoot;
	vector<string> strResultVec;
	split(strName, "/", strResultVec);
	for(vector<string>::iterator strIter=strResultVec.begin(); strIter!=strResultVec.end(); ++strIter)
	{
		// 过滤路径中的根节点：pXMLElement->Name() != strIter.
		if (pJson != 0 && !strIter->empty())
		{
			pJson = cJSON_GetObjectItem(pJson, strIter->c_str());
		}
	}

	return pJson;
}

cJSON* CJSONParse::GetNextSiblingItem(const cJSON* pItem)
{
	if (!pItem)
	{
		return NULL;
	}

	return pItem->next;
}

bool CJSONParse::GetValueStr(const cJSON* pJson, const std::string &strName, std::string &strValue)
{
	if (!pJson || strName.empty())
	{
		return false;
	}

	cJSON* pItem = cJSON_GetObjectItem(pJson, strName.c_str());
	if (pItem)
	{
		strValue = pItem->valuestring;
		return true;
	}

	return false;
}

void CJSONParse::split(const std::string str,const std::string strPattern, std::vector<std::string> &strResultVec)
{
	std::string substring;
	std::string::size_type start = 0, index = -1;

	do
	{
		index = str.find_first_of(strPattern,start);
		if (index != std::string::npos)
		{    
			substring = str.substr(start,index-start);
			strResultVec.push_back(substring);
			start = str.find_first_not_of(strPattern,index);
			if (start == std::string::npos) return;
		}
	}while(index != std::string::npos);

	substring = str.substr(start);
	strResultVec.push_back(substring);
}

void CJSONParse::printJson(cJSON * root, std::map<std::string, std::string> &KeyValue)//以递归的方式打印json的最内层键值对
{
	for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
	{
		cJSON * item = cJSON_GetArrayItem(root, i);        
		if(cJSON_Object == item->type || cJSON_Array == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
			printJson(item, KeyValue);
		else                                //值不为json对象就直接打印出键和值
		{
			KeyValue[item->string] = item->valuestring;
		}
	}
}