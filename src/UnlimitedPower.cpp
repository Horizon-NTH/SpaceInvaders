#include "../include/UnlimitedPower.h"
#include "../include/Player.h"

UnlimitedPower::UnlimitedPower(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/unlimited_power.png")))
{
}

void UnlimitedPower::apply_effect(const std::shared_ptr<Player>& player) const
{
	m_point = 3000u;
	if (player->get_health() == 3u)
		m_point += 1000u;
	if (player->get_shield() == 3u)
		m_point += 1000u;
	player->heal(true);
	player->shield_up(true);
	player->make_invincible(std::chrono::seconds(10));
}
