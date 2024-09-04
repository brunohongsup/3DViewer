#pragma once

class CSequenceMain : public Ravid::Framework::CSequenceInfo
{
public:
	CSequenceMain();
	virtual ~CSequenceMain();

	enum EStep
	{
		EStep_Idle = 0,
		EStep_Ready,
		EStep_Wait,
		EStep_DoSomething,
		EStep_End,
	};

protected:

	virtual void OnStart() { };
	virtual void OnPause() { };
	virtual void OnStop() { };

	virtual bool Work();

	EStep m_eStep = EStep_Idle;
};

