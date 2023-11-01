#include "pch.h"
#include "Brain.h"
#include "TH_FastAttBase.h"
#include "TH_ZerglingBase.h"
#include "TH_HydraBase.h"
#include "SituationManager.h"


Brain::Brain()
{
	m_mainTheme = new TH_FastAttBase();
	//m_mainTheme = new TH_HydraBase();
}

void Brain::Update(const Controller* con)
{
	if (m_mainTheme->IsFinished())
	{
		SG_SITU.ResetPL();

		auto str = m_mainTheme->ID;
		if (str == TH_FastAttBase::GetID())
		{
			ChangTheme<TH_ZerglingBase>();
		}
		else if (str == TH_ZerglingBase::GetID())
		{
			ChangTheme<TH_HydraBase>();
		}
	}


	m_mainTheme->Update(con);

	//zergling
	//hydra
	//lurker
	//mutal
	//High mutal
	//ultra

	//flyer
	//fight difference

	
}

