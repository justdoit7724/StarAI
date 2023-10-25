#pragma once
#include "Theme.h"


class Controller;
class Goal;

class Brain
{
private:
	Theme* m_mainTheme;

	template <class T>
	void ChangTheme()
	{
		if (m_mainTheme)
		{
			if (T::ID() != m_mainTheme->ID)
			{
				delete m_mainTheme;

				m_mainTheme = new T();
			}


		}
		else
		{
			m_mainTheme = new T();
		}
		
	}

public:
	Brain();
	void Update( const Controller* con);

};

