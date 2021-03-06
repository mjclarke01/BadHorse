#include <vector>
#include <memory>
#include "gmock/gmock.h"
#include "BadGuy.h"
#include "Graph.h"


class BadGuyTest : public ::testing::Test {
protected:
	virtual void SetUp() override {
		baddie = std::make_shared<BadGuy>("Baddie");
		enemy = std::make_shared<BadGuy>("Enemy");
		BadGuy::addEnemyAndViceVersa(baddie, enemy);
	}

	std::shared_ptr<BadGuy> baddie;
	std::shared_ptr<BadGuy> enemy;
};


TEST(BadHorseSolver, NumberOfEnemiesForPersonWithNoEnemiesIsZero) {
	BadGuy baddie("Dave");

	ASSERT_EQ(baddie.getNumberEnemies(), 0);
}

TEST(BadHorseSolver, GettingNameReturnsName) {
	BadGuy baddie("Dave");

	ASSERT_EQ(baddie.getName(), "Dave");
}

TEST(BadHorseSolver, GettingColourWhenNotSetReturnsNone) {
	BadGuy baddie("Dave");

	ASSERT_EQ(baddie.getColour(), Colour::NONE);
}

TEST(BadHorseSolver, AfterSettingColourGettingColourReturnsSetValue) {
	BadGuy baddie("Dave");
	baddie.setColour(Colour::RED);

	ASSERT_EQ(baddie.getColour(), Colour::RED);
}

TEST_F(BadGuyTest, AddedEnemyIsAnEnemy) {
	baddie->addEnemy(enemy);

	ASSERT_TRUE(baddie->isEnemy(enemy));
}

TEST_F(BadGuyTest, AddingEnemyMoreThanOnceIsIgnored) {
	baddie->addEnemy(enemy);

	ASSERT_EQ(baddie->getNumberEnemies(), 1);
}

TEST_F(BadGuyTest, CanAccessEnemiesCollection) {
	baddie->addEnemy(enemy);

	auto enemies = baddie->getEnemies();

	// Get first (and, in this case only) enemy
	for (auto e : enemies) {
		ASSERT_EQ(e->getName(), enemy->getName());
	}
}

TEST_F(BadGuyTest, AddingEnemyAlsoAddsMeToTheirEnemies) {
	ASSERT_TRUE(enemy->isEnemy(baddie));
}

TEST(GraphTest, EmptyGraphHasNoNodes) {
	Graph graph;
	ASSERT_EQ(graph.getNumberNodes(), 0);
}

TEST(GraphTest, AddingEnemyPairGivesNodeCountOfTwo) {
	Graph graph;

	graph.addPairing("Baddie", "Enemy");
	ASSERT_EQ(graph.getNumberNodes(), 2);
}

TEST(GraphTest, CanGetNodeNyName) {
	Graph graph;

	graph.addPairing("Baddie", "Enemy");
	ASSERT_EQ(graph.getNodeByName("Baddie")->getName(), "Baddie");
}

TEST(GraphTest, AddingEnemyPairGivesNodeWithOneEnemy) {
	Graph graph;

	graph.addPairing("Baddie", "Enemy");
	ASSERT_EQ(graph.getNodeByName("Baddie")->getNumberEnemies(), 1);
}

TEST(GraphTest, ForTwoPairsOfEnemiesGivesNodeCountOfFour) {
	Graph graph;
	graph.addPairing("Baddie", "Enemy");
	graph.addPairing("Naughty", "Nemesis");

	ASSERT_EQ(graph.getNumberNodes(), 4);
}

TEST(GraphTest, ForTwoPairsOfEnemiesWithOneSharedEnemyGivesNodeCountOfThree) {
	Graph graph;
	graph.addPairing("Baddie", "Enemy");
	graph.addPairing("Baddie", "Nemesis");

	ASSERT_EQ(graph.getNumberNodes(), 3);
}

TEST(GraphTest, ForTwoPairsOfEnemiesWithOneSharedEnemyTheSharedEnemyHasTwoEnemies) {
	Graph graph;
	graph.addPairing("Baddie", "Enemy");
	graph.addPairing("Baddie", "Nemesis");

	ASSERT_EQ(graph.getNodeByName("Baddie")->getNumberEnemies(), 2);
}

TEST(GraphTest, ForEnemyPairSolvingSetsColours) {
	Graph graph;

	graph.addPairing("Baddie", "Enemy");
	graph.canBeSolved();

	ASSERT_NE(graph.getNodeByName("Baddie")->getColour(), Colour::NONE);
	ASSERT_NE(graph.getNodeByName("Enemy")->getColour(), Colour::NONE);
}

TEST(GraphTest, ForEnemyPairSolvingSetsDifferentColours) {
	Graph graph;

	graph.addPairing("Baddie", "Enemy");
	graph.canBeSolved();

	ASSERT_EQ(graph.getNodeByName("Baddie")->getColour(), Colour::RED);
	ASSERT_EQ(graph.getNodeByName("Enemy")->getColour(), Colour::BLACK);
}

TEST(GraphTest, ForTwoPairsOfEnemiesWithOneSharedEnemyThenSharedEnemyIsDifferentColour) {
	Graph graph;
	graph.addPairing("Baddie", "Enemy");
	graph.addPairing("Baddie", "Nemesis");
	graph.canBeSolved();

	auto shared = graph.getNodeByName("Baddie")->getColour();

	ASSERT_NE(graph.getNodeByName("Enemy")->getColour(), shared);
	ASSERT_NE(graph.getNodeByName("Nemesis")->getColour(), shared);
}

TEST(GraphTest, ForChainedEnemiesTheyAlternateColours) {
	Graph graph;
	graph.addPairing("Baddie", "Enemy");
	graph.addPairing("Enemy", "Nemesis");
	graph.canBeSolved();

	ASSERT_NE(graph.getNodeByName("Baddie")->getColour(), graph.getNodeByName("Enemy")->getColour());
	ASSERT_NE(graph.getNodeByName("Enemy")->getColour(), graph.getNodeByName("Nemesis")->getColour());
}

TEST(GraphTest, ForChainedEnemiesCanBeSolvedIsTrue) {
	Graph graph;
	graph.addPairing("Baddie", "Enemy");
	graph.addPairing("Enemy", "Nemesis");

	ASSERT_EQ(true, graph.canBeSolved());
}

TEST(GraphTest, ForNoSolutionPossibleCanBeSolvedIsFalse) {
	Graph graph;
	graph.addPairing("Dead_Bowie", "Fake_Thomas_Jefferson");
	graph.addPairing("Fake_Thomas_Jefferson", "Fury_Leika");
	graph.addPairing("Fury_Leika", "Dead_Bowie");

	ASSERT_EQ(false, graph.canBeSolved());
}

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}