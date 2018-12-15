// QuestNpc.h: interface for the CQuestNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_)
#define AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStrTokenizer;																		// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.

class CQuestNpc																				// ����Ʈ NPC Ŭ����.
{
protected:
	DWORD		m_dwQuestIdx;																// ����Ʈ �ε����� ��� ����.
	DWORD		m_dwSubQuestIdx;															// ���� ����Ʈ �ε����� ��� ����.
	cPtrList	m_NpcDataList;																// NPC ������ ����Ʈ.

public:
	CQuestNpc( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestNpc();
	void Release();
	BOOL ReadNpcData( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	cPtrList& GetNpcDataList() { return m_NpcDataList; }
};

#endif // !defined(AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_)