#ifndef KTongDataH
#define	KTongDataH

struct KTongMap
{
	char szName[80];
	char szPath[MAX_PATH];
};

class KTongLevelAdd
{
private:
	int		m_nExpVal[MAX_TONG_LEVEL];				// Éý¼¶¾­Ñé
	int		m_nEffVal[MAX_TONG_LEVEL];			// Ã¿¸öÏµÉý¼¶¼ÓÉúÃüµã
	int		m_nMemberNum[MAX_TONG_LEVEL];			// Ã¿¸öÏµÉý¼¶¼ÓÉúÃüµã
public:
	KTongLevelAdd();
	BOOL		Init();
	int			GetExp(int nLevel);	
	int			GetEff(int nLevel);			// ´«ÈëµÈ¼¶»ñµÃ¿É´ø¶ÓÔ±Êý
	int			GetMemNum(int nLevel);			// ´«Èë¾­Ñé»ñµÃ¿É´ø¶ÓÔ±Êý
};

class KOneTong
{
private:
	DWORD		m_dwNameID;
	KTongMap	m_sMap;

public:
	KOneTong();
	BOOL		Init();
	BOOL		AddTong();
};


class KTongData
{
private:
#ifdef	_SERVER
	int						m_nNumTong;
public:
	KOneTong				m_sTong[MAX_TONG_COUNT];
#endif

public:
	KTongParam				m_sTongParam;
	KTongLevelAdd			m_sTongLevel;

	KTongData();
	BOOL	Init();
};

extern KTongData TongData;
#endif //KTongDataH
