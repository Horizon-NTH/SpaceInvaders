#pragma once

#include "PowerUp.h"

class UnlimitedPower final : public PowerUp
{
public:
	explicit UnlimitedPower(const hgui::point& position);

	static constexpr double probability{0.825};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
