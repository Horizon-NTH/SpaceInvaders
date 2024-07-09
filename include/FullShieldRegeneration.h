#pragma once

#include "PowerUp.h"

class FullShieldRegeneration final : public PowerUp
{
public:
	explicit FullShieldRegeneration(const hgui::point& position);

	static constexpr double probability{0.7};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
