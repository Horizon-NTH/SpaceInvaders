#pragma once

#include "Include.h"
#include "GameObject.h"
#include "SpaceShip.h"

class SpaceShip::Shot : public GameObject
{
public:
	Shot(const hitbox& hitbox, const std::shared_ptr<hgui::kernel::Image>& shotImage, GameObject::hitbox damageHitbox, damage damage, const hgui::vec2& velocity);
	~Shot() override;

	[[nodiscard]] bool is_destroyed() const;

protected:
	void destroy();
	virtual void has_collide() = 0;
	virtual bool can_damaged(const std::weak_ptr<Entity>& entity);
	virtual void collide();

	std::shared_ptr<hgui::kernel::Sprite> m_texture;

	hitbox m_damageHitbox;
	hgui::point m_position;
	hgui::vec2 m_velocity;
	unsigned m_damage;

private:
	void move();
};
