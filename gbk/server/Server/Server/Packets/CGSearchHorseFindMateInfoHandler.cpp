/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "CGSearchHorseFindMateInfo.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "HorseFindMateModule.h"

namespace Packets
{

uint	CGSearchHorseFindMateInfoHandler::Execute(CGSearchHorseFindMateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	
	HorseFindMateModule::GetInstance().SearchHorseFindMateInfo(pHuman, pPacket->GetName(),
		pPacket->GetHorseLevel(), pPacket->GetHorseSex(), pPacket->GetHorseGrowRate(), pPacket->GetMaxPostion() );

	g_pLog->FastSaveLog(LOG_FILE_1, "CGSearchHorseFindMateInfoHandler: ok obj=%d,scene=%d",
		pHuman->GetID(), pScene->SceneID());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
