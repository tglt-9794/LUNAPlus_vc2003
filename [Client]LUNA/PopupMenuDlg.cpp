#include "stdafx.h"
#include "popupmenudlg.h"
#include "WindowIDEnum.h"
#include "cStatic.h"
#include "cButton.h"
#include "../input/Mouse.h"
#include "partymanager.h"
#include "guildmanager.h"
#include "objectmanager.h"
#include "conductmanager.h"
#include "gamein.h"
#include "guilddialog.h"
#include "familydialog.h"
#include "MiniFriendDialog.h"
#include "friendmanager.h"
#include "chattingdlg.h"
#include "cEditBox.h"
#include "input/UserInput.h"

#include "cHousingMgr.h"
#include "..\hseos\Family\SHFamily.h"
#include "../ChatManager.h"

#include "cMsgBox.h"
#include "../interface/cWindowManager.h"
#include "ObjectStateManager.h"

#include "../DateMatchingDlg.h"

const DWORD enableColor = RGB( 255, 255, 255 );
const DWORD unableColor = RGB( 120, 100, 100 );

CPopupMenuDlg::CPopupMenuDlg(void)
{}

CPopupMenuDlg::~CPopupMenuDlg(void)
{
}

void CPopupMenuDlg::Linking()
{
	mFace =		( cStatic* )GetWindowForID( ID_POPUPMENU_STATIC_CHARACTER_FACE );
	mName =		( cStatic* )GetWindowForID( ID_POPUPMENU_STATIC_CHARACTER_NAME );

	mWhisper =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_WHISPER );
	mTrade =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_TRADE );
	mFallow =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_FALLOW );
	mFriend =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_FRIEND );
	mParty =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_PARTY );
	mGuild =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_GUILD );
	mFamily =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_FAMILY );
	mVersus =	( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_VERSUS );

	//090615 pdy �Ͽ�¡ ĳ���� ��Ŭ�� ��ũ �Ͽ콺���� ��� �߰�
	mVisitHouse = ( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_HOUSE );
	mQuickDate = ( cButton* )GetWindowForID( ID_POPUPMENU_BUTTON_QUICKDATE );

	mPCRoomDlgOpen = FALSE;
}

void CPopupMenuDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
	if( we & WE_BTNCLICK )																											
	{
		//100118 ONS ģ������ ���̾�α׿� PC���������� ����߰�
		if( mPCRoomDlgOpen )
		{
 			switch( lId )																												
			{
			case ID_POPUPMENU_BUTTON_WHISPER:
				{
					char buf[256] = "/";
					strcat( buf, mSelectedName );
					strcat( buf, " " );
					GAMEIN->GetChattingDlg()->GetInputBox()->SetEditText( buf );
					GAMEIN->GetChattingDlg()->GetInputBox()->SetFocusEdit( TRUE );
				}
				break;
			case ID_POPUPMENU_BUTTON_FRIEND:
				{
					FRIENDMGR->AddDelFriendSyn( mSelectedName, MP_FRIEND_ADD_SYN);
					GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
				}
				break;
			case ID_POPUPMENU_BUTTON_PARTY:
				{
					if( !HERO->GetPartyIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) ) ;
					}
					else
					{
						// ��Ƽ �ʴ� ó���� �Ѵ�.
						if( !mSelectedIndex )
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1043)) ;
						}
						else
						{
							// �۾����ѽð� Ȯ��
							const DWORD dwLastTime = HERO->GetInvitePartyByNameLastTime();
							const DWORD dwLimitTime = HERO->GetInvitePartyByNameLimitTime();

							// �۾� ���ɽð��� ������� �������
							if( dwLastTime != 0 &&
								gCurTime < dwLastTime + dwLimitTime )
							{
								float fRemainTime = float( dwLimitTime - (gCurTime - dwLastTime) ) / 1000.0f;
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1923 ), fRemainTime );
								return;
							}

							// �۾� ���ѽð� ����
							HERO->SetInvitePartyByNameLastTime( 0 );
							HERO->SetInvitePartyByNameLimitTime( 0 );

							if( strlen( mSelectedName ) <= 0 )
								break;

							// �ڱ��ڽ��� �̸��� ���� ���(��ҹ��� ���о��� ��)
							if( stricmp( HERO->GetObjectName(), mSelectedName ) == 0 )
							{
								CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1269 ) );
								SetActive( FALSE );
								break;
							}

							// ��Ƽ �ʴ� �޼����� ������
							PARTYMGR->InvitePartyMemberByNameSyn( mSelectedName );
						}
					}
				}
				break;
			}
		}
		else
		{
 			switch( lId )																												
			{
			case ID_POPUPMENU_BUTTON_WHISPER:
				{
					char buf[256] = "/";
					strcat( buf, OBJECTMGR->GetSelectedObject()->GetObjectName() );
					strcat( buf, " " );
					GAMEIN->GetChattingDlg()->GetInputBox()->SetEditText( buf );
					GAMEIN->GetChattingDlg()->GetInputBox()->SetFocusEdit( TRUE );
				}
				break;
			case ID_POPUPMENU_BUTTON_TRADE:
				{
					CONDUCTMGR->LifeSkillCommand( 3 );
				}
				break;
			case ID_POPUPMENU_BUTTON_FALLOW:
				{
					CONDUCTMGR->LifeSkillCommand( 66 );
				}
				break;
			case ID_POPUPMENU_BUTTON_FRIEND:
				{
					FRIENDMGR->AddDelFriendSyn( OBJECTMGR->GetSelectedObject()->GetObjectName(), MP_FRIEND_ADD_SYN);
					GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
				}
				break;
			case ID_POPUPMENU_BUTTON_PARTY:
				{
					// 090204 LYW --- PopupMenuDlg : ��Ƽ �ʴ� ���� ó���� �Ѵ�.
					//CONDUCTMGR->LifeSkillCommand( 2 );
					if( !HERO->GetPartyIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) ) ;
					}
					else
					{
						// ��Ƽ �ʴ� ó���� �Ѵ�.
						DWORD dwObjectID = 0 ;
						CObject* pObject = OBJECTMGR->GetSelectedObject() ;
						if( !pObject )
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1043)) ;
						}
						else
						{
							if( pObject->GetObjectKind() != eObjectKind_Player )
							{
								CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1043)) ;
							}
							else
							{
								dwObjectID = pObject->GetID() ;
								PARTYMGR->AddPartyMemberSyn( dwObjectID ) ;
							}
						}
					}
				}
				break;
			case ID_POPUPMENU_BUTTON_GUILD:
				{
					GAMEIN->GetGuildDlg()->OnActionEvent( GD_INVITEMEMBER, NULL, WE_BTNCLICK );
				}
				break;
			case ID_POPUPMENU_BUTTON_FAMILY:
				{
					GAMEIN->GetFamilyDlg()->OnActionEvent( FMY_INVITE, NULL, WE_BTNCLICK );
				}
				break;
			case ID_POPUPMENU_BUTTON_VERSUS:
				{
					CONDUCTMGR->LifeSkillCommand( 5 );
				}
				break;
			case ID_POPUPMENU_BUTTON_HOUSE:
				{
					//090618 pdy �Ͽ�¡ �ý��۸޼��� ����Ұ� ����[��Ŭ�� ����]
					if( HERO->GetState() != eObjectState_None || HERO->IsPKMode() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(789) ) ;
						break ;
					}

					char MsgBuf[256] = {0,};
					if( HOUSINGMGR->IsHousingMap() )
					{
						BASEOBJECT_INFO DestObjInfo;	
						OBJECTMGR->GetSelectedObject()->GetBaseObjectInfo(&DestObjInfo);

						if( HOUSINGMGR->IsHouseOwnerID(DestObjInfo.dwUserID) )
						{
							CHATMGR->AddMsg(CTC_SYSMSG,  CHATMGR->GetChatMsg(1904) ); 
							break;
						}
					}

					//090615 pdy �Ͽ�¡ ĳ���� ��Ŭ�� ��ũ �Ͽ콺���� ��� �߰�
					sprintf(MsgBuf,CHATMGR->GetChatMsg(1890), OBJECTMGR->GetSelectedObject()->GetObjectName() ); //1890	"%s�� ������ �̵� �Ͻðڽ��ϱ�?"

					//090615 pdy �Ͽ�¡ �˾�â [ĳ���� ��Ŭ�� ��ũ����] 
					cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_VISIT_LINK_PLAYER_RCLICK, MBT_YESNO, MsgBuf ) ;	
					pBox->SetParam( OBJECTMGR->GetSelectedObject()->GetID() );

					OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal) ;
				}
				break;
			case ID_POPUPMENU_BUTTON_QUICKDATE:
				{
					BASEOBJECT_INFO DestObjInfo;	
					OBJECTMGR->GetSelectedObject()->GetBaseObjectInfo(&DestObjInfo);

					if(DestObjInfo.dwObjectID && DestObjInfo.dwUserID)
					{
						if(! GAMEIN->GetDateMatchingDlg()->IsActive())
						{
							GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->SettingControls();
							GAMEIN->GetDateMatchingDlg()->SetActive(TRUE) ;
						}

						GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->OpenDlgForQuickDate(DestObjInfo.dwObjectID);
					}
				}
				break;
			}
		}

		SetActive( FALSE );
	}
}

