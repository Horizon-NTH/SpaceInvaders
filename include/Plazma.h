#pragma once

#include "Include.h"
#include "Player.h"

class Plazma final : public Player
{
	class Shot;

public:
	Plazma();

	bool shoot() override;
};
