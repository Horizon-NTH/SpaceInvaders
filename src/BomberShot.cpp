#include "../include/BomberShot.h"

Bomber::Shot::Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const hgui::point& position, const unsigned level) :
	SpaceShip::Shot(hitbox, image, std::make_pair(hitbox.first - level * hitbox.second / 2., (level + 1) * hitbox.second), position, hgui::vec2(0)),
	m_level(std::clamp(level, 1u, 3u))
{
	m_velocity = hgui::vec2(0, -static_cast<float>(m_level * 2));
}

void Bomber::Shot::has_collide()
{
	destroy();
	const auto position = m_hitbox.first + m_hitbox.second / 2 - m_damageHitbox.second / 2;
	const auto gif = hgui::gif_loader("assets/textures/explosions/explosion_bomber.gif");
	const auto explosion = hgui::SpriteManager::create(gif, m_damageHitbox.second, position);
	explosion->play();
	hgui::kernel::GIFData::delay totalDelay(100);
	for (const auto& delay : gif->get_data().pixels | std::views::values)
		totalDelay += delay;
	hgui::TaskManager::program(totalDelay, [explosion] {});
}
