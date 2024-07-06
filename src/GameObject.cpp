#include "../include/GameObject.h"

GameObject::GameObject(hitbox hitbox) :
	m_hitbox(std::move(hitbox))
{
}

bool GameObject::is_overlapping(const hitbox& hitbox) const
{
	const auto& [position, size] = hitbox;
	const auto& [m_position, m_size] = m_hitbox;
	const bool collisionX = position.x + size.width >= m_position.x &&
	                        m_position.x + m_size.width >= position.x;
	const bool collisionY = position.y + size.height >= m_position.y &&
	                        m_position.y + m_size.height >= position.y;
	return collisionX && collisionY;
}

bool GameObject::is_in_window() const
{
	const auto position = hgui::point(0);
	const auto size = hgui::size(100_em);
	const auto& [m_position, m_size] = m_hitbox;
	const bool collisionX = position.x + size.width >= m_position.x &&
	                        m_position.x + m_size.width >= position.x;
	const bool collisionY = position.y + size.height >= m_position.y &&
	                        m_position.y + m_size.height >= position.y;
	return collisionX && collisionY;
}

GameObject::hitbox& GameObject::get_hitbox()
{
	return m_hitbox;
}

const GameObject::hitbox& GameObject::get_hitbox() const
{
	return m_hitbox;
}


