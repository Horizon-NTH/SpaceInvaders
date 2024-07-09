#pragma once

#include "GameObject.h"

class Player;

class PowerUp : public GameObject
{
public:
	explicit PowerUp(const hgui::point& position, const std::shared_ptr<hgui::kernel::Texture>& texture);
	~PowerUp() override;

	[[nodiscard]] bool is_consumed() const;
	[[nodiscard]] unsigned get_point() const;

protected:
	mutable unsigned m_point;

	virtual void apply_effect(const std::shared_ptr<Player>& player) const = 0;

private:
	std::shared_ptr<hgui::kernel::Sprite> m_texture;

	void move();
	void check_collision();
	void destroy(const std::string& warpType = "_player");
};