void CPopupMenuDlg::SetActive(BOOL val)
{
	cDialog::SetActive( val );

	if( val )
	{
		CObject* pObject = OBJECTMGR->GetSelectedObject();

		if( !pObject )
		{
			SetActive( FALSE );
			return;
		}

		mName->SetStaticText( pObject->GetObjectName() );

		if( HERO->GetPartyIdx() )
		{
			mParty->SetDisable( FALSE );
			mParty->SetImageRGB( enableColor );
			mParty->SetBasicColor( enableColor );
		}
		else
		{
			mParty->SetDisable( TRUE );
			mParty->SetImageRGB( unableColor );
			mParty->SetBasicColor( unableColor );
		}

		if( HERO->GetGuildIdx() && ( HERO->GetGuildMemberRank() == GUILD_MASTER || HERO->GetGuildMemberRank() == GUILD_VICEMASTER ) )
		{
			mGuild->SetDisable( FALSE );
			mGuild->SetImageRGB( enableColor );
		}
		else
		{
			mGuild->SetDisable( TRUE );
			mGuild->SetImageRGB( unableColor );
			mGuild->SetBasicColor( unableColor );
		}
		
		// 090204 LYW --- PopupMenuDlg : �йи� ��ȿ���� üũ�Ѵ�.
		CSHFamily* pFamily = HERO->GetFamily() ;
		if( pFamily )
		{
			if( pFamily->Get()->nMasterID == HEROID )
			{
				mFamily->SetDisable( FALSE );
				mFamily->SetImageRGB( enableColor );
			}
			else
			{
				mFamily->SetDisable( TRUE );
				mFamily->SetImageRGB( unableColor );
				mFamily->SetBasicColor( unableColor );
			}
		}
		else
		{
			mFamily->SetDisable( TRUE );
			mFamily->SetImageRGB( unableColor );
			mFamily->SetBasicColor( unableColor );
		}

		// 100217 ONS PC���� ����Ʈ�� �˾��޴��߰��� ���� ó��
		mTrade->SetDisable( FALSE );
		mTrade->SetImageRGB( enableColor );
		mTrade->SetBasicColor( enableColor );

		mFallow->SetDisable( FALSE );
		mFallow->SetImageRGB( enableColor );
		mFallow->SetBasicColor( enableColor );

		mVersus->SetDisable( FALSE );
		mVersus->SetImageRGB( enableColor );
		mVersus->SetBasicColor( enableColor );
	}
	else
	{
		// 100217 ONS PC���� ����Ʈ�� �˾��޴��߰��� ���� ó��
		if(mPCRoomDlgOpen)
			mPCRoomDlgOpen = FALSE;
	}
}

//100118 ONS ģ������ ���̾�α׿� PC���������� ����߰�
void CPopupMenuDlg::SetActivePCRoomDlg(DWORD dwIndex, const char* pObjectName, BOOL val)
{
	cDialog::SetActive( val );

	if( val )
	{
		mPCRoomDlgOpen = TRUE;
		mSelectedIndex = dwIndex;
		SafeStrCpy(mSelectedName, pObjectName, sizeof(mSelectedName));

		mName->SetStaticText( pObjectName );

		mTrade->SetDisable( TRUE );
		mTrade->SetImageRGB( unableColor );
		mTrade->SetBasicColor( unableColor );

		mFallow->SetDisable( TRUE );
		mFallow->SetImageRGB( unableColor );
		mFallow->SetBasicColor( unableColor );

		mGuild->SetDisable( TRUE );
		mGuild->SetImageRGB( unableColor );
		mGuild->SetBasicColor( unableColor );

		mFamily->SetDisable( TRUE );
		mFamily->SetImageRGB( unableColor );
		mFamily->SetBasicColor( unableColor );

		mVersus->SetDisable( TRUE );
		mVersus->SetImageRGB( unableColor );
		mVersus->SetBasicColor( unableColor );

		if( HERO->GetPartyIdx() )
		{
			mParty->SetDisable( FALSE );
			mParty->SetImageRGB( enableColor );
			mParty->SetBasicColor( enableColor );
		}
		else
		{
			mParty->SetDisable( TRUE );
			mParty->SetImageRGB( unableColor );
			mParty->SetBasicColor( unableColor );
		}
	}
}

DWORD CPopupMenuDlg::ActionEvent(CMouse *mouseInfo)
{
	DWORD rt = cDialog::ActionEvent(mouseInfo) ;
	
	if( !rt && ( mouseInfo->LButtonDown() || mouseInfo->RButtonDown() || mouseInfo->MButtonDown() ) )
	{
		SetActive( FALSE );
	}

	return rt;
}