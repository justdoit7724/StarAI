#include "pch.h"
#include "Brain.h"
#include "TH_ZerglingBase.h"
#include "TH_HydraBase.h"
#include "SituationManager.h"


Brain::Brain()
{
	m_mainTheme = new TH_ZerglingBase();
}

void Brain::Update(const Controller* con)
{
	if (SG_SITU.GetPL() < -500)
	{
		SG_SITU.ResetPL();

		ChangTheme<TH_HydraBase>();
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

