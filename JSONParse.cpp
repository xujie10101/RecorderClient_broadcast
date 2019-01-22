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
		// ����·���еĸ��ڵ㣺pXMLElement->Name() != strIter.
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

void CJSONParse::printJson(cJSON * root, std::map<std::string, std::string> &KeyValue)//�Եݹ�ķ�ʽ��ӡjson�����ڲ��ֵ��
{
	for(int i=0; i<cJSON_GetArraySize(root); i++)   //���������json��ֵ��
	{
		cJSON * item = cJSON_GetArrayItem(root, i);        
		if(cJSON_Object == item->type || cJSON_Array == item->type)      //�����Ӧ����ֵ��ΪcJSON_Object�͵ݹ����printJson
			printJson(item, KeyValue);
		else                                //ֵ��Ϊjson�����ֱ�Ӵ�ӡ������ֵ
		{
			KeyValue[item->string] = item->valuestring;
		}
	}
}