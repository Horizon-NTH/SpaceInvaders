#include  "../include/PlazmaShot.h"

Plazma::Shot::Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const damage damage, const unsigned level) :
	SpaceShip::Shot(hitbox, image, hitbox, damage, hgui::vec2(0)),
	m_level(std::clamp(level, 1u, 3u))
{
	m_velocity = hgui::vec2(0, -static_cast<float>(m_level * 3));
}

void Plazma::Shot::has_collide()
{
	const auto size = hgui::size(5_em).set_reference(hgui::reference::HEIGHT);
	const auto position = m_hitbox.first + m_hitbox.second / 2 - size / 2;
	const auto explosion = hgui::SpriteManager::create(hgui::image_loader("assets/textures/explosions/explosion_plazma.png"), size, position);
	hgui::TaskManager::program(std::chrono::milliseconds(500), [explosion] {});
}

void Plazma::Shot::collide()
{
	std::vector<std::weak_ptr<Entity>> damagedEntities;
	std::ranges::copy_if(m_gameEntities, std::back_inserter(damagedEntities), [&](const std::weak_ptr<Entity>& ptr)
		{
			if (const auto entity = ptr.lock(); entity && entity->is_alive())
				return entity->is_overlapping(m_hitbox);
			return false;
		});
	if (!damagedEntities.empty())
	{
		bool newDamaged = false;
		for (const auto& ptr : damagedEntities)
		{
			if (std::ranges::find_if(m_collidedEntities, [&](const std::weak_ptr<Entity>& existingPtr)
				    {
					    return existingPtr.lock() == ptr.lock();
				    }) == m_collidedEntities.end())
			{
				m_collidedEntities.push_back(ptr);
				if (const auto entity = ptr.lock())
					entity->take_damage();
				newDamaged = true;
			}
		}
		if (newDamaged)
			has_collide();
	}
}
