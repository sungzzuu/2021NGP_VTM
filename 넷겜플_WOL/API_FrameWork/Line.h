#pragma once

#ifndef __LINE_H__
#define __LINE_H__

class CLine
{
public:
	CLine();
	CLine(LINEPOS& _Left, LINEPOS& _Right);
	~CLine();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	const LINEINFO& Get_Info() const { return m_tInfo; }

private:
	LINEINFO	m_tInfo;
	
};

#endif // !__LINE_H__
