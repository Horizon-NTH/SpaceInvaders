#include "../include/FullShieldRegeneration.h"
#include "../include/Player.h"

FullShieldRegeneration::FullShieldRegeneration(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/full_shield.png")))
{
}

void FullShieldRegeneration::apply_effect(const std::shared_ptr<Player>& player) const
{
	if (player->get_shield() == 3u)
		m_point = 1000u;
	else
		m_point = 500u;
	player->shield_up(true);
}
