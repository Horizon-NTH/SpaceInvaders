#include "../include/Shot.h"
#include "../include/Player.h"

SpaceShip::Shot::Shot(const hitbox& hitbox, const std::shared_ptr<hgui::kernel::Image>& shotImage, GameObject::hitbox damageHitbox, const damage damage, const hgui::vec2& velocity, const std::shared_ptr<hgui::kernel::SoundPlayer>& shotSound) :
	GameObject(hitbox),
	m_texture(nullptr),
	m_shotSound(shotSound),
	m_damageHitbox(std::move(damageHitbox)),
	m_velocity(velocity),
	m_damage(damage)
{
	m_texture = hgui::SpriteManager::create(shotImage, m_hitbox.second, m_hitbox.first);
	if (*m_isSfx)
		m_shotSound->play();
	m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
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
		m_hitbox.first += m_velocity;
		m_damageHitbox.first += m_velocity;
		m_texture->set_position(m_hitbox.first);
		m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
		collide();
	}
}

bool SpaceShip::Shot::can_damaged(const std::weak_ptr<Entity>& entity)
{
	return true;
}

void SpaceShip::Shot::collide()
{
	if (std::ranges::any_of(m_gameEntities, [&](const std::weak_ptr<Entity>& ptr)
		{
			if (const auto entity = ptr.lock(); entity && entity->is_alive())
				return entity->is_overlapping(m_hitbox);
			return false;
		}))
	{
		std::vector<std::weak_ptr<Entity>> damagedEntities;
		std::ranges::copy_if(m_gameEntities, std::back_inserter(damagedEntities), [&](const std::weak_ptr<Entity>& ptr)
			{
				if (const auto entity = ptr.lock(); entity && entity->is_alive())
					return entity->is_overlapping(m_damageHitbox);
				return false;
			});
		for (const auto& ptr : damagedEntities)
		{
			if (const auto entity = ptr.lock(); entity && can_damaged(entity))
				for (unsigned i = 0; i < m_damage; i++)
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
