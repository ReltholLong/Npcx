//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.CPP
// Date:	2002.08.26
// Code:	DongBo
// Desc:    CPP file. ±¾ÎÄ¼þÊµÏÖµÄÀàÓÃÓÚÉú³ÉµÀ¾ß
//---------------------------------------------------------------------------
#include "KEngine.h"
#include "KCore.h"
#include "MyAssert.H"
#include "KItem.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"

KItemGenerator	ItemGen;			//	×°±¸Éú³ÉÆ÷

KItemGenerator::KItemGenerator()
{
}

KItemGenerator::~KItemGenerator()
{
}

/******************************************************************************
	¹¦ÄÜ£º	Êý¾Ý³õÊ¼»¯. ´Ótab fileÖÐ¶ÁÈ¡Êý¾Ý
******************************************************************************/
BOOL KItemGenerator::Init()
{
	if (!m_BPTLib.Init())		// ´Ëµ÷ÓÃ´ÓÈô¸ÉµÄtab fileÖÐÔØÈëËùÓÐµÄ³õÊ¼ÊôÐÔ
		return FALSE;

	for (int i = equip_meleeweapon; i < equip_detailnum - equip_meleeweapon; i++)
	{
		switch(i)
		{
		case equip_meleeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMeleeWeaponRecordNumber();
			break;
		case equip_rangeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRangeWeaponRecordNumber();
			break;
		case equip_armor:
			m_EquipNumOfEntries[i] = m_BPTLib.GetArmorRecordNumber();
			break;
		case equip_helm:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHelmRecordNumber();
			break;
		case equip_boots:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBootRecordNumber();
			break;
		case equip_belt:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBeltRecordNumber();
			break;
		case equip_cuff:
			m_EquipNumOfEntries[i] = m_BPTLib.GetCuffRecordNumber();
			break;
		case equip_amulet:
			m_EquipNumOfEntries[i] = m_BPTLib.GetAmuletRecordNumber();
			break;
		case equip_ring:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRingRecordNumber();
			break;
		case equip_pendant:
			m_EquipNumOfEntries[i] = m_BPTLib.GetPendantRecordNumber();
			break;
		case equip_horse:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHorseRecordNumber();
			break;
		}
	}
	m_MedNumOfEntries = m_BPTLib.GetMedicineRecordNumber();
	return TRUE;
/*
	m_BPTLib.Init();
	int naryMALevel[] = {1,2,3,4,5,6};
	KItem theItem;
	Gen_Equipment(1, 1, 0, 5, naryMALevel, 45, &theItem);
	return TRUE;
*/
}

