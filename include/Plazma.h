#pragma once

#include "Include.h"
#include "Player.h"

class Plazma final : public Player
{
	class Shot;

public:
	Plazma();
	explicit Plazma(const Player& player);

	bool shoot() override;

private:
	void level_up() override;
};
