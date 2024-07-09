#pragma once

#include "PowerUp.h"
#include "Player.h"

class Bomber;
class Devastator;
class Laser;

template<typename T, typename = std::enable_if<std::is_base_of_v<Player, T>>>
class Upgrade final : public PowerUp
{
public:
	explicit Upgrade(const hgui::point& position);

	static constexpr double probability{1.};
	static inline std::shared_ptr<Player>* m_player;

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};

template<typename T, typename T0>
Upgrade<T, T0>::Upgrade(const hgui::point& position) :
	PowerUp(position, hgui::TextureManager::create(hgui::image_loader("assets/textures/power_up/" +
	                                                                  std::string(std::is_same_v<T, Laser>
		                                                                  ? "laser"
		                                                                  : std::is_same_v<T, Devastator>
		                                                                  ? "devastator"
		                                                                  : std::is_same_v<T, Bomber>
		                                                                  ? "bomber"
		                                                                  : "plazma") + std::string(".png"))))
{
}

template<typename T, typename T0>
void Upgrade<T, T0>::apply_effect(const std::shared_ptr<Player>& player) const
{
	m_point = 1000u;
	(*m_player)->stop();
	*m_player = std::make_shared<T>(*player);
}
