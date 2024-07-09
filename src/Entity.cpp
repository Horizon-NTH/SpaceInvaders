#include "../include/Entity.h"

Entity::Entity(const hitbox& hitbox, const unsigned health) :
	GameObject(hitbox),
	m_health(health)
{
	hgui::after(std::chrono::milliseconds{}, [&]
		{
			m_gameEntities.push_back(weak_from_this());
		});
}

Entity::~Entity()
{
	m_gameEntities.erase(std::begin(std::ranges::remove_if(m_gameEntities, [](const std::weak_ptr<Entity>& ptr) { return !ptr.lock(); })), m_gameEntities.end());
}
