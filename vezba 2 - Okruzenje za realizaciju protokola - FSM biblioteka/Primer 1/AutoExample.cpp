#include <stdio.h>
#include "AutoExample.h"

AutoExample::AutoExample() : FiniteStateMachine( AUTOEXAMPLE_FSM, AUTOEXAMPLE_MBX_ID,10, 10, 10) {
}

AutoExample::~AutoExample() {
}


uint8 AutoExample::GetAutomate() {
	return AUTOEXAMPLE_FSM;
}

/* This function actually connnects the AutoExamplee with the mailbox. */
uint8 AutoExample::GetMbxId() {
	return AUTOEXAMPLE_MBX_ID;
}

MessageInterface *AutoExample::GetMessageInterface(uint32 id) {
  if(id == 0) 
	  return &StandardMsgCoding;
  throw TErrorObject( __LINE__, __FILE__, 0x01010400);
}

void AutoExample::SetDefaultHeader(uint8 infoCoding) {
	SetMsgInfoCoding(infoCoding);
	SetMessageFromData();
}

void AutoExample::SetDefaultFSMData() {

}

void AutoExample::NoFreeInstances() {
	printf("[%d] AutoExample::NoFreeInstances()\n", GetObjectId());
}

void AutoExample::Initialize() {
	SetState(AUTO_STATE0);
	SetDefaultFSMData();
	InitEventProc(AUTO_STATE0,MSG_CHANGE_STATE,(PROC_FUN_PTR)&AutoExample::S0_ChangeState);
	InitEventProc(AUTO_STATE0,TIMER1_EXPIRED,(PROC_FUN_PTR)&AutoExample::S1_ChangeState);

	InitTimerBlock(TIMER1_ID, TIMER1_COUNT, TIMER1_EXPIRED);
	StartTimer(TIMER1_ID);
}

/* Initial system message */
void AutoExample::Start() {
	printf("AutoExample[%d]::Start() - send message !\n", GetObjectId());
	PrepareNewMessage(0x00,MSG_CHANGE_STATE);
	SetMsgToAutomate(AUTOEXAMPLE_FSM);
	SetMsgObjectNumberTo(GetObjectId()); // Salje sam sebi.
	SendMessage(AUTOEXAMPLE_MBX_ID);
}

void AutoExample::S0_ChangeState() {
	printf("AutoExample[%d]::S0_ChangeState() - receive message !\n", GetObjectId());
	SetState(AUTO_STATE1);
}

void AutoExample::S1_ChangeState() {
	printf("AutoExample[%d]::S1_ChangeState() - istekla je vremenska kontrola !\n", GetObjectId());
	SetState(AUTO_STATE1);
	StopTimer(TIMER1_ID);
}
