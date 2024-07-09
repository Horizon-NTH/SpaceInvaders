#include "../include/BotShot.h"

Bot::Shot::Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const damage damage, const unsigned level) :
	SpaceShip::Shot(hitbox, image, hitbox, damage, hgui::vec2(0)),
	m_level(std::clamp(level, 1u, 3u))
{
	m_velocity = hgui::vec2(0, static_cast<float>(m_level * 5));
}

bool Bot::Shot::can_damaged(const std::weak_ptr<Entity>& entity)
{
	if (const auto bot = std::dynamic_pointer_cast<Bot>(entity.lock()))
	{
		bot->set_protection_shield();
		return false;
	}
	return true;
}

void Bot::Shot::has_collide()
{
	destroy();
	const auto size = hgui::size(5_em).set_reference(hgui::reference::HEIGHT);
	const auto position = m_hitbox.first + m_hitbox.second / 2 - size / 2;
	const auto explosion = hgui::SpriteManager::create(hgui::image_loader("assets/textures/explosions/explosion_laser.png"), size, position);
	hgui::TaskManager::program(std::chrono::milliseconds(500), [explosion] {});
}