/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉÒ©Æ·
	Èë¿Ú:	nDetailType: Ò©Æ·ÀàÐÍ
			nLevel: µÈ¼¶
			nVersion: °æ±¾ºÅ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_Medicine(IN int nDetailType,
								  IN int nLevel,
								  IN int nVersion,
								  IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	
#ifdef _SERVER	// ·þÎñÆ÷°æ±¾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = 0;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;
#endif

	const int i = nDetailType * 5 + nLevel - 1;	// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
												// µÈ¼¶Êý¾Ý´Ó1¿ªÊ¼£¬ËùÒÔÒª¼õ»ØÀ´
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pMed;
	return TRUE;
}
/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉËæ»ú×°±¸
	Èë¿Ú:	nSeriesReq: ÎåÐÐÊôÐÔ
			nLevel: µÈ¼¶
			nLucky: ÔËÆøÖµ
			nVersion: °æ±¾ºÅ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_Item(IN int nItemGenre,
							  IN int nSeriesReq,
							  IN int nLevel,
							  IN int nLucky,
							  IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	switch (nItemGenre)
	{
	case item_equip:
		{
			int nDetailType, nParticularType, i;
			nDetailType = g_Random(equip_detailnum);
			nParticularType = g_Random(m_EquipNumOfEntries[nDetailType] + 1);
			int naryMALevel[6];
			ZeroMemory(naryMALevel, 6 * sizeof(int));
			for (i = 0; i < 6; i++)
			{
				naryMALevel[i] = g_Random(nLevel / 10 + 1);
				if (!naryMALevel[i])
					break;
			}
			Gen_Equipment(nDetailType, nParticularType, nSeriesReq, nLevel, naryMALevel, nLucky, g_SubWorldSet.GetGameVersion(), pItem);
		}
		break;
	case item_medicine:
		{
			int nDetailType;
			nDetailType = g_Random(medicine_detailnum);
			Gen_Medicine(nDetailType, nLevel, g_SubWorldSet.GetGameVersion(), pItem);
		}
		break;
	case item_task:
		{
			Gen_Quest(nSeriesReq, pItem);			
		}
		break;
	case item_townportal:
		{
			Gen_TownPortal(pItem);
		}
		break;
	default:
		break;
	}
	
	return TRUE;
}
/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉËæ»ú×°±¸
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð, Èç½ü³ÌÎäÆ÷¡¢°µÆ÷¡¢......
			nParticularType: ÏêÏ¸Àà±ð
			nSeriesReq: ÎåÐÐÊôÐÔ
			nLevel: µÈ¼¶
			pnaryMALevel: Ä§·¨ÊôÐÔµÈ¼¶Êý×é[6]
			nLucky: ÔËÆøÖµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_Equipment(IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeriesReq,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
#ifdef _SERVER	// ·þÎñÆ÷°æ±¾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;
#endif
	// ¸ù¾ÝÈë¿Ú²ÎÊý, È·¶¨×°±¸µÄ»ù±¾Êý¾Ý
	const int i = nParticularType * 10 + nLevel - 1;// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
												// Ïê¼ûSPE 2002Äê8ÔÂ31ÈÕ7:40
												// email: »ù´¡µÀ¾ßÊýÖµ±í
												// µÈ¼¶Êý¾Ý´Ó1¿ªÊ¼£¬ËùÒÔÒª¼õ»ØÀ´
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	default:
		break;
	}
	if (NULL == pEqu)
		{ _ASSERT(FALSE); return bEC; }
		// ÔËÐÐÖÁ´ËµÄÖ±½ÓÔ­Òò: Ö»ÓÐnÖÖ×°±¸, ¶øÉÏÃæ i µÄÖµÔÚ[0,n-1]Ö®Íâ
		// ¼ì²é3µã: nParticularType ÓÐÎó?
		//			nLevel ÓÐÎó?
		//			Ô­Ê¼µÄtab fileÓÐÎóµ¼ÖÂ m_BPTLib.m_BPTEquipment Ëù
		//			¹ÜÀíµÄÊý¾ÝÓÐÎÊÌâ?
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeriesReq);

	if (NULL == pnaryMALevel)
		return TRUE;
	// È·¶¨×°±¸µÄÄ§·¨ÊôÐÔ
	KItemNormalAttrib	sMA[6];	// µÀ¾ßµÄÄ§·¨ÊôÐÔ
	bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA);
	if (bEC)
		pItem->SetAttrib_MA(sMA);

	return bEC;
}

/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉÄ§·¨ÊôÐÔ
	Èë¿Ú:	nType: ×°±¸ÀàÐÍ
			pnaryMALevel: Êý×éÖ¸Õë, ¸ø³öÄ§·¨µÄµÈ¼¶ÒªÇó, ¹²6¸ö
			nSeriesReq: ÎåÐÐÊôÐÔ
			nLucky: ÔËÆøÖµ
	³ö¿Ú:	ÔÚ pnaryMA ËùÖ¸Êý×éÖÐ·µ»ØÄ§·¨µÄÊôÐÔºËÐÄÊý¾Ý, ¹²6¸ö
