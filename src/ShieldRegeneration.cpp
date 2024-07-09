#include "../include/ShieldRegeneration.h"
#include "../include/Player.h"

ShieldRegeneration::ShieldRegeneration(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/shield.png")))
{
}

void ShieldRegeneration::apply_effect(const std::shared_ptr<Player>& player) const
{
	if (player->get_shield() == 3u)
		m_point = 500u;
	else
		m_point = 250u;
	player->shield_up();
}
