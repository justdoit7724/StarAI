#include "pch.h"
#include "Brain.h"
#include "TH_ZerglingBase.h"

Brain::Brain()
{
	m_mainTheme = new TH_ZerglingBase();
}

void Brain::Update(const Controller* con)
{
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

