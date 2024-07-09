#include "../include/HealthRegeneration.h"
#include "../include/Player.h"

HealthRegeneration::HealthRegeneration(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/heal.png")))
{
}

void HealthRegeneration::apply_effect(const std::shared_ptr<Player>& player) const
{
	if (player->get_health() == 3u)
		m_point = 500u;
	else
		m_point = 250u;
	player->heal();
}
