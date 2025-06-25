#ifndef KPlayerH
#define	KPlayerH
//#include <objbase.h>
#include "KWorldMsg.h"
#include "KMessage.h"
#include "KInventory.h"
#include "KPlayerFaction.h"
#include "KPlayerChat.h"
#include "KPlayerTask.h"
#include "KPlayerMenuState.h"
#include "KPlayerTrade.h"
#include "KPlayerTeam.h"
#include "KPlayerPK.h"
#include "KPlayerTong.h"
#include "KItemList.h"
#include "KNpc.h"
#include "KSkills.h"

#include "KPlayerDef.h"

#ifndef _SERVER
#include <string>       
#include <winsock2.h>   
#include <ws2tcpip.h>   
#endif

#define		MAX_ANSWERNUM					50
#define		PLAYER_LIFE_REPLENISH			0
#define		PLAYER_MANA_REPLENISH			0
#define		PLAYER_STAMINA_GAIN				1
#define		PLAYER_STAMINA_LOSS				4

#define		STRENGTH_SET_DAMAGE_VALUE		5
#define		DEXTERITY_SET_DAMAGE_VALUE		5

#define		MAX_AVENGE_NUM					4

enum	UIInfo //½Å±¾Í¨ÖªÏÔÊ¾µÄ½çÃæÀàÐÍ
{
	UI_SELECTDIALOG,
	UI_TRADEDIALOG,
	UI_TALKDIALOG,
	UI_NOTEINFO,
	UI_MSGINFO,//×ÔÓÒÏò×óÃ°³öÀ´µÄÐÅÏ¢
	UI_NEWSINFO,//ÐÂÎÅ
	UI_PLAYMUSIC,
	UI_OPENTONGUI,
};

// ÖØÉúµãÎ»ÖÃÐÅÏ¢
typedef struct PLAYER_REVIVAL_POS_DATA
{
	int				m_nSubWorldID;		// ÖØÉúµãµØÍ¼
	int				m_ReviveID;			// ÖØÉúµãË÷Òý
	int				m_nMpsX;			// ÖØÉúµãµØÍ¼Î»ÖÃ x
	int				m_nMpsY;			// ÖØÉúµãµØÍ¼Î»ÖÃ y
} PLAYER_REVIVAL_POS;

typedef struct
{
	int				m_nSubWorldId;		// ´«ËÍÃÅÊÀ½çID
	int				m_nTime;			// ´«ËÍÃÅ±£³ÖÊ±¼ä
	int				m_nMpsX;
	int				m_nMpsY;
} PLAYER_TOWNPORTAL_POS;

typedef struct 
{
	DWORD			m_dwMapID;
	int				m_nX;
	int				m_nY;
} PLAYER_EXCHANGE_POS;

typedef struct BuySellInfo_struct
{
	int m_nBuyIdx;
	DWORD m_SubWorldID;
	int m_nMpsX;
	int m_nMpsY;
	void Clear() { m_nBuyIdx = -1; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0; }
} BuySellInfo;
class KIniFile;


