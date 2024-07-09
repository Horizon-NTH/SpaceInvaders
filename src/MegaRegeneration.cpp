#include "../include/MegaRegeneration.h"
#include "../include/Player.h"

MegaRegeneration::MegaRegeneration(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/mega_heal.png")))
{
}

void MegaRegeneration::apply_effect(const std::shared_ptr<Player>& player) const
{
	m_point = 1000u;
	if (player->get_health() == 3u)
		m_point += 1000u;
	if (player->get_shield() == 3u)
		m_point += 1000u;
	player->heal(true);
	player->shield_up(true);
}