******************************************************************************/
// Add by Freeway Chen in 2003.5.30
BOOL KItemGenerator::Gen_MagicAttrib(
    int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
	KItemNormalAttrib* pnaryMA
)
{
    int nResult = false;

    KBPT_ClassMAIT  SelectedMagicTable;	
	// ¼ÇÂ¼Ã¿Ò»¸ö¼ÇÂ¼Ñ¡ÖÐµÄÄ§·¨ÊôÐÔ£¬Ö÷ÒªÊÇÎªÁË×îºó½«Ê¹ÓÃ±êÖ¾Çå³ý
    KMAGICATTRIB_TABFILE *pMagicAttrTable[6] = { NULL, NULL, NULL, NULL, NULL, NULL };

    int i = 0;

    _ASSERT(pnaryMALevel);
    _ASSERT(pnaryMA);

    for (i = 0; i < 6; i++) 
    {
        // Èç¹û·¢ÏÖ¼¶±ðÎªÁã£¬±íÊ¾²»ÐèÒª¼ÌÐø´¦ÀíÁË£¬ÍË³ö
        if (pnaryMALevel[i] == 0)	
        	break;

        SelectedMagicTable.Clear();

        // ¸ù¾ÝÇ°×ººó×º¡¢ÎïÆ·ÀàÐÍ¡¢ÎåÐÐÐèÇó¡¢ÐèÇó¼¶±ð½«ËùÓÐ·ûºÏÌõ¼þµÄÄ§·¨ÊôÐÔÏîÈ¡³öÀ´
        const KBPT_ClassMAIT *pCMITItem = m_BPTLib.GetCMIT(
            1 - (i & 1),        // Ç°×ººó×º
            nType,              // ÎïÆ·ÀàÐÍ
            nSeriesReq,         // ÎåÐÐÐèÇó 
            pnaryMALevel[i]     // ÐèÇó¼¶±ð
        );

        _ASSERT(pCMITItem);
        if (!pCMITItem)
        {
            g_DebugLog(
                "[GenMagicAttrib] GetCMIT Error, nPrefixPostfix = %d, nType = %d, nSeries = %d, nLevel = %d", 
                1 - (i & 1), nType, nSeriesReq, pnaryMALevel[i]
            );
            break;
        }

        // ±éÀú·ûºÏÌõ¼þµÄÄ§·¨ÊôÐÔ£¬¸ù¾Ý
        // 1.ÊÇ·ñÒÑ¾­Ê¹ÓÃ£¬
        // 2.ÔËÆøÖµ
        // 3.ÊÇ·ñÓÐÏàÍ¬µÄ¡°ÊôÐÔµ÷ÕûÀà±ð¡±
        // È¡³öÏàÓ¦µÄÏî·ÅÔÚSelectedMagicTableÖÐ
        int nCMITItemCount = pCMITItem->GetCount();
        int j = 0;
        int nDecide = GetRandomNumber(0, 100 - 1) / (1 + nLucky * 20 / 100);

        for (j = 0; j < nCMITItemCount; j++)
        {
            // ¸ù¾ÝË÷ÒýÖµ£¬È¡³öÏàÓ¦µÄÏî
            int nMAIndex = pCMITItem->Get(j);
            KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(nMAIndex);
            if (!pMAItem)
            {
                _ASSERT(pMAItem);
                continue;
            }

            // Èç¹ûÒÑ¾­Ê¹ÓÃ£¬Ìø¹ýÈ¡ÏÂÒ»¸ö
            if (pMAItem->m_nUseFlag)
                continue;

            // Èç¹ûÔËÆø²»×ã£¬È¡ÏÂÒ»¸ö
            if ((pMAItem->m_DropRate[nType]) <= nDecide)
                continue;
            
            // ¼ì²éÊÇ·ñÒÑ¾­ÓÐÏàÍ¬µÄ¡°ÊôÐÔµ÷ÕûÀà±ð¡±
            int k = 0;
            for (k = 0; k < i; k++)
            {
                _ASSERT(pMagicAttrTable[k]);

                if ((pMagicAttrTable[k]->m_MagicAttrib.nPropKind) == (pMAItem->m_MagicAttrib.nPropKind))
                    break;
            }

            // Èç¹û·¢ÏÖ ¡°ÊôÐÔµ÷ÕûÀà±ð¡±ÏàÍ¬£¬ËµÃ÷ÒÑ¾­ÓÐÒ»¸öÏàÍ¬µÄÖµ±»Ñ¡ÖÐ£¬ÄÇÃ´È¡ÏÂÒ»¸ö
            if (k < i)
                continue;

            SelectedMagicTable.Insert(nMAIndex);
        }

        int nSelectedCount = SelectedMagicTable.GetCount();

        if (nSelectedCount == 0)
        {   
            // Èç¹ûÃ»ÓÐÂú×ãÌõ¼þµÄÄ§·¨ÊôÐÔ£¬¾ÍÍË³ö
            break;
        }
        
        // Èç¹û·¢ÏÖÂú×ãÌõ¼þµÄÄ§·¨ÊôÐÔ£¬¾ÍÉèÖÃ
        int nLuckyItemIndex = GetRandomNumber(0, nSelectedCount - 1);
        KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(
             SelectedMagicTable.Get(nLuckyItemIndex)
        );

        // ±£´æÂú×ãÌõ¼þµÄÄ§·¨ÊôÐÔ£¬ÒÔ±ãÔÚ×îºóµÄÊ±ºò½«Ê¹ÓÃ±êÖ¾Çå³ý
        pMagicAttrTable[i] = pMAItem;

        _ASSERT(pMAItem);
        _ASSERT(!(pMAItem->m_nUseFlag));     // È·¶¨»¹Ã»ÓÐÊ¹ÓÃ

        // ÉèÖÃÊ¹ÓÃ±êÖ¾
        pMAItem->m_nUseFlag = true;
		
        pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;
        pnaryMA[i].nValue[0] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[0].nMin, 
            pMAItem->m_MagicAttrib.aryRange[0].nMax
        );
        pnaryMA[i].nValue[1] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[1].nMin, 
            pMAItem->m_MagicAttrib.aryRange[1].nMax
        );
        pnaryMA[i].nValue[2] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[2].nMin, 
            pMAItem->m_MagicAttrib.aryRange[2].nMax
        );
		g_DebugLog(
			"[GenMagicAttrib] pnaryMA[%d]: (%d:%s), nAttribType = %d, nValue[0] = %d, nValue[1] = %d, nValue[2] = %d,", 
			i,
			SelectedMagicTable.Get(nLuckyItemIndex),
			pMAItem->m_szName,
			pnaryMA[i].nAttribType,
			pnaryMA[i].nValue[0],
			pnaryMA[i].nValue[1],
			pnaryMA[i].nValue[2]
		);

    } // for ±éÀúÐèÒªÈ¡»ØµÄÄ§·¨ÊôÐÔ±í

    for (NULL; i < 6; i++)
    {
        // Çå¿ÕÊ£ÏÂµÄÏî
        pnaryMA[i].nAttribType = 0;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
    }

    for (i = 0; i < 6; i++)
    {
        if (!pMagicAttrTable[i])
            break;

        _ASSERT(pMagicAttrTable[i]->m_nUseFlag);   // È·¶¨ÒÑ¾­Ê¹ÓÃ

        pMagicAttrTable[i]->m_nUseFlag = false;    // ÉèÖÃÎªÃ»ÓÃÊ¹ÓÃ
    }

    nResult = true;