#ifdef TOOLVERSION
class CORE_API KPlayer
#else
class KPlayer
#endif
{
private:

#ifndef _SERVER
	int				m_nLeftSkillID;
	int				m_nLeftSkillLevel;
	int				m_nRightSkillID;
	int				m_nRightSkillLevel;
	BOOL			m_MouseDown[2];
#endif

#ifdef _SERVER
	PLAYER_REVIVAL_POS		m_sLoginRevivalPos;	// µÇÈëÖØÉúµãÎ»ÖÃ£¨»á´æÅÌ£©
	PLAYER_REVIVAL_POS		m_sDeathRevivalPos;	// ËÀÍöÖØÉúµã£¨Ä¬ÈÏÎªµÇÈëÖØÉúµã£¬²»´æÅÌ£©
	PLAYER_TOWNPORTAL_POS	m_sPortalPos;		// ´«ËÍÃÅÎ»ÖÃ
	BOOL			m_bUseReviveIdWhenLogin;
	int				m_nExtPoint;				// »î¶¯µãÊý
	int				m_nChangeExtPoint;			// ±ä¶¯µÄµãÊý
#endif
	int				m_nPhysicsSkillID;		//µ±Ç°Íæ¼ÒµÄÎïÀí¹¥»÷¼¼ÄÜ
	int				m_nPeapleIdx;
	int				m_nObjectIdx;
	int				m_nPickObjectIdx;
	int				m_nPlayerIndex;				// ±¾ÊµÀýÔÚ Player Êý×éÖÐµÄÎ»ÖÃ
	KCacheNode *	m_pLastScriptCacheNode;

public:
#ifdef _SERVER
	PLAYER_EXCHANGE_POS		m_sExchangePos;
	KTimerTaskFun	m_TimerTask;
	BOOL			m_bIsQuiting;
	UINT			m_uMustSave;
	DWORD			m_ulLastSaveTime;
	DWORD			m_dwLoginTime;
//	DWORD			m_uLastPingTime;
	char			m_AccoutName[32];				
	void*			m_pStatusLoadPlayerInfo;	//¼ÓÔØÍæ¼ÒÐÅÏ¢Ê±ÓÃ
	BYTE*			m_pCurStatusOffset;			//¶þ½øÖÆÊ±£¬¼ÇÂ¼¶Áµ½Ö¸ÕëÎ»ÖÃÁË
	BOOL			m_bFinishLoading;			//Íê³É¼ÓÔØ
	BYTE			m_SaveBuffer[64 * 1024];	//±£´æ»º³å
	int				m_nLastNetOperationTime;	//×îºóÒ»´ÎÍøÂç²Ù×÷Ê±¼ä
	BOOL			m_bSleepMode;
	KList			m_PlayerWayPointList;		//
	KList			m_PlayerStationList;
	int				m_nViewEquipTime;			// ×îºóÒ»´Î²ì¿´ËûÈË×°±¸µÄÊ±¼ä
	int				m_nPrePayMoney;
	enum
	{
		FF_CHAT = 0x01,
	};
	int				m_nForbiddenFlag;			// ½ûÖ¹±êÖ¾
#endif

#ifndef _SERVER
	int				m_RunStatus;				// ÊÇÅÜ»¹ÊÇ×ß
	DWORD			m_dwNextLevelLeadExp;		// Í³ÂÊÁ¦ÏÂÒ»¼¶¾­ÑéÖµ
	int				m_nSendMoveFrames;			// ÓÃÓÚ¿ØÖÆ¿Í»§¶ËÏò·þÎñÆ÷·¢ËÍÒÆ¶¯(×ß»òÅÜ)Ð­ÒéµÄÆµÂÊ£¬Ê¹Ö®²»ÄÜ·¢ËÍ´óÁ¿µÄÒÆ¶¯Ð­Òé£¬¼õÐ¡´ø¿íÑ¹Á¦
#endif

	KIndexNode		m_Node;
	GUID			m_Guid;
	BOOL			m_bExchangeServer;
	int				m_DebugMode;
	DWORD			m_dwID;						// Íæ¼ÒµÄ32Î»ID
	int				m_nIndex;					// Íæ¼ÒµÄNpc±àºÅ
	int				m_nNetConnectIdx;			// µÚ¼¸¸öÍøÂçÁ¬½Ó
	KItemList		m_ItemList;					// Íæ¼ÒµÄ×°±¸ÁÐ±í
	BuySellInfo		m_BuyInfo;					// ½øÐÐµÄ½»Ò×ÁÐ±í
	KPlayerMenuState	m_cMenuState;			// ÊÇ·ñ´¦ÓÚ½»Ò×»ò¶ÓÎé¿ª·Å×´Ì¬
	KTrade			m_cTrade;					// ½»Ò×Ä£¿é
	int				m_nAttributePoint;			// Î´·ÖÅäÊôÐÔµã
	int				m_nSkillPoint;				// Î´·ÖÅä¼¼ÄÜµã

	int				m_nStrength;				// Íæ¼ÒµÄ»ù±¾Á¦Á¿£¨¾ö¶¨»ù±¾ÉËº¦£©
	int				m_nDexterity;				// Íæ¼ÒµÄ»ù±¾Ãô½Ý£¨¾ö¶¨ÃüÖÐ¡¢ÌåÁ¦£©
	int				m_nVitality;				// Íæ¼ÒµÄ»ù±¾»îÁ¦£¨¾ö¶¨ÉúÃü¡¢ÌåÁ¦£©
	int				m_nEngergy;					// Íæ¼ÒµÄ»ù±¾¾«Á¦£¨¾ö¶¨ÄÚÁ¦£©
	int				m_nLucky;					// Íæ¼ÒµÄ»ù±¾ÔËÆø£¨Ó°ÏìµÃµ½×°±¸µÄºÃ»µ£©
	BYTE			m_btChatSpecialChannel;		

	int				m_nCurStrength;				// Íæ¼ÒµÄµ±Ç°Á¦Á¿£¨¾ö¶¨»ù±¾ÉËº¦£©
	int				m_nCurDexterity;			// Íæ¼ÒµÄµ±Ç°Ãô½Ý£¨¾ö¶¨ÃüÖÐ¡¢ÌåÁ¦£©
	int				m_nCurVitality;				// Íæ¼ÒµÄµ±Ç°»îÁ¦£¨¾ö¶¨ÉúÃü¡¢ÌåÁ¦£©
	int				m_nCurEngergy;				// Íæ¼ÒµÄµ±Ç°¾«Á¦£¨¾ö¶¨ÄÚÁ¦£©
	int				m_nCurLucky;				// Íæ¼ÒµÄµ±Ç°ÔËÆø£¨Ó°ÏìµÃµ½×°±¸µÄºÃ»µ£©

	int				m_nExp;						// µ±Ç°¾­ÑéÖµ(µ±Ç°µÈ¼¶ÔÚnpcÉíÉÏ)
	int				m_nNextLevelExp;			// ÏÂÒ»¼¶¾­ÑéÖµ

	DWORD			m_dwLeadExp;				// Í³ÂÊÁ¦¾­ÑéÖµ
	DWORD			m_dwLeadLevel;				// Í³ÂÊÁ¦µÈ¼¶
	char			m_PlayerName[32];
	KPlayerTeam		m_cTeam;					// Íæ¼ÒµÄ×é¶ÓÐÅÏ¢
	KPlayerFaction	m_cFaction;					// Íæ¼ÒµÄÃÅÅÉÐÅÏ¢

	KPlayerChat		m_cChat;

	KPlayerTask		m_cTask;					// Íæ¼ÒÈÎÎñÏµÍ³(±äÁ¿)

	KPlayerPK		m_cPK;						// PK¹ØÏµ´¦Àí

	KPlayerTong		m_cTong;					// ×Ô¼ºµÄ°ï»áÐÅÏ¢

	DWORD			m_dwDeathScriptId;			// 

	char			m_szTaskAnswerFun[MAX_ANSWERNUM][32];
	int				m_nAvailableAnswerNum;//µ±Ç°Ñ¡Ôñ½çÃæÏÂ£¬×î´ó»Ø´ðÊý¡£
	bool			m_bWaitingPlayerFeedBack;	//µ±Ç°ÊÇ·ñÕýµÈ´ýÍæ¼ÒÔÚ¿Í»§¶ËµÄ·´À¡¡£¸Ã×´Ì¬ÏÂ£¬µ±Ç°½Å±¾²»ÖÃ¿Õ.ÀàÊ½¶Ô»°Ñ¡ÔñÇé¿ö
//	DWORD			m_dwOutOfDateFeedBackTime;	//µ±Ç°µÈ´ýÍæ¼ÒÓë·þÎñÆ÷½Å±¾½»»¥µÄ×î´óÏÞ¶¨Ê±¿Ì£¬µ½ÁËÕâ¸öÓÎÏ·Ê±¼äÈç¹ûÍæ¼ÒÈÔÎ´·´À¡ÔòÊÓ×÷·Ï¡£
	BYTE			m_btTryExecuteScriptTimes;	//
	//char			m_CurScriptName[128];
	int				m_nWorldStat;
	int				m_nSectStat;

public:
	KPlayer();
	~KPlayer();

	void			SetPlayerIndex(int nNo);					// Éè¶¨ m_nPlayerIndex
	void			GetAboutPos(KMapPos *pMapPos);			// »ñµÃÍæ¼Ò¸½½üÒ»¸ö¿ÕÎ»ÖÃ
	int				GetPlayerIndex();							// »ñµÃ±¾ÊµÀýÔÚ Player Êý×éÖÐµÄÎ»ÖÃ
	DWORD			GetPlayerID(){return m_dwID;};
	void			GetFactionName(char *lpszName, int nSize);	// »ñµÃµ±Ç°ÃÅÅÉÃû³Æ not end
	void			ChatFriendOnLine(DWORD dwID, int nFriendIdx);// »ñµÃÍ¨ÖªÄ³ºÃÓÑÉÏÏßÁË
	BOOL			ExecuteScript(char * ScriptFileName, char * szFunName, int nParam = 0);
	BOOL			ExecuteScript(char * ScriptFileName, char * szFunName, char * szParams);
	BOOL			ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams);
	BOOL			ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam);
	BOOL			DoScript(char * ScriptCommand);				//Ö´ÐÐÄ³¸ö½Å±¾Ö¸Áî

	void			ChangeCurStrength(int nData);			// ¸Ä±äµ±Ç°Á¦Á¿(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	void			ChangeCurDexterity(int nData);			// ¸Ä±äµ±Ç°Ãô½Ý(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	void			ChangeCurVitality(int nData);			// ¸Ä±äµ±Ç°»îÁ¦(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	void			ChangeCurEngergy(int nData);			// ¸Ä±äµ±Ç°¾«Á¦(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	BOOL			ExecuteScript(char * ScriptFileName);
	void			Release();
	void			Active();								// Íæ¼ÒÃ¿´ÎÓÎÏ·Ñ­»·¶¼ÐèÒª´¦ÀíµÄ¶«Î÷
	void			ProcessMsg(KWorldMsgNode *lpMsg);		// ´¦ÀíÊÀ½çÏûÏ¢£¬×ªÎªNPCÃüÁî
	
	LPSTR			GetPlayerName() { return m_PlayerName; };

	BOOL			NewPlayerGetBaseAttribute(int Series);	// ÐÂÍæ¼ÒµÇÂ½Ê±¸ù¾ÝÎåÐÐÊôÐÔ²úÉú Á¦Á¿ Ãô½Ý »îÁ¦ ¾«Á¦ ËÄÏîÊýÖµ
	void			AddBaseLucky(int nData);				// Ôö¼Ó»ù±¾ÔËÆø

	void			AddExp(int nExp, int nTarLevel);		// Ôö¼Ó¾­Ñé(Ô­Ê¼Êý¾Ý£¬»¹Î´¾­¹ý´¦Àí)
	void			AddSelfExp(int nExp, int nTarLevel);	// Ôö¼Ó¾­Ñé(²»ÐèÒªÔÙ¾­¹ý¶ÓÎé·ÖÅäµÄ´¦Àí£¬µ«ÐèÒª¿¼ÂÇ±»¿³ËÀnpcµÄµÈ¼¶)
	void			DirectAddExp(int nExp);					// Ö±½ÓÔö¼Ó¾­ÑéÖµ£¬²»¿¼ÂÇÆäËûÒòËØ
	void			LevelUp();								// ÉýÒ»¼¶
	void			AddLeadExp(int nExp);					// Ôö¼ÓÍ³ÂÊÁ¦¾­Ñé
	void			UpdataCurData();
	void			ReCalcEquip();							// ÖØÐÂ¼ÆËãÉíÉÏµÄ×°±¸
	void			ReCalcState();							// ÖØÐÂ¼ÆËãÉíÉÏµÄ×´Ì¬

	void			ChangePlayerCamp(int nCamp);			// ¸Ä±äÍæ¼ÒÕóÓª
	void			Revive(int nType);						// ÖØÉú	

	BOOL			CheckTrading();
	void			SetFirstDamage();
	void			SetBaseAttackRating();
	void			SetBaseDefence();
	void			SetBaseResistData();
	void			SetBaseSpeedAndRadius();
	int				GetPhysicsSkillId(){return m_nPhysicsSkillID;};
	void			SetPhysicsSkillId(int nPhysicsSkillID)
	{
		if (nPhysicsSkillID <= 0) return ;
		ISkill * pISkill =  g_SkillManager.GetSkill(nPhysicsSkillID, 1);
		if (!pISkill) 
            return ;
		
        if (nPhysicsSkillID > 0 && pISkill->IsPhysical())
			m_nPhysicsSkillID = nPhysicsSkillID;
	};

