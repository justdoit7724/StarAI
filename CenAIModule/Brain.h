#pragma once

class Controller;
class Theme;
class Goal;

class Brain
{
private:
	Theme* m_mainTheme;

public:
	Brain();
	void Update( const Controller* con);

};

