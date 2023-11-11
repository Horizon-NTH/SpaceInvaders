#pragma once

#include "Include.h"
#include "SpaceShip.h"
#include "Laser.h"

class SpaceInvaders
{
public:
	SpaceInvaders();
	
	void start();

private:
	std::shared_ptr<hgui::kernel::Window> m_window;
	std::shared_ptr<hgui::kernel::Monitor> m_monitor;
	std::shared_ptr<hgui::kernel::Font> m_font;

	std::pair<std::shared_ptr<hgui::kernel::Sprite>, std::shared_ptr<hgui::kernel::Sprite>> m_backgrounds;
	std::vector<std::shared_ptr<hgui::kernel::Label>> m_texts;

	unsigned int m_wave;
	std::shared_ptr<SpaceShip> m_player;

	void move_background();
	void set_main_menu();
	void start_game();
};