//Exit0:
	return nResult;
}

// Following Code is implement by DongBo
//BOOL KItemGenerator::Gen_MagicAttrib(int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
//									 KItemNormalAttrib* pnaryMA)
//{
//	
//    // È·¶¨ÊÊÓÃÓÚ¸Ã×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔ¼°ÆäÊýÁ¿
//	if (FALSE == GMA_GetAvaliableMA(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// È·¶¨ÉÏÊö¸÷Ä§·¨ÊôÐÔµÄµÈ¼¶,³öÏÖ¸ÅÂÊ
//	if (FALSE == GMA_GetLevelAndDropRate(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// ´ÓÉÏÊöÄ§·¨ÊôÐÔÖÐÑ¡³ö·ûºÏÌõ¼þµÄ. ²ß»®ÒªÇóÑ¡³ö6¸ö
//	for (int i = 0; i < 6; i++)				// ÆæÊý: Ç°×º, Å¼Êý: ºó×º
//	{
//		GMA_ChooseMA((i&1), pnaryMALevel[i], nLucky, &(pnaryMA[i]));
//		if (pnaryMA[i].nAttribType == 0)
//			break;
//	}
//	return TRUE;
//}

/******************************************************************************
	¹¦ÄÜ:	È·¶¨ÊÊÓÃÓÚÖ¸¶¨×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔ¼°ÆäÊýÁ¿
	Èë¿Ú:	nType: Ö¸¶¨×°±¸
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã. m_GMA_naryMA[0]¸ø³öÊÊÓÃÓÚ¸Ã×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔÇ°×º
							m_GMA_nCount[0]¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ×ÜÊý
							m_GMA_naryMA[1]¸ø³öÊÊÓÃÓÚ¸Ã×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔºó×º
							m_GMA_nCount[1]¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ×ÜÊý
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::GMA_GetAvaliableMA(int nType)
{
	//TODO: ¿ÉÄÜÐèÒª¶ÔnType½øÐÐ×ª»», ´Ó×°±¸ÀàÐÍÖµ±ä³É m_BPTLib.m_CMAT Êý×éµÄË÷Òý
	for (int i = 0; i < 2; i++)		// 0: Ç°×º, 1: ºó×º
	{
		const KBPT_ClassifiedMAT* pCMAT;
		pCMAT = m_BPTLib.GetCMAT(i, nType);
		if (pCMAT == NULL)
			{ _ASSERT(FALSE); return FALSE; }
		m_GMA_nCount[i] = NUMOFCMA;
		pCMAT->GetAll((int*)m_GMA_naryMA[i], &(m_GMA_nCount[i]));
	}
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ:	È·¶¨¸÷¸ø¶¨Ä§·¨ÊôÐÔµÄµÈ¼¶ÒªÇóºÍ³öÏÖ¸ÅÂÊ
	Èë¿Ú:	nType: Ö¸¶¨×°±¸
			m_GMA_naryMAÊý×é: ¸ø³öÈ«²¿Ä§·¨ÊôÐÔµÄË÷ÒýÖµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã. m_GMA_naryLevelÊý×é¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
							m_GMA_naryDropRate¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ³öÏÖ¸ÅÂÊ
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::GMA_GetLevelAndDropRate(int nType)
{
	//TODO: ¿ÉÄÜÐèÒª¶ÔnType½øÐÐ×ª»», ´Ó×°±¸ÀàÐÍÖµ±ä³É m_BPTLib.m_CMAT Êý×éµÄË÷Òý

	for (int i = 0; i < 2; i++)		// 0: Ç°×º, 1: ºó×º
	{
		for (int n = 0; n < m_GMA_nCount[i]; n++)
		{
			const KMAGICATTRIB_TABFILE* pMATF;
			pMATF = GetMARecord(m_GMA_naryMA[i][n]);
			if (pMATF == NULL)
				{ _ASSERT(FALSE); return FALSE; }
			m_GMA_naryLevel[i][n] = pMATF->m_nLevel;
			m_GMA_naryDropRate[i][n] = pMATF->m_DropRate[nType];		// ¿ÉÓÅ»¯
		}
	}
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ:	´ÓÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔÖÐÑ¡³ö·ûºÏÌõ¼þµÄÄ§·¨
	Èë¿Ú:	nPos: 0: Ñ¡Ç°×º, 1: Ñ¡ºó×º
			nLevel: ¶ÔÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
			nLucky: ÔËÆøÖµ
			m_GMA_naryMAÊý×é: ¸ø³öÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔµÄË÷ÒýÖµ
	³ö¿Ú:	*pINA ¸ø³öËùÑ¡Ä§·¨µÄºËÐÄ²ÎÊý
******************************************************************************/
void KItemGenerator::GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA)
{
	_ASSERT(pINA != NULL);

	// ¸³³õÖµ
	pINA->nAttribType = 0;
	pINA->nValue[0] = 0;
	pINA->nValue[1] = 0;
	pINA->nValue[2] = 0;

	// É¸Ñ¡³öÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔ
	int nDropRate = ::GetRandomNumber(0, 100) - nLucky;
	int nCount = GMA_GetCandidateMA(nPos, nLevel, nDropRate);
	if (nCount > 0)
	{
		int nFinal = ::GetRandomNumber(0, nCount);
		int nMAi = m_GMA_naryCandidateMA[nFinal];
		int nMA = m_GMA_naryMA[nPos][nMAi];
		if (nMA == -1)	// Ëæ»ú³öÖØ¸´µÄÊôÐÔÁË
		{
			pINA->nAttribType = 0;
			return;
		}
		m_GMA_naryMA[nPos][nMAi] = -1;		// ÖÃÎª-1,±íÊ¾¸ÃMAÒÑÓÃ
		const KMAGICATTRIB_TABFILE* pMATF = GetMARecord(nMA);
		if (NULL != pMATF)
		{
			const KMACP* pMACP = &(pMATF->m_MagicAttrib);
			pINA->nAttribType = pMACP->nPropKind;
			pINA->nValue[0] = ::GetRandomNumber(pMACP->aryRange[0].nMin, pMACP->aryRange[0].nMax);
			pINA->nValue[1] = ::GetRandomNumber(pMACP->aryRange[1].nMin, pMACP->aryRange[1].nMax);
			pINA->nValue[2] = ::GetRandomNumber(pMACP->aryRange[2].nMin, pMACP->aryRange[2].nMax);
		}
	}
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÄ§·¨ÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	i: Ö¸¶¨¼ÇÂ¼
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KMAGICATTRIB_TABFILE* KItemGenerator::GetMARecord(int i) const
{
	return m_BPTLib.GetMARecord(i);
}

/******************************************************************************
	¹¦ÄÜ:	´ÓÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔÖÐÑ¡³ö·ûºÏÌõ¼þµÄÄ§·¨
	Èë¿Ú:	nPos: 0: Ñ¡Ç°×º, 1: Ñ¡ºó×º
			nLevel: ¶ÔÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
			nDropRate: Ñ¡³ö³öÏÖ¸ÅÂÊ´óÓÚ´ËÖµµÄÄ§·¨ÊôÐÔ
			m_GMA_naryMAÊý×é: ¸ø³öÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔµÄË÷ÒýÖµ
			m_GMA_naryLevelÊý×é: ¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
			m_GMA_naryDropRateÊý×é: ¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ³öÏÖ¸ÅÂÊ
	³ö¿Ú:	m_GMA_naryCandidateMAÊý×é¸ø³ö·ûºÏÌõ¼þµÄÄ§·¨
	ËµÃ÷:	Èô n = m_GMA_naryMA[nPos][m_GMA_naryCandidateMA[i]]
			Ôò GetMARecord(n) ·µ»ØÖ¸ÏòÄ§·¨ÊôÐÔ½á¹¹µÄÖ¸Õë
******************************************************************************/
int KItemGenerator::GMA_GetCandidateMA(int nPos, int nLevel, int nDropRate)
{
	int nCount = 0;
	for (int i = 0; i < m_GMA_nCount[nPos]; i++)
	{
		if (m_GMA_naryMA[nPos][i] != -1 &&		// -1±íÊ¾¸ÃMAÒÑÓÃ¹ý
			m_GMA_naryLevel[nPos][i] == nLevel &&
			m_GMA_naryDropRate[nPos][i] >= nDropRate)
		{
			m_GMA_naryCandidateMA[nCount++] = i;
		}
	}
	return nCount;
}

BOOL KItemGenerator::GetMedicineCommonAttrib(IN int nDetailType, IN int nLevel, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
		KASSERT(FALSE);
		return FALSE;
	}

	const int i = nDetailType * 5 + nLevel - 1;

	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		_ASSERT(FALSE);
		return FALSE;
	}
	*pItem = *pMed;
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ£º	Ëæ»úÉú³É»Æ½ð×°±¸×°±¸
	Èë¿Ú:	nCondition: Ç°ÌáÌõ¼þ£¬(Emf+Pmf)*LevelµÄÔËËã½á¹û
			pItem: ×°±¸ÏîÄ¿Ö¸Õë¡£
	³ö¿Ú:	³É¹¦Ê±·µ»ØÕæ, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»Ø¼Ù
