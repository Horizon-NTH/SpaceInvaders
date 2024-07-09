#include "../include/DevastatorShot.h"

Devastator::Shot::Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const damage damage, const unsigned level) :
	SpaceShip::Shot(hitbox, image, hitbox, damage, hgui::vec2(0, -1)),
	m_level(std::clamp(level, 1u, 3u))
{
	hgui::TaskManager::program(std::chrono::milliseconds(1000 * level), [this] { destroy(); });
}

void Devastator::Shot::collide()
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
		for (const auto& ptr : damagedEntities)
		{
			if (std::ranges::find_if(m_collidedEntities, [&](const std::weak_ptr<Entity>& existingPtr)
				    {
					    return existingPtr.lock() == ptr.lock();
				    }) == m_collidedEntities.end())
			{
				m_collidedEntities.push_back(ptr);
				if (const auto entity = ptr.lock())
				{
					entity->take_damage();
					const auto& [entityPosition, entitySize] = entity->get_hitbox();
					const auto position = hgui::point(
						(std::max(entityPosition.x, m_hitbox.first.x) + std::min(entityPosition.x + entitySize.width, m_hitbox.first.x + m_hitbox.second.width)) / 2,
						(std::max(entityPosition.y, m_hitbox.first.y) + std::min(entityPosition.y + entitySize.height, m_hitbox.first.y + m_hitbox.second.height)) / 2);
					const auto explosion = hgui::SpriteManager::create(hgui::image_loader("assets/textures/explosions/explosion_devastator.png"), hgui::size(5_em).set_reference(hgui::reference::HEIGHT), position - hgui::size(2.5_em).set_reference(hgui::reference::HEIGHT));
					hgui::TaskManager::program(std::chrono::milliseconds(500), [explosion] {});
				}
			}
		}
	}
}

void Devastator::Shot::has_collide()
{
}

