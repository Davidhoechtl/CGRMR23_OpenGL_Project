#pragma once
#include "../Objects/Coin.h"
#include <vector>
#include <glm/glm.hpp>

class CoinGenerator 
{
public:
	int coinAmount = 10;
	std::vector<Coin*> coins;

	CoinGenerator(int coinAmount) 
	{
		if(coinAmount <= 10)
			this->coinAmount = coinAmount;

		coinPositions.push_back(glm::vec2(200, 0));
		coinPositions.push_back(glm::vec2(0, 200));
		coinPositions.push_back(glm::vec2(-200, 300));
		coinPositions.push_back(glm::vec2(-400, 550));
		coinPositions.push_back(glm::vec2(-700, -100));
		coinPositions.push_back(glm::vec2(-270, -400));
		coinPositions.push_back(glm::vec2(550, 150));
		coinPositions.push_back(glm::vec2(200, -600));
		coinPositions.push_back(glm::vec2(520, -180));
		coinPositions.push_back(glm::vec2(500, 500));

		SetupCoins();
	}

private:
	std::vector<glm::vec2> coinPositions;

	void SetupCoins() 
	{
		std::vector<glm::vec2> tempCoinPositions = coinPositions;
		srand(time(NULL));

		for (int i = 0; i < coinAmount; ++i) 
		{
			int index = rand() % (tempCoinPositions.size());
			glm::vec2 position = tempCoinPositions[index];

			coins.push_back(new Coin(position.x, position.y));
			tempCoinPositions.erase(tempCoinPositions.begin() + index);
		}
	}
};