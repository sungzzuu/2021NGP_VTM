#pragma once

#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__

class CStrCmp
{
public:
	CStrCmp(const TCHAR* _pString)
		: m_pString(_pString)
	{
	}

public:
	template<typename T>
	bool operator()(T& _obj)
	{
		return !lstrcmp(m_pString, _obj.first);
	}

private:
	const TCHAR* m_pString;
};

#endif // !__FUNCTOR_H__
