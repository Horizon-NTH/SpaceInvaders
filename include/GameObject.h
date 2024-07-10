#pragma once

#include "Include.h"

class GameObject
{
public:
	using hitbox = std::pair<hgui::point, hgui::size>;

	explicit GameObject(hitbox hitbox);
	virtual ~GameObject() = default;

	[[nodiscard]] bool is_overlapping(const hitbox& hitbox) const;
	[[nodiscard]] bool is_in_window() const;

	[[nodiscard]] hitbox& get_hitbox();
	[[nodiscard]] const hitbox& get_hitbox() const;

	static inline bool* m_isSfx;

protected:
	hitbox m_hitbox;
	std::string m_tempID;
};