#ifndef _SERVER
	int				GetTargetNpc() { return m_nPeapleIdx; };
	int				GetTargetObj() { return m_nObjectIdx; };
	void			SetTargetNpc(int n) { m_nPeapleIdx = n; };
	void			SetTargetObj(int n) { m_nObjectIdx = n; };
	void			FindSelectNpc(int x, int y, int nRelation);
	void			FindSelectObject(int x, int y);
	void			Walk(int nDir, int nSpeed);	
	void			TurnLeft();
	void			TurnRight();
	void			TurnBack();
	void			DrawSelectInfo();
	BOOL			ConformIdx(int nIdx);	
	void			GetEchoDamage(int* nMin, int* nMax, int nType);// »ñÈ¡½çÃæÐèÒªÏÔÊ¾µÄÉËº¦Öµ
	void			ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);// ´¦Àí¼üÅÌÊó±êÏûÏ¢
	void			RecvSyncData();								// ½ÓÊÕÍ¬²½Êý¾Ý

	void			ApplyTeamInfo(DWORD dwNpcID);				// Ïò·þÎñÆ÷ÉêÇë²éÑ¯Ä³¸önpcËùÔÚ¶ÓÎéµÄÐÅÏ¢
	void			ApplySelfTeamInfo();						// Ïò·þÎñÆ÷ÉêÇë²éÑ¯Íæ¼Ò×ÔÉíµÄ¶ÓÎéÇé¿ö
	BOOL			ApplyCreateTeam();//char *lpszTeamName);		// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë´´½¨¶ÓÎé
	BOOL			ApplyTeamOpenClose(BOOL bFlag);				// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë¿ª·Å¡¢¹Ø±Õ¶ÓÎéÊÇ·ñÔÊÐí¼ÓÈë³ÉÔ±×´Ì¬
	void			ApplyAddTeam(int nNpcIndex);				// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë¼ÓÈëÄ³¸ö¶ÓÎé
	void			AcceptTeamMember(DWORD dwNpcID);			// Íæ¼ÒÍ¨Öª·þÎñÆ÷½ÓÊÜÄ³¸önpcÎª¶ÓÎé³ÉÔ±
	void			TeamDropApplyOne(DWORD dwNpcID);			// ¶Ó³¤É¾³ý¼ÓÈë¶ÓÎéÉêÇëÁÐ±íÖÐµÄÄ³¸önpc
	void			LeaveTeam();								// Í¨Öª·þÎñÆ÷±¾Íæ¼ÒÀë¿ª¶ÓÎé
	void			TeamKickMember(DWORD dwNpcID);				// ¶Ó³¤Í¨Öª·þÎñÆ÷Ìß³ýÄ³¸ö¶ÓÔ±
	void			ApplyTeamChangeCaptain(DWORD dwNpcID);		// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë°Ñ×Ô¼ºµÄ¶Ó³¤Éí·Ý½»¸ø±ðµÄ¶ÓÔ±
	void			ApplyTeamDismiss();							// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë½âÉ¢¶ÓÎé
	void			ApplySetPK(BOOL bPK);						// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë´ò¿ª¡¢¹Ø±Õpk¿ª¹Ø
	void			ApplyFactionData();							// Íæ¼ÒÏò·þÎñÆ÷ÉêÇëÃÅÅÉÊý¾Ý
	void			SendChat(KUiMsgParam *pMsg, char *lpszSentence);// ¿Í»§¶Ë·¢ËÍÁÄÌìÓï¾ä¸ø·þÎñÆ÷
	void			ApplyAddBaseAttribute(int nAttribute, int nNo);// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇëÔö¼ÓËÄÏîÊôÐÔÖÐÄ³Ò»ÏîµÄµãÊý(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	BOOL			ApplyAddSkillLevel(int nSkillID, int nAddPoint);// Ïò·þÎñÆ÷ÉêÇëÄ³¸ö¼¼ÄÜÉý¼¶
	BOOL			ApplyUseItem(int nItemID, ItemPos SrcPos);	// Ïò·þÎñÆ÷ÉêÇëÊ¹ÓÃÄ³¸öÎïÆ·£¨Êó±êÓÒ¼üµã»÷¸ÃÎïÆ·£©
	void			PickUpObj(int nObjIndex);					// ¿Í»§¶ËÊó±êµã»÷obj¼ìÆðÄ³¸öÎïÆ·£¬Ïò·þÎñÆ÷·¢ÏûÏ¢
	void			ObjMouseClick(int nObjIndex);				// ¿Í»§¶ËÊó±êµã»÷obj£¬Ïò·þÎñÆ÷·¢ÏûÏ¢
	void			MoveItem(ItemPos DownPos, ItemPos UpPos);	// DownPos ²»ÄÜÊÇÊÖ£¬UpPos ±ØÐëÊÇÊÖ
	int				ThrowAwayItem();
	void			ChatAddFriend(int nPlayerIdx);				// ¿Í»§¶ËÍ¨¹ý±ðÈËµÄÌí¼ÓÁÄÌìºÃÓÑµÄÉêÇë
	void			ChatRefuseFriend(int nPlayerIdx);			// ¿Í»§¶Ë¾Ü¾ø±ðÈËµÄÌí¼ÓÁÄÌìºÃÓÑµÄÉêÇë
	void			TradeApplyOpen(char *lpszSentence, int nLength);
	void			TradeApplyClose();
	void			TradeApplyStart(int nNpcIdx);
	BOOL			TradeMoveMoney(int nMoney);					// ½»Ò×Ê±ÊäÈë×Ô¼ºµÄÇ®	
	void			TradeDecision(int nDecision);				// if nDecision == 0 ÍÆ³ö½»Ò×  if nDecision == 1 È·¶¨½»Ò×  if nDecision == 2 È¡Ïû½»Ò×È·¶¨
	void			TradeApplyLock(int nLockOrNot);				// ½»Ò×Ëø¶¨»òÈ¡ÏûËø¶¨

	void			SetChatCurChannel(int nChannelNo);			// Éè¶¨µ±Ç°ÁÄÌìÆµµÀ
	void			TeamInviteAdd(DWORD dwNpcID);				// ÑûÇë¼ÓÈë¶ÓÎé

	void			SetLeftSkill(int nSkillID);
	void			SetRightSkill(int nSkillID);
	void			UpdateWeaponSkill();
	int				GetLeftSkill(){return m_nLeftSkillID;};
	int				GetRightSkill(){return m_nRightSkillID;};
	void			SetDefaultImmedSkill();
	void			s2cApplyAddTeam(BYTE* pProtocol);			// ÊÕµ½·þÎñÆ÷Í¨ÖªÓÐÈËÉêÇë¼ÓÈë¶ÓÎé
	void			s2cTradeChangeState(BYTE* pMsg);
	void			s2cTradeMoneySync(BYTE* pMsg);
	void			s2cTradeDecision(BYTE* pMsg);				// ÊÕµ½·þÎñÆ÷Í¨Öª½»Ò×Íê³É»òÈ¡Ïû
	void			SyncCurPlayer(BYTE* pMsg);
	void			s2cLevelUp(BYTE* pMsg);
	void			s2cGetCurAttribute(BYTE* pMsg);
	void			s2cSetExp(int nExp);
	void			s2cSyncMoney(BYTE* pMsg);
	void			s2cTradeApplyStart(BYTE* pMsg);
	void			CheckObject(int nIdx);
