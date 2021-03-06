#pragma once

#include <memory>
#include <set>

enum class Colour { NONE, RED, BLACK };

class BadGuy {
private:
	std::set<std::shared_ptr<BadGuy>> enemies;
	std::string name;
	Colour colour;

public:
	static void addEnemyAndViceVersa(std::shared_ptr<BadGuy> enemy1, std::shared_ptr<BadGuy> enemy2);

	explicit BadGuy(std::string name) : name(name), colour(Colour::NONE) {

	}

	std::string getName() const {
		return name;
	}

	Colour getColour() const {
		return colour;
	}

	void setColour(Colour colour) {
		this->colour = colour;
	}

	size_t getNumberEnemies() const {
		return enemies.size();
	}

	void addEnemy(std::shared_ptr<BadGuy> enemy) {
		enemies.insert(enemy);
	}

	bool isEnemy(std::shared_ptr<BadGuy> enemy) const {
		auto it = enemies.find(enemy);
		if (it != enemies.end()) {
			return true;
		}

		return false;
	}

	std::set<std::shared_ptr<BadGuy>> getEnemies() const {
		return enemies;
	}
};

inline void BadGuy::addEnemyAndViceVersa(std::shared_ptr<BadGuy> enemy1, std::shared_ptr<BadGuy> enemy2) {
	enemy1->addEnemy(enemy2);
	enemy2->addEnemy(enemy1);
}
