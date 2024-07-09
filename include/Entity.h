#pragma once

#include "Include.h"
#include "GameObject.h"

class PowerUp;

class Entity : public GameObject, public std::enable_shared_from_this<Entity>
{
	friend PowerUp;

public:
	Entity(const hitbox& hitbox, unsigned health);
	~Entity() override;

	virtual void take_damage() = 0;
	[[nodiscard]] virtual bool is_alive() const = 0;

protected:
	unsigned int m_health;

	inline static std::vector<std::weak_ptr<Entity>> m_gameEntities{};
};
