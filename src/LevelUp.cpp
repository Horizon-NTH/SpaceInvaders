#include "../include/LevelUp.h"
#include "../include/Player.h"

LevelUp::LevelUp(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/level_up.png")))
{
}

void LevelUp::apply_effect(const std::shared_ptr<Player>& player) const
{
	if (player->get_level() == 3u)
		m_point = 5000u;
	else
		m_point = 2000u;
	player->level_up();
}
