#pragma once

#include "Include.h"
#include "Player.h"

class Devastator final : public Player
{
	class Shot;

public:
	Devastator();
	explicit Devastator(const Player& player);

	bool shoot() override;

private:
	void level_up() override;
};
