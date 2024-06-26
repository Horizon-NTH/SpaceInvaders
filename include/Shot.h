#pragma once

#include "Include.h"
#include "GameObject.h"
#include "SpaceShip.h"

class SpaceShip::Shot : public GameObject
{
public:
	Shot(const hitbox& hitbox, const std::shared_ptr<hgui::kernel::Image>& shotImage, const GameObject::hitbox& damageHitbox, const hgui::point& position, const hgui::vec2& velocity);
	~Shot() override;

	[[nodiscard]] bool is_destroyed() const;

protected:
	void destroy();
	virtual void has_collide() = 0;
	virtual void collide();

	std::shared_ptr<hgui::kernel::Sprite> m_texture;

	hitbox m_damageHitbox;
	hgui::point m_position;
	hgui::vec2 m_velocity;

private:
	void move();
};
