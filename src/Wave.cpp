#include "../include/Wave.h"
#include "../include/Bot.h"
#include "../include/Shot.h"
#include "../include/HealthRegeneration.h"
#include "../include/FullHealthRegeneration.h"
#include "../include/ShieldRegeneration.h"
#include "../include/FullShieldRegeneration.h"
#include "../include/MegaRegeneration.h"
#include "../include/Invincibility.h"
#include "../include/UnlimitedPower.h"
#include "../include/LevelUp.h"
#include "../include/Upgrade.h"
#include "../include/Laser.h"
#include "../include/Devastator.h"
#include "../include/Bomber.h"
#include "../include/Plazma.h"

Wave::Wave(const std::shared_ptr<hgui::kernel::Font>& font) :
	m_waveNumber(1u),
	m_score(0u),
	m_font(font),
	m_tempIDs({"", "", ""})
{
	m_labels = {
				hgui::LabelManager::create("Wave: 1", hgui::point(), font),
				hgui::LabelManager::create("Score: 0", hgui::point(0.5_em), font)
			};
	m_onBotDeath = [this](const std::shared_ptr<Bot>& bot)
		{
			m_tempIDs.push_back(hgui::TaskManager::program(std::chrono::milliseconds{1000}, [=, this, points = bot->get_level() * 100, hitbox = bot->get_hitbox()] { show_point(hitbox, points); }));
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

	m_labels.erase(std::ranges::remove_if(m_labels, [](const auto& label) { return label->get_text().substr(0, 5) == "Wave "; }).begin(), m_labels.end());
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
	std::ranges::for_each(botToRemove, [this](const auto& bot)
		{
			const auto hitbox = bot->get_hitbox();
			drop_power_up(hitbox.first + hitbox.second / 2.f, bot->get_level());
		});
	m_shotsToKeepAlive.insert(m_shotsToKeepAlive.end(), std::make_move_iterator(botToRemove.begin()), std::make_move_iterator(botToRemove.end()));
	m_bots.erase(botToRemove.begin(), botToRemove.end());
	m_shotsToKeepAlive.erase(std::ranges::remove_if(m_shotsToKeepAlive, [](const auto& bot) { return std::ranges::all_of(bot->get_shots(), [](const auto& shot) { return shot->is_destroyed(); }); }).begin(), m_shotsToKeepAlive.end());
	if (m_bots.empty())
	{
		m_waveNumber++;
		generate_wave();
		update();
	}
	const auto powerUpToRemove = std::ranges::stable_partition(m_powerUps, [](const auto& powerUp) { return !powerUp->is_consumed(); });
	std::ranges::for_each(powerUpToRemove, [this](const auto& power_up)
		{
			if (const auto point = power_up->get_point())
			{
				m_tempIDs.push_back(hgui::TaskManager::program(std::chrono::milliseconds{500}, [=, this, hitbox = power_up->get_hitbox()] { show_point(hitbox, point); }));
			}
		});
	m_powerUps.erase(powerUpToRemove.begin(), powerUpToRemove.end());
	m_tempIDs.erase(std::remove_if(m_tempIDs.begin() + 3, m_tempIDs.end(), [](const auto& tempID) { return !hgui::TaskManager::is_program(tempID); }), m_tempIDs.end());
	m_tempIDs.at(1) = hgui::TaskManager::program(std::chrono::milliseconds{1000}, [this] { check_wave_status(); });
}

void Wave::show_point(const Bot::hitbox& hitbox, const unsigned int points)
{
	const auto label = hgui::LabelManager::create("+ " + std::to_string(points), hitbox.first, m_font);
	m_labels.push_back(label);
	m_labels.back()->set_height(static_cast<unsigned int>(hgui::size(1.5_em).height));
	m_labels.back()->set_position(hitbox.first + hitbox.second / 2.f - m_labels.back()->get_size() / 2.f);
	m_score += points;
	update();
	m_tempIDs.at(2) = hgui::TaskManager::program(std::chrono::milliseconds{1000}, [this, label] { m_labels.erase(std::ranges::find(m_labels, label)); });
}

void Wave::drop_power_up(const hgui::point& position, const unsigned level)
{
	const double dropChance = level == 1u ? 0.2 : level == 2u ? 0.4 : 0.6;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution random(0., 1.);
	if (hgui::random(dropChance))
	{
		if (const double number = random(gen); number < HealthRegeneration::probability)
			m_powerUps.emplace_back(std::make_shared<HealthRegeneration>(position));
		else if (number < ShieldRegeneration::probability)
			m_powerUps.emplace_back(std::make_shared<ShieldRegeneration>(position));
		else if (number < FullHealthRegeneration::probability)
			m_powerUps.emplace_back(std::make_shared<FullHealthRegeneration>(position));
		else if (number < FullShieldRegeneration::probability)
			m_powerUps.emplace_back(std::make_shared<FullShieldRegeneration>(position));
		else if (number < MegaRegeneration::probability)
			m_powerUps.emplace_back(std::make_shared<MegaRegeneration>(position));
		else if (number < Invincibility::probability)
			m_powerUps.emplace_back(std::make_shared<Invincibility>(position));
		else if (number < UnlimitedPower::probability)
			m_powerUps.emplace_back(std::make_shared<UnlimitedPower>(position));
		else if (number < LevelUp::probability)
			m_powerUps.emplace_back(std::make_shared<LevelUp>(position));
		else
		{
			static std::uniform_int_distribution randomSpaceShipType(0, 3);
			if (const auto spaceShipType = randomSpaceShipType(gen); spaceShipType == 0)
				m_powerUps.emplace_back(std::make_shared<Upgrade<Laser>>(position));
			else if (spaceShipType == 1)
				m_powerUps.emplace_back(std::make_shared<Upgrade<Devastator>>(position));
			else if (spaceShipType == 2)
				m_powerUps.emplace_back(std::make_shared<Upgrade<Bomber>>(position));
			else
				m_powerUps.emplace_back(std::make_shared<Upgrade<Plazma>>(position));
		}
	}
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