******************************************************************************/
// Add by flying on May.30.2003
// Try to get a "Gold Item" if possible.
BOOL KItemGenerator::GetGoldItemByRandom(IN int nCondition, OUT KItem * pItem)
{
	BOOL bResult = FALSE;
	const KBASICPROP_EQUIPMENT_GOLD* pArray[MAX_ITEM];
	int i = 0;
	int j = 0;
	int nIndex = 0;

	::memset(pArray, 0, sizeof(KBASICPROP_EQUIPMENT_GOLD *) * MAX_ITEM);

	for (i = 0; i < MAX_ITEM; i++)
	{
		const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(i);
		if (pTemp == 0)
			break;
		if (pTemp->m_nRarity < nCondition)
			pArray[j] = pTemp;
	}

	if (j == 0)
		goto Exit0;			 
	nIndex = GetRandomNumber(0, j);	
	*pItem = *pArray[nIndex];
Exit0:
	return FALSE;
}

/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉÖ¸¶¨»Æ½ð×°±¸×°±¸
	Èë¿Ú:	nCondition: Ç°ÌáÌõ¼þ£¬(Emf+Pmf)*LevelµÄÔËËã½á¹û
			pItem: ×°±¸ÏîÄ¿Ö¸Õë¡£
	³ö¿Ú:	³É¹¦Ê±·µ»ØÕæ, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»Ø¼Ù
