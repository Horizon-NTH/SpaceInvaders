#pragma once

#include "Bot.h"
#include "Include.h"

class Bot;

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
	unsigned long long m_score;

	std::vector<std::shared_ptr<Bot>> m_bots;
	std::vector<std::shared_ptr<Bot>> m_shotsToKeepAlive;
	std::vector<std::shared_ptr<hgui::kernel::Label>> m_labels;
	std::shared_ptr<hgui::kernel::Font> m_font;
	std::array<std::string, 4> m_tempIDs;
	std::array<std::array<std::uniform_real_distribution<float>, 2>, 3> m_spawnRanges;
	std::function<void(const std::shared_ptr<Bot>&)> m_onBotDeath;

	void generate_wave();
	void check_wave_status();
	void show_point(const Bot::hitbox& hitbox, unsigned int points);

	hgui::point get_random_position();
};
