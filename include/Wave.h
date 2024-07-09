#pragma once

#include "Bot.h"
#include "Include.h"

class Meteor;
class Bot;
class PowerUp;

class Wave final
{
public:
	explicit Wave(const std::shared_ptr<hgui::kernel::Font>& font);
	~Wave();

	[[nodiscard]] unsigned get_wave_number() const;
	void update(bool windowSizeChanged = false);
	void stop();

private:
	unsigned m_waveNumber;
	unsigned long m_spawnedMeteors;
	unsigned long long m_score;

	std::vector<std::shared_ptr<Bot>> m_bots;
	std::vector<std::shared_ptr<Meteor>> m_meteors;
	std::vector<std::shared_ptr<PowerUp>> m_powerUps;
	std::vector<std::shared_ptr<Bot>> m_shotsToKeepAlive;
	std::vector<std::shared_ptr<hgui::kernel::Label>> m_labels;
	std::shared_ptr<hgui::kernel::Font> m_font;
	std::vector<std::string> m_tempIDs;
	std::array<std::array<std::uniform_real_distribution<float>, 2>, 4> m_spawnRanges;
	std::function<void(const std::shared_ptr<Bot>&)> m_onBotDeath;

	void generate_wave();
	void check_wave_status();
	void spawn_meteors();
	void show_point(const Bot::hitbox& hitbox, unsigned int points);
	void drop_power_up(const hgui::point& position, unsigned level);

	hgui::point get_random_position(bool onlyOnTop = false);
};
