#include "stdafx.h"
#include "SequenceMain.h"

CSequenceMain::CSequenceMain()
{
	SetStep(EStep_Idle);
}

CSequenceMain::~CSequenceMain()
{
}

bool CSequenceMain::Work()
{
	switch(GetStep())
	{
	case EStep_Idle:
		{
			SetStep(EStep_Ready);
		}
		break;

	case EStep_Ready:
		{
			SetStep(EStep_Wait);
		}
		break;

	case EStep_Wait:
		{
			SetStep(EStep_DoSomething);
		}
		break;

	case EStep_DoSomething:
		{
			SetStep(EStep_End);
		}
		break;

	case EStep_End:
		{
			SetStep(EStep_Wait);
		}
		break;
	}

	return true;
}
