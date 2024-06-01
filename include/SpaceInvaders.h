#pragma once

#include "Include.h"
#include "SpaceShip.h"

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
	std::pair<std::shared_ptr<hgui::kernel::Texture>, std::shared_ptr<hgui::kernel::Texture>> m_sfx;
	std::pair<std::shared_ptr<hgui::kernel::Texture>, std::shared_ptr<hgui::kernel::Texture>> m_music;
	std::vector<std::shared_ptr<hgui::kernel::Label>> m_texts;
	std::vector<std::shared_ptr<hgui::kernel::Button>> m_buttons;
	std::string m_backgroundMovingTaskId;

	unsigned int m_wave;
	std::shared_ptr<SpaceShip> m_player;
	bool m_isSfx;
	bool m_isMusic;

	void move_background();
	void set_main_menu();
	void start_game();
};
