#include "../include/Invincibility.h"
#include "../include/Player.h"

Invincibility::Invincibility(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/invincibility.png")))
{
}

void Invincibility::apply_effect(const std::shared_ptr<Player>& player) const
{
	m_point = 2000u;
	player->make_invincible(std::chrono::seconds(10));
}