#endif

#ifdef _SERVER
	void			SetExtPoint(int nPoint, int nChangePoint);
	int				GetExtPoint();
	BOOL			PayExtPoint(int nPoint);
	int				GetExtPointChanged();
	void			RepairItem(DWORD dwItemID);
	BOOL			PrePay(int nMoney);
	void			SetLastNetOperationTime(int nTime);
	int				FindAroundPlayer(DWORD dwNpcID);		// Ñ°ÕÒÍæ¼ÒÖÜÎ§µÄÄ³¸öÖ¸¶¨npc idµÄplayer index
	int				FindAroundNpc(DWORD dwNpcID);			// Ñ°ÕÒÍæ¼ÒÖÜÎ§µÄÄ³¸öÖ¸¶¨npc idµÄnpc index
	BOOL			CheckPlayerAround(int nPlayerIdx);		// ÅÐ¶ÏÄ³Íæ¼ÒÊÇ·ñÔÚÖÜÎ§
	BOOL			IsExchangingServer();
	void			TobeExchangeServer(DWORD dwMapID, int nX, int nY);
//	void			UpdateEnterGamePos(DWORD dwSubWorldID, int nX, int nY, int nFightMode);
	BOOL			IsWaitingRemove();
	BOOL			IsLoginTimeOut();
	void			WaitForRemove();
	void			LoginTimeOut();
	void			UseTownPortal();
	void			BackToTownPortal();
	void			GetLoginRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);		// »ñÈ¡Íæ¼ÒµÇÈëÖØÉúµãÎ»ÖÃ
	void			GetDeathRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);		// »ñÈ¡Íæ¼ÒËÀÍöÖØÉúµãÎ»ÖÃ
	void			SetRevivalPos(int nSubWorld, int nRevalId);								// Éè¶¨Íæ¼ÒÖØÉúµãID
	BOOL			Save();									// ±£´æÍæ¼ÒÊý¾Ý
	BOOL			CanSave();
	void			ProcessUser();
	BOOL			SendSyncData(int &nStep, unsigned int &nParam);	// ·¢ËÍÍ¬²½Êý¾Ý
	BOOL			SendSyncData_Skill();					// ·¢ËÍÍ¬²½Êý¾Ý - ¼¼ÄÜ
	void			SendCurNormalSyncData();				// ·¢ËÍÆ½Ê±¸ø×Ô¼ºµÄÍ¬²½Êý¾Ý
	void			SetChatForbiddenFlag(int nFlag)
	{
		m_nForbiddenFlag = nFlag;
	};

	BOOL			AddFaction(char *lpszFactionName);		// ¼ÓÈëÃÅÅÉ
	BOOL			AddFaction(int nFactionID);				// ¼ÓÈëÃÅÅÉ
	BOOL			LeaveCurFaction();						// Àë¿ªÃÅÅÉ
	BOOL			CurFactionOpenSkill(int nLevel);		// ¿ª·Åµ±Ç°ÃÅÅÉÄ³¸öµÈ¼¶µÄ¼¼ÄÜ

	void			TaskClearTempVal();						// Çå¿ÕÈÎÎñÁÙÊ±±äÁ¿
	int				TaskGetSaveVal(int nNo);				// µÃµ½ÈÎÎñÍê³ÉÇé¿ö
	void			TaskSetSaveVal(int nNo, BOOL bFlag);	// Éè¶¨ÈÎÎñÍê³ÉÇé¿ö
	int				TaskGetClearVal(int nNo);				// µÃµ½ÈÎÎñÁÙÊ±¹ý³Ì¿ØÖÆ±äÁ¿Öµ
	void			TaskSetClearVal(int nNo, int nVal);		// Éè¶¨ÈÎÎñÁÙÊ±¹ý³Ì¿ØÖÆ±äÁ¿Öµ

	//void			SetTimeTaskTime(DWORD time){m_dwTimeTaskTime = time;};
	void			BuyItem(BYTE* pProtocol);
	void			SellItem(BYTE* pProtocol);
	void			QuitGame(int nQuitType);				// ÍË³öÓÎÏ·
	void			S2CSendTeamInfo(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶Ë²éÑ¯Ä³¸önpc×é¶ÓÐÅÏ¢µÄÉêÇëºó£¬ÏòÕâ¸ö¿Í»§¶Ë·¢ËÍ¶ÓÎéÐÅÏ¢
	void			SendSelfTeamInfo();						// ·þÎñÆ÷Ïò¿Í»§¶Ë·¢ËÍ¶ÓÎéÐÅÏ¢
	BOOL			CreateTeam(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÇëÇó´´½¨Ò»Ö§¶ÓÎé
	BOOL			SetTeamState(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÇëÇó¿ª·Å¡¢¹Ø±Õ±¾player¶ÓÎéÊÇ·ñÔÊÐí¶ÓÔ±¼ÓÈë×´Ì¬
	BOOL			S2CSendAddTeamInfo(BYTE* pProtocol);	// ÊÕµ½¿Í»§¶ËÇëÇó¼ÓÈëÒ»Ö§¶ÓÎé
	BOOL			AddTeamMember(BYTE* pProtocol);			// ¶Ó³¤Í¨Öª·þÎñÆ÷½ÓÊÜÄ³¸önpcÎª¶ÓÎé³ÉÔ±
	void			LeaveTeam(BYTE* pProtocol);				// ÊÕµ½¿Í»§¶Ë¶ÓÔ±Í¨ÖªÀë¿ª¶ÓÎé
	void			TeamKickOne(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶Ë¶Ó³¤Í¨ÖªÌß³öÄ³¸ö¶ÓÔ±
	void			TeamChangeCaptain(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶Ë¶Ó³¤Í¨Öª°Ñ¶Ó³¤Éí·Ý½»¸øÄ³¸ö¶ÓÔ±
	void			TeamDismiss(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶Ë¶Ó³¤ÇëÇó½âÉ¢¶ÓÎé
	void			SetPK(BYTE* pProtocol);					// ÊÕµ½¿Í»§¶ËÇëÇóÉè¶¨PK×´Ì¬
	void			SendFactionData(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÇëÇó»ñµÃÃÅÅÉÊý¾Ý
	void			ServerSendChat(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶Ë·¢À´µÄÁÄÌìÓï¾ä
	void			AddBaseAttribute(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÒªÇóÔö¼Ó»ù±¾ÊôÐÔµã(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	void			AddSkillPoint(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÒªÇóÔö¼ÓÄ³¸ö¼¼ÄÜµÄµãÊý
	BOOL			ServerPickUpItem(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÏûÏ¢Êó±êµã»÷Ä³¸öobj¼ðÆð×°±¸»ò½ðÇ®
	void			EatItem(BYTE* pProtocol);				// ÊÕµ½¿Í»§¶ËÏûÏ¢³ÔÒ©
	void			ServerMoveItem(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÏûÏ¢ÒÆ¶¯ÎïÆ·
	void			ServerThrowAwayItem(BYTE* pProtocol);	// ÊÕµ½¿Í»§¶ËÏûÏ¢¶ªÆúÎïÆ·
	void			ChatSetTakeChannel(BYTE* pProtocol);	// ÊÕµ½¿Í»§¶ËÏûÏ¢Éè¶¨ÁÄÌì¶©ÔÄÆµµÀ
	void			ChatTransmitApplyAddFriend(BYTE* pProtocol);// ÊÕµ½¿Í»§¶ËÇëÇó×ª·¢ÁÄÌìÌí¼ÓºÃÓÑÐÅÏ¢
	BOOL			ChatAddFriend(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÏûÏ¢Ìí¼ÓÁÄÌìºÃÓÑ
	void			ChatRefuseFriend(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÏûÏ¢¾Ü¾øÌí¼ÓÁÄÌìºÃÓÑ
	void			ChatResendAllFriend(BYTE* pProtocol);
	void			ChatSendOneFriendData(BYTE* pProtocol);
	void			ChatDeleteFriend(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇëÉ¾³ýÄ³¸öÁÄÌìºÃÓÑ
	void			ChatRedeleteFriend(BYTE* pProtocol);	// ÊÕµ½¿Í»§¶ËÉêÇëÔÙ´ÎÉ¾³ýÄ³¸öÁÄÌìºÃÓÑ
	void			TradeApplyOpen(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë½øÈë´ý½»Ò××´Ì¬
	void			TradeApplyClose(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇëÈ¡Ïû´ý½»Ò××´Ì¬
	void			TradeApplyStart(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë¿ªÊ¼½»Ò×
	void			TradeMoveMoney(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë½»Ò×ÖÐmoneyµÄ¸Ä±ä
	void			TradeDecision(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÉêÇë½»Ò×È·¶¨»òÈ¡Ïû
	void			c2sTradeReplyStart(BYTE* pProtocol);
	void			SyncTradeState();						// ¸ø½»Ò×Ë«·½µÄ¿Í»§¶Ë·¢ËÍ½»Ò××´Ì¬ÐÅÏ¢
	void			SendEquipItemInfo(int nTargetPlayer);	// ·¢ËÍ×Ô¼º×°±¸ÔÚÉíÉÏµÄ×°±¸ÐÅÏ¢¸ø±ðÈË¿´
	PLAYER_REVIVAL_POS* GetDeathRevivalPos() 
	{
		return &m_sDeathRevivalPos;
	};
	BOOL			IsUseReviveIdWhenLogin() { return m_bUseReviveIdWhenLogin; };
	void			SetLoginType(BOOL bUseReviveId) { m_bUseReviveIdWhenLogin = bUseReviveId; };

	BOOL			CreateTong(int nCamp, char *lpszTongName);
#endif

private:
//	void			CalcCurStrength();						// ¼ÆËãµ±Ç°Á¦Á¿
//	void			CalcCurDexterity();						// ¼ÆËãµ±Ç°Ãô½Ý
//	void			CalcCurVitality();						// ¼ÆËãµ±Ç°»îÁ¦
//	void			CalcCurEngergy();						// ¼ÆËãµ±Ç°¾«Á¦
	void			CalcCurLucky();							// ¼ÆËãµ±Ç°ÔËÆø
	void			LevelAddBaseLifeMax();					// µÈ¼¶ÉýÒ»¼¶ºóÔö¼Ó×î´óÉúÃüµã
	void			LevelAddBaseManaMax();					// µÈ¼¶ÉýÒ»¼¶ºóÔö¼Ó×î´óÄÚÁ¦µã
	void			LevelAddBaseStaminaMax();				// µÈ¼¶ÉýÒ»¼¶ºóÔö¼Ó×î´óÌåÁ¦µã

	void			SendFactionData();						// Ïò¿Í»§¶Ë·¢ËÍÃÅÅÉÊý¾Ý

	void			AddBaseStrength(int nData);				// Ôö¼Ó»ù±¾Á¦Á¿
	void			AddBaseDexterity(int nData);			// Ôö¼Ó»ù±¾Ãô½Ý
	void			AddBaseVitality(int nData);				// Ôö¼Ó»ù±¾»îÁ¦
	void			AddBaseEngergy(int nData);				// Ôö¼Ó»ù±¾¾«Á¦
	void			SetNpcPhysicsDamage();					// ÓÉµ±Ç°Á¦Á¿¼ÆËã¶ÔÓ¦npcµÄÎïÀíÉËº¦(PhysicsDamage)
	void			SetNpcAttackRating();					// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄ¹¥»÷ÃüÖÐÂÊ(AttackRating)
	void			SetNpcDefence();						// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄ·ÀÓùÁ¦
//	void			SetNpcWalkSpeed();						// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄÐÐ×ßËÙ¶È
//	void			SetNpcRunSpeed();						// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄÅÜ²½ËÙ¶È

#ifndef _SERVER
	void			ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton);
	void			OnButtonUp(int x,int y,MOUSE_BUTTON nButton);				// ´¦ÀíÊó±ê¼üÌ§Æð
	void			OnButtonDown(int x,int y,int Key,MOUSE_BUTTON nButton);		// ´¦ÀíÊó±ê¼ü°´ÏÂ
	void			OnButtonMove(int x,int y,int Key,MOUSE_BUTTON nButton);		// ´¦ÀíÊó±ê¼ü°´ÏÂºóÒÆ¶¯
	void			OnMouseMove(int x,int y);									// ´¦ÀíÊó±êÒÆ¶¯
	int				NetCommandPlayerTalk(BYTE* pProtocol);

	//Question:Îªµ¥»ú²âÊÔ°æÊ¹ÓÃ
	friend int		LuaInitStandAloneGame(Lua_State* L);
#endif


// ¶¹¶¹µÄ¶«Î÷
private:
	void			S2CExecuteScript(char * ScriptName, char * szParam);

#ifdef _SERVER
//Êý¾Ý¿âÄ£¿éº¯Êý-----------------
private:
	int				LoadPlayerBaseInfo(BYTE * pRoleBuffer, BYTE * &pRoleBaseBuffer, unsigned int &nParam );
	int				LoadPlayerItemList(BYTE * pRoleBuffer, BYTE * &pItemBuffer, unsigned int &nParam );
	int				LoadPlayerFightSkillList(BYTE * pRoleBuffer, BYTE * &pSkillBuffer, unsigned int &nParam);
	int				LoadPlayerLifeSkilllList(BYTE * pRoleBuffer, BYTE * &pSkillBuffer, unsigned int &nParam);
	int				LoadPlayerFriendList(BYTE * pRoleBuffer, BYTE * &pFriendBuffer, unsigned int &nParam);
	int				LoadPlayerTaskList(BYTE * pRoleBuffer, BYTE * &pTaskBuffer, unsigned int &nParam);
	int				SavePlayerBaseInfo(BYTE * pRoleBuffer);
	int				SavePlayerItemList(BYTE * pRoleBuffer);
	int				SavePlayerFightSkillList(BYTE * pRoleBuffer);
	int				SavePlayerLifeSkilllList(BYTE * pRoleBuffer);
	int				SavePlayerFriendList(BYTE * pRoleBuffer);
	int				SavePlayerTaskList(BYTE * pRoleBuffer);
#endif

public:
	void			SetNpcDamageAttrib();
	void			DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo); //Í¨Öª¸Ã¿Í»§¶ËÏÔÊ¾Ä³¸öUI½çÃæ
	void			ProcessPlayerSelectFromUI(BYTE* pProtocol);			// ´¦Àíµ±Íæ¼Ò´ÓÑ¡Ôñ²Ëµ¥Ñ¡ÔñÄ³ÏîÊ±µÄ²Ù×÷
#ifndef _SERVER
	void			DialogNpc(int nIndex);
	void			OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo);//µ±Íæ¼Ò´ÓÑ¡Ôñ¿òÖÐÑ¡ÔñÄ³Ïîºó£¬½«Ïò·þÎñÆ÷·¢ËÍ			
	void			OnScriptAction(PLAYER_SCRIPTACTION_SYNC * );
#endif
#ifdef _SERVER
	void			RestoreLiveData();						//ÖØÉúºó»Ö¸´Íæ¼ÒµÄ»ù±¾Êý¾Ý
	void			SetTimer(DWORD nTime, int nTimeTaskId);//Ê±¼äÈÎÎñ½Å±¾£¬¿ªÆô¼ÆÊ±Æ÷
	void			CloseTimer();							//¹Ø±ÕÊ±¼ä¼ÆÊ±Æ÷

	int				AddDBPlayer(char * szPlayerName, int sex, DWORD * pdwID );
	int				LoadDBPlayerInfo(BYTE * pPlayerInfo,  int &nStep, unsigned int &nParam);
	BOOL			GetNewPlayerFromIni(KIniFile * pIniFile, BYTE * pRoleBuffer);
	int				UpdateDBPlayerInfo(BYTE * pPlayerInfo);
	int				DeletePlayer(char * szPlayerName = NULL);//×¢Òâ£º±¾º¯ÊýÊÇÇå³ýÍæ¼ÒÕÊºÅ£¡£¡£¡£¬²»ÄÜÂÒÓÃ
	void			LaunchPlayer();
	BOOL			Pay(int nMoney);
	BOOL			Earn(int nMoney);
	void			DialogNpc(BYTE * pProtocol);

	int				AddTempTaskValue(void* pData);
#endif
};

#ifdef TOOLVERSION
extern CORE_API KPlayer	Player[MAX_PLAYER];
#else
extern KPlayer	Player[MAX_PLAYER];
#endif
#endif //KPlayerH