******************************************************************************/
// Add by flying on May.30.2003
// Try to get a "Gold Item" if possible.
BOOL KItemGenerator::GetGoldItemByIndex(IN int nIndex, OUT KItem * pItem)
{
	BOOL bResult = FALSE;
	int i = 0;
	int j = 0;
	int nCount = 0;

	nCount = m_BPTLib.GetGoldItemNumber();
	const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(i);
	if (nIndex >= nCount)
		goto Exit0;
	if (pTemp == NULL)
		goto Exit0;
	*pItem = *pTemp;
	bResult = TRUE;
Exit0:
	return bResult;
}

BOOL KItemGenerator::GetEquipmentCommonAttrib(IN int nDetailType, IN int nParticularType, IN int nLevel, IN int nSeries, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
		KASSERT(FALSE);
		return FALSE;
	}

	const int i = nParticularType * 10 + nLevel - 1;// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
												// Ïê¼ûSPE 2002Äê8ÔÂ31ÈÕ7:40
												// email: »ù´¡µÀ¾ßÊýÖµ±í
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	default:
		break;
	}
	if (NULL == pEqu)
		{ _ASSERT(FALSE); return FALSE; }
		// ÔËÐÐÖÁ´ËµÄÖ±½ÓÔ­Òò: Ö»ÓÐnÖÖ×°±¸, ¶øÉÏÃæ i µÄÖµÔÚ[0,n-1]Ö®Íâ
		// ¼ì²é3µã: nParticularType ÓÐÎó?
		//			nLevel ÓÐÎó?
		//			Ô­Ê¼µÄtab fileÓÐÎóµ¼ÖÂ m_BPTLib.m_BPTEquipment Ëù
		//			¹ÜÀíµÄÊý¾ÝÓÐÎÊÌâ?
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeries);
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉËæ»ú×°±¸
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð, Èç½ü³ÌÎäÆ÷¡¢°µÆ÷¡¢......
			nParticularType: ÏêÏ¸Àà±ð
			nSeriesReq: ÎåÐÐÊôÐÔ
			nLevel: µÈ¼¶
			pnaryMALevel: Ä§·¨ÊôÐÔµÈ¼¶Êý×é[6]
			nLucky: ÔËÆøÖµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_ExistEquipment(IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeriesReq,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;

	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	// ¸ù¾ÝÈë¿Ú²ÎÊý, È·¶¨×°±¸µÄ»ù±¾Êý¾Ý
	const int i = nParticularType * 10 + nLevel - 1;// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
												// Ïê¼ûSPE 2002Äê8ÔÂ31ÈÕ7:40
												// email: »ù´¡µÀ¾ßÊýÖµ±í
												// µÈ¼¶Êý¾Ý´Ó1¿ªÊ¼£¬ËùÒÔÒª¼õ»ØÀ´
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	default:
		break;
	}
	if (NULL == pEqu)
		{ _ASSERT(FALSE); return bEC; }
		// ÔËÐÐÖÁ´ËµÄÖ±½ÓÔ­Òò: Ö»ÓÐnÖÖ×°±¸, ¶øÉÏÃæ i µÄÖµÔÚ[0,n-1]Ö®Íâ
		// ¼ì²é3µã: nParticularType ÓÐÎó?
		//			nLevel ÓÐÎó?
		//			Ô­Ê¼µÄtab fileÓÐÎóµ¼ÖÂ m_BPTLib.m_BPTEquipment Ëù
		//			¹ÜÀíµÄÊý¾ÝÓÐÎÊÌâ?
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeriesReq);

	if (NULL == pnaryMALevel)
		return TRUE;
	// È·¶¨×°±¸µÄÄ§·¨ÊôÐÔ
	KItemNormalAttrib	sMA[6];	// µÀ¾ßµÄÄ§·¨ÊôÐÔ
	bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA);
	if (bEC)
		pItem->SetAttrib_MA(sMA);

	return bEC;
}

BOOL KItemGenerator::Gen_Quest(IN int nDetailType, IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	const KBASICPROP_QUEST* pQuest = NULL;
	pQuest = m_BPTLib.GetQuestRecord(nDetailType);
	if (NULL == pQuest)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;
	return TRUE;
}

BOOL KItemGenerator::Gen_TownPortal(IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	const KBASICPROP_TOWNPORTAL* pPortal = NULL;
	pPortal = m_BPTLib.GetTownPortalRecord(0);
	if (NULL == pPortal)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pPortal;
	return TRUE;
}
