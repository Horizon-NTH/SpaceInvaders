#include "../include/FullHealthRegeneration.h"
#include "../include/Player.h"

FullHealthRegeneration::FullHealthRegeneration(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/full_heal.png")))
{
}

void FullHealthRegeneration::apply_effect(const std::shared_ptr<Player>& player) const
{
	if (player->get_health() == 3u)
		m_point = 1000u;
	else
		m_point = 500u;
	player->heal(true);
}
