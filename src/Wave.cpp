#include "../include/Wave.h"
#include "../include/Bot.h"
#include "../include/Shot.h"

Wave::Wave(const std::shared_ptr<hgui::kernel::Font>& font) :
	m_waveNumber(100u),
	m_score(0u),
	m_font(font)
{
	m_labels = {
				hgui::LabelManager::create("Wave: 1", hgui::point(), font),
				hgui::LabelManager::create("Score: 0", hgui::point(0.5_em), font)
			};
	m_onBotDeath = [this](const std::shared_ptr<Bot>& bot)
		{
			const unsigned points = bot->get_level() * 100;
			m_score += points;
			update();
			m_tempIDs.at(3) = hgui::TaskManager::program(std::chrono::milliseconds{1000}, [=, this] { show_point(bot->get_hitbox(), points); });
		};
	update(true);
	generate_wave();
	check_wave_status();
}

Wave::~Wave()
{
	stop();
}

unsigned Wave::get_wave_number() const
{
	return m_waveNumber;
}

void Wave::update(const bool windowSizeChanged)
{
	m_labels[0]->set_text("Wave: " + std::to_string(m_waveNumber));
	m_labels[1]->set_text("Score: " + std::to_string(m_score));
	m_labels[0]->set_height(static_cast<unsigned int>(hgui::size(1.5_em).height));
	m_labels[1]->set_height(static_cast<unsigned int>(hgui::size(1.5_em).height));
	m_labels[0]->set_position(hgui::point(99.5_em - m_labels[0]->get_size().width, 0.5_em));
	if (windowSizeChanged)
		m_spawnRanges = {
					std::array{std::uniform_real_distribution(hgui::point(-10_em).x, 0.f), std::uniform_real_distribution(hgui::point(100_em).x, hgui::point(110_em).x)},
					std::array{std::uniform_real_distribution(hgui::point(-10_em).y, 0.f), std::uniform_real_distribution(hgui::point(100_em).y, hgui::point(110_em).y)},
					std::array{std::uniform_real_distribution(hgui::point(-10_em).x, hgui::point(110_em).x), std::uniform_real_distribution(hgui::point(-10_em).y, hgui::point(110_em).y)}
				};
}

void Wave::stop()
{
	std::ranges::for_each(m_tempIDs, [](const auto& tempID)
		{
			if (hgui::TaskManager::is_program(tempID))
				hgui::TaskManager::deprogram(tempID);
		});

	m_labels.erase(std::ranges::remove_if(m_labels, [](const auto& label) { return label->get_text().substr(0, 4) == "Wave"; }).begin(), m_labels.end());
}

void Wave::generate_wave()
{
	std::vector<bool> invincibilities(std::div(static_cast<int>(m_waveNumber), 5).quot);
	while (m_bots.size() < std::clamp(m_waveNumber * 2u + static_cast<unsigned>((hgui::random(0.5) ? 1 : -1) * std::ceil(m_waveNumber / 10)), 3u, 25u))
	{
		std::generate_n(invincibilities.begin(), std::div(static_cast<int>(m_waveNumber), 5).quot, [] { return hgui::random(0.01); });
		m_bots.emplace_back(std::make_shared<Bot>(get_random_position(),
			1 + hgui::random(std::sqrt(m_waveNumber) * std::log(m_waveNumber * m_waveNumber) * 0.01) + hgui::random(std::sqrt(m_waveNumber) * std::log(m_waveNumber) * 0.01),
			hgui::random(std::pow(std::log(m_waveNumber * m_waveNumber), 2) * 0.01),
			std::ranges::any_of(invincibilities, [](const auto& invincibility) { return invincibility; }), m_onBotDeath));
	}
	const auto waveTitle = hgui::LabelManager::create("Wave " + std::to_string(m_waveNumber), hgui::point(), m_font);
	waveTitle->set_height(static_cast<unsigned int>(hgui::size(5_em).height));
	waveTitle->set_position(hgui::point(50_em) - waveTitle->get_size() / 2.f);
	m_labels.push_back(waveTitle);
	m_tempIDs.at(0) = hgui::TaskManager::program(std::chrono::milliseconds(1000), [this, waveTitle] { m_labels.erase(std::ranges::find(m_labels, waveTitle)); });
}

void Wave::check_wave_status()
{
	const auto botToRemove = std::ranges::stable_partition(m_bots, [](const auto& bot) { return bot->is_alive(); });
	m_shotsToKeepAlive.insert(m_shotsToKeepAlive.end(), std::make_move_iterator(botToRemove.begin()), std::make_move_iterator(botToRemove.end()));
	m_bots.erase(botToRemove.begin(), botToRemove.end());
	m_shotsToKeepAlive.erase(std::ranges::remove_if(m_shotsToKeepAlive, [](const auto& bot) { return std::ranges::all_of(bot->get_shots(), [](const auto& shot) { return shot->is_destroyed(); }); }).begin(), m_shotsToKeepAlive.end());
	if (m_bots.empty())
	{
		m_waveNumber++;
		generate_wave();
		update();
	}
	m_tempIDs.at(1) = hgui::TaskManager::program(std::chrono::milliseconds{1000}, [this] { check_wave_status(); });
}

void Wave::show_point(const Bot::hitbox& hitbox, const unsigned int points)
{
	const auto label = hgui::LabelManager::create("+ " + std::to_string(points), hitbox.first, m_font);
	m_labels.push_back(label);
	m_labels.back()->set_height(static_cast<unsigned int>(hgui::size(1.5_em).height));
	m_labels.back()->set_position(hitbox.first + hitbox.second / 2.f - m_labels.back()->get_size() / 2.f);
	m_tempIDs.at(2) = hgui::TaskManager::program(std::chrono::milliseconds{1000}, [this, label] { m_labels.erase(std::ranges::find(m_labels, label)); });
}

hgui::point Wave::get_random_position()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution randomRanges(0, 1);
	if (randomRanges(gen))
		return {m_spawnRanges[0][randomRanges(gen)](gen), m_spawnRanges[2][1](gen)};
	return {m_spawnRanges[2][0](gen), m_spawnRanges[1][randomRanges(gen)](gen)};
}
