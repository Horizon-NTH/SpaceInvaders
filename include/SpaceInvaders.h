#pragma once

#include "Include.h"
#include "Wave.h"

class Player;

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
	std::shared_ptr<hgui::kernel::AnimatedSprite> m_gameOver;
	std::string m_backgroundMovingTaskId;

	std::shared_ptr<Player> m_player;
	std::unique_ptr<Wave> m_wave;
	bool m_isSfx;
	bool m_isMusic;

	void move_background();
	void set_main_menu();
	void set_death_menu();
	void start_game();
};
