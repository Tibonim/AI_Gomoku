#ifndef ARBITER_HPP
# define ARBITER_HPP

#include "Island.hpp"

class Arbiter {
private:
    Case map[SIZE * SIZE];
	std::stack< std::tuple<Coord, Coord, Coord, bool> > plays;
    bool round;
    unsigned int bTooks;
    unsigned int wTooks;
	bool brkFive;
	bool dblThree;
	static Arbiter m_instance;
public:
	static Arbiter& Instance() {
		return m_instance;
	};
	void SetRules(bool, bool);
	void Fill(std::vector<int> const&);
	void Play(unsigned int, unsigned int);
	bool NextRound();
	bool PrevRound();
	bool GetTurn();
	int GetMap(int x, int y);
	char PutCheck(int x, int y, int mode);
	int VictoryCheck();
private:
	Arbiter();
	~Arbiter();
	Arbiter& operator= (const Arbiter&) {};
	Arbiter(const Arbiter&) {};
	std::vector<Island *> getIslands(Color, Case *);
	std::vector<Island *> epurIslands(std::vector<Island *> &);
	std::vector<Island *> winningIslands(std::vector<Island *> &);
	Island *recursiveSearch(unsigned int x, unsigned int y, Color color, Island *island, unsigned int xDir, unsigned int yDir);
	Island *joinIslands(unsigned int j, unsigned int i, Island *A, Island *B);
	std::vector<int> findIntersec(std::vector<Island *> &islands, Coord c, int prevent);
	int searchApplyTooks(std::vector<Island *> &islands);
	void generateMetadata();
	void filterFDT(std::vector<Island *> &islands);
	int simulation(int i, Case *map);
};

#endif /* !ARBITER_HPP */
