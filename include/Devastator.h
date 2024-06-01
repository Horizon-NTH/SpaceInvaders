#pragma once

#include "Include.h"
#include "Player.h"

class Devastator final : public Player
{
	class Shot;

public:
	Devastator();

	bool shoot() override;
};
