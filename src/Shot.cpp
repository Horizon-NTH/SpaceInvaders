#include "../include/Shot.h"
#include "../include/Player.h"

SpaceShip::Shot::Shot(const hitbox& hitbox, const std::shared_ptr<hgui::kernel::Image>& shotImage, const GameObject::hitbox& damageHitbox, const hgui::point& position, const hgui::vec2& velocity):
	GameObject(hitbox),
	m_texture(nullptr),
	m_damageHitbox(damageHitbox),
	m_position(position),
	m_velocity(velocity)
{
	m_texture = hgui::SpriteManager::create(shotImage, m_hitbox.second, m_hitbox.first);
	hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
}

SpaceShip::Shot::~Shot()
{
	destroy();
}

bool SpaceShip::Shot::is_destroyed() const
{
	return !static_cast<bool>(m_texture);
}

void SpaceShip::Shot::move()
{
	if (!is_in_window())
		destroy();
	if (m_texture)
	{
		m_position += m_velocity;
		m_hitbox.first += m_velocity;
		m_texture->set_position(m_hitbox.first);
		m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
		collide();
	}
}

void SpaceShip::Shot::collide()
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
			if (const auto entity = ptr.lock())
				entity->take_damage();
		}
		has_collide();
	}
}

void SpaceShip::Shot::destroy()
{
	m_texture = nullptr;
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
}
