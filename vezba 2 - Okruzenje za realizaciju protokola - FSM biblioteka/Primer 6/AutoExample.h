#ifndef _AUTOMAT_H_
#define _AUTOMAT_H_

#include <fsm.h>
#include <fsmsystem.h>
#include "Constants.h"

#define TIMER1_COUNT 5 

#include "../kernel/stdMsgpc16pl16.h"
typedef stdMsg_pc16_pl16 StandardMessage;

class AutoExample : public FiniteStateMachine {
	enum AutoExampleStates { AUTO_STATE0, AUTO_STATE1, AUTO_STATE2 };

	StandardMessage StandardMsgCoding;

	/* FiniteStateMachine abstract functions */
	MessageInterface *GetMessageInterface(uint32 id);
	void	SetDefaultHeader(uint8 infoCoding);
	void	SetDefaultFSMData();
	void	NoFreeInstances();
	uint8	GetMbxId();
	uint8	GetAutomate();

public:
	AutoExample();
	~AutoExample();
	
	void Initialize();
	void Start();
	void S0_ChangeState();
	void S1_ChangeState();
};

#endif /* _AUTOMAT_H */