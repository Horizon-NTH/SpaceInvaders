#pragma once

#include "PowerUp.h"

class ShieldRegeneration final : public PowerUp
{
public:
	explicit ShieldRegeneration(const hgui::point& position);

	static constexpr double probability{0.5};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
