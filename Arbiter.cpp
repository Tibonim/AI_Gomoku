#include <iostream>
#include "Arbiter.hpp"

bool compareIslands(Island *A, Island *B)
{
    if (A->GetBody().size() < B->GetBody().size())
        return true;
    else if (A->GetBody().size() == B->GetBody().size())
    {
        if (A->GetBody()[0].x < B->GetBody()[0].x)
        return true;
        else if (A->GetBody()[0].x < B->GetBody()[0].x)
        {
            if (A->GetBody()[0].y < B->GetBody()[0].y)
            return true;
        }
    }
    return false;
}

bool uniqueIsland(Island *A, Island *B)
{
    if (A->GetBody().size() == B->GetBody().size() && A->GetBody()[0].x == B->GetBody()[0].x && A->GetBody()[0].y == B->GetBody()[0].y)
    return true;
    return false;
}

Arbiter::Arbiter()
{
    for (size_t i = 0; i < SIZE * SIZE; i++) {
        this->map[i].property = 0 | (1 << 2);
	}
    this->round = true;
    this->bTooks = 0;
	this->wTooks = 0;
	this->brkFive = false;
	this->dblThree = false;
}

Arbiter::~Arbiter()
{

}

bool Arbiter::GetTurn()
{
	return round;
}

void Arbiter::SetRules(bool brokenFive, bool doubleThree)
{
	this->brkFive = brokenFive;
	this->dblThree = doubleThree;
}

void Arbiter::Fill(std::vector<int> const&m)
{
    int i = 0;
    for(std::vector<int>::const_iterator it = m.begin(); it != m.end(); ++it) {
        this->map[i].property = *it;
		this->map[i].property = this->map[i].property | (1 << 2);
        i++;
    }
}

void Arbiter::Play(unsigned int x, unsigned int y) {
	Coord c = { x, y, static_cast<unsigned int>((CHECK_TURN(this->round) == Color::black) ? 2 : 1) };
	Coord a = { 0,0,0 };
	Coord b = { 0,0,0 };
	this->plays.push(std::make_tuple(c, a, b, false));
	map[x + (SIZE * y)].property = CHECK_TURN(this->round);
}

bool Arbiter::NextRound() {
	round = !round;
	return round;
}

bool Arbiter::PrevRound() {
	if (!this->plays.empty())
	{
		map[std::get<0>(this->plays.top()).x + (SIZE * std::get<0>(this->plays.top()).y)].property = 0;
		if (std::get<3>(this->plays.top()) == true)
		{
			map[std::get<1>(this->plays.top()).x + (SIZE * std::get<1>(this->plays.top()).y)].property = std::get<1>(this->plays.top()).color;
			map[std::get<2>(this->plays.top()).x + (SIZE * std::get<2>(this->plays.top()).y)].property = std::get<2>(this->plays.top()).color;
		}
		this->plays.pop();
		round = !round;
	}
	return round;
}

int Arbiter::GetMap(int x, int y) {
	int **convertedMap = static_cast<int **>(malloc(sizeof(int *) * SIZE));
	for (size_t i = 0; i < SIZE; i++) {
		convertedMap[i] = static_cast<int *>(malloc(sizeof(int) * SIZE));
		for (size_t j = 0; j < SIZE; j++)
			convertedMap[i][j] = GET_COLOR_WXY(this->map[i + (SIZE * j)]);
	}
	std::vector<Island *> islands = getIslands(CHECK_TURN(this->round), this->map);
	for (std::vector<Island *>::iterator it = islands.begin(); it != islands.end(); ++it) {
		std::vector<Coord> const tmp = (*it)->GetBody();
		std::cout << "[" << (*it)->GetBefore()[2].color << "-" << (*it)->GetBefore()[1].color << "-" << (*it)->GetBefore()[0].color << "]";
		for (std::vector<Coord>::const_iterator itt = tmp.begin(); itt != tmp.end(); ++itt) {
			std::cout << "(" << (*itt).x << ":" << (*itt).y << ")";
		}
		std::cout << "[" << (*it)->GetAfter()[0].color << "-" << (*it)->GetAfter()[1].color << "-" << (*it)->GetAfter()[2].color << "]";
		std::cout << std::endl;
	}
	return GET_COLOR_WXY(this->map[x + (SIZE * y)]);
}

char Arbiter::PutCheck(int x, int y, int mode)
{
	if (mode == 1) {
		for (size_t i = 0; i < (SIZE * SIZE); i++)
			map[i].property = GET_COLOR_WXY(this->map[i]) | (1 << 2);
		generateMetadata();
	}
	return CAN_PUT_WXY(this->map[x + (SIZE * y)]);
}

int Arbiter::VictoryCheck()
{
	std::vector<Island *> islands = getIslands(CHECK_TURN(this->round), this->map);
	std::vector<Island *> winIslands = winningIslands(islands);
	bool bWin = false;
	bool wWin = false;
	if (CHECK_TURN(this->round) == Color::black) {
		this->bTooks += searchApplyTooks(islands);
	}
	else if (CHECK_TURN(this->round) == Color::white) {
		this->wTooks += searchApplyTooks(islands);
	}
	if (this->bTooks >= 10)
		bWin = true;
	else if (this->wTooks >= 10)
		wWin = true;
	for (std::vector<Island *>::iterator it = winIslands.begin(); it != winIslands.end(); ++it) {
		if ((*it)->GetColor() == Color::black)
			bWin = true;
		else if ((*it)->GetColor() == Color::white)
			wWin = true;
	}
    return (bWin == true && wWin == true) ? 3 : ((bWin == false && wWin == false) ? 0 : (wWin == true) ? 1 : 2);
}

Island *Arbiter::joinIslands(unsigned int x, unsigned int y, Island *a, Island *b)
{
	Color col = a->GetColor();
    std::vector<Coord> const &aBody = a->GetBody();
    std::vector<Coord> const &bBody = b->GetBody();
    Island *fusion = new Island(col);
    fusion->AddBack(x, y, col);
	fusion->SetBefore(a->GetAfter()[0].x, a->GetAfter()[0].y, static_cast<Color>(a->GetAfter()[0].color), 0);
	fusion->SetBefore(a->GetAfter()[1].x, a->GetAfter()[1].y, static_cast<Color>(a->GetAfter()[1].color), 1);
	fusion->SetBefore(a->GetAfter()[2].x, a->GetAfter()[2].y, static_cast<Color>(a->GetAfter()[2].color), 2);
	fusion->SetAfter(b->GetAfter()[0].x, b->GetAfter()[0].y, static_cast<Color>(b->GetAfter()[0].color), 0);
	fusion->SetAfter(b->GetAfter()[1].x, b->GetAfter()[1].y, static_cast<Color>(b->GetAfter()[1].color), 1);
	fusion->SetAfter(b->GetAfter()[2].x, b->GetAfter()[2].y, static_cast<Color>(b->GetAfter()[2].color), 2);
	for(std::vector<Coord>::const_iterator it = aBody.begin(); it != aBody.end(); ++it) {
        if (it != aBody.begin()) {
            fusion->AddFront(*it);
        }
    }
    for(std::vector<Coord>::const_iterator it = bBody.begin(); it != bBody.end(); ++it) {
        if (it != bBody.begin()) {
            fusion->AddBack(*it);
        }
    }
    return fusion;
}

Island *Arbiter::recursiveSearch(unsigned int x, unsigned int y, Color color, Island *island, unsigned int xDir, unsigned int yDir)
{
    if (IS_IN_MAP(x,y)) {
        if (GET_COLOR(x, y, this->map) != color) {
			if (island->GetAfter()[0].color == Color::illegal) {
				island->SetAfter(x, y, static_cast<Color>(GET_COLOR(x, y, this->map)), 0);
				return recursiveSearch(x + xDir, y + yDir, color, island, xDir, yDir);
			}
			else if (island->GetAfter()[1].color == Color::illegal) {
				island->SetAfter(x, y, static_cast<Color>(GET_COLOR(x, y, this->map)), 1);
				return recursiveSearch(x + xDir, y + yDir, color, island, xDir, yDir);
			}
			else
				island->SetAfter(x, y, static_cast<Color>(GET_COLOR(x, y, this->map)), 2);
        }
        else {
            island->AddBack(x, y, color);
			return recursiveSearch(x + xDir, y + yDir, color, island, xDir, yDir);
		}
    }
    return island;
}

std::vector<Island *> Arbiter::epurIslands(std::vector<Island *> &islands)
{
	for (int i = 0; i < islands.size(); i++)
	{
		if (islands[i]->GetBody().size() == 1)
		{
			std::vector<int> r = findIntersec(islands, islands[i]->GetBody()[0], i);
			if (r.size() >= 1) {
				islands.erase(islands.begin() + i);
				i--;
			}
		}
	}
    std::sort(islands.begin(), islands.end(), compareIslands);
    for(std::vector<Island *>::iterator it = islands.begin(); it != islands.end(); ++it) {
        if ((it + 1) != islands.end() && uniqueIsland(*it, *(it + 1)))
            it = islands.erase(it);
    }
	for (size_t i = 0; i < islands.size(); i++)
	{
		if (i + 1 < islands.size() && uniqueIsland(islands[i], islands[i + 1])) {
			islands.erase(islands.begin() + i);
			i--;
		}
	}
    return islands;
}

std::vector<Island *> Arbiter::getIslands(Color color, Case *m)
{
    std::vector<Island *> islands;
    for (unsigned int i = 0; i < SIZE; i++) {
        for (unsigned int j = 0; j < SIZE; j++) {
            if (GET_COLOR(j, i, m) == color) {
                islands.push_back(joinIslands(j, i, recursiveSearch(j, i, color, new Island(color), -1, -1), recursiveSearch(j, i, color, new Island(color), 1, 1)));
                islands.push_back(joinIslands(j, i, recursiveSearch(j, i, color, new Island(color), 0, -1), recursiveSearch(j, i, color, new Island(color), 0, 1)));
                islands.push_back(joinIslands(j, i, recursiveSearch(j, i, color, new Island(color), 1, -1), recursiveSearch(j, i, color, new Island(color), -1, 1)));
                islands.push_back(joinIslands(j, i, recursiveSearch(j, i, color, new Island(color), -1, 0), recursiveSearch(j, i, color, new Island(color), 1, 0)));
            }
        }
    }
    return epurIslands(islands);
}

std::vector<int> Arbiter::findIntersec(std::vector<Island *> &islands, Coord c, int prevent)
{
	std::vector<int> r;
	for (size_t i = 0; i < islands.size(); i++) {
		if (i != prevent) {
			for (std::vector<Coord>::const_iterator it = islands[i]->GetBody().begin(); it != islands[i]->GetBody().end(); ++it) {
				if (c.color == (*it).color && c.x == (*it).x && c.y == (*it).y)
					r.push_back(i);
			}
		}
	}
	return r;
}

std::vector<Island *> Arbiter::winningIslands(std::vector<Island *> &islands)
{
	std::vector<Island *> nw;
	for (size_t i = 0; i < islands.size(); i++) {
		if (islands[i]->GetBody().size() >= 5) {
			int l = 0;
			for (std::vector<Coord>::const_iterator it = islands[i]->GetBody().begin(); it != islands[i]->GetBody().end(); ++it) {
				std::vector<int> r = findIntersec(islands, (*it), i);
				l++;
				for (std::vector<int>::iterator it = r.begin(); it != r.end(); ++it) {
					if (brkFive == true && IS_EATABLE_ISLAND(islands[*it])) {
						l = 0;
					}
				}
				if (l == 5) {
					nw.push_back(islands[i]);
					break;
				}
			}
		}
	}
    return nw;
}

int Arbiter::searchApplyTooks(std::vector<Island *> &islands) {
	int i = 0;
	for (std::vector<Island *>::iterator it = islands.begin(); it != islands.end(); ++it) {
		if ((std::get<0>(this->plays.top()).x == (*it)->GetBefore()[0].x && std::get<0>(this->plays.top()).y == (*it)->GetBefore()[0].y) ||
			(std::get<0>(this->plays.top()).x == (*it)->GetAfter()[0].x && std::get<0>(this->plays.top()).y == (*it)->GetAfter()[0].y)) {
			map[(*it)->GetBody()[0].x + (SIZE * (*it)->GetBody()[0].y)].property = 0 | (1 << 2);
			map[(*it)->GetBody()[1].x + (SIZE * (*it)->GetBody()[1].y)].property = 0 | (1 << 2);
			std::get<1>(this->plays.top()) = (*it)->GetBody()[0];
			std::get<2>(this->plays.top()) = (*it)->GetBody()[1];
			std::get<3>(this->plays.top()) = true;
			i++;
		}
	}
	return i;
}

void Arbiter::filterFDT(std::vector<Island *> &islands) {
	int cFDT = 0;
	for (size_t i = 0; i < islands.size(); i++)
	{
		if (islands[i]->GetBody().size() == 3 && islands[i]->GetBefore()[0].color == Color::empty && islands[i]->GetAfter()[0].color == Color::empty)
			cFDT++;
		else if (islands[i]->GetBody().size() == 2 && islands[i]->GetBefore()[0].color == Color::empty && islands[i]->GetAfter()[0].color == Color::empty &&
			((islands[i]->GetAfter()[1].color == islands[i]->GetColor() && islands[i]->GetAfter()[2].color == Color::empty) || (islands[i]->GetBefore()[1].color == islands[i]->GetColor() && islands[i]->GetBefore()[2].color == Color::empty)))
			cFDT++;
		else {
			islands.erase(islands.begin() + i);
			i--;
		}
	}
}

int Arbiter::simulation(int i, Case *tmp) {
	int meta = -1;
	tmp[i].property = CHECK_TURN(this->round);
	std::vector<Island *> islands = getIslands(INVERT_C(CHECK_TURN(this->round)), tmp);
	for (std::vector<Island *>::iterator it = islands.begin(); it != islands.end(); ++it) {
		if (((i % SIZE) == (*it)->GetBefore()[0].x && trunc(i / SIZE) == (*it)->GetBefore()[0].y) ||
			((i % SIZE) == (*it)->GetAfter()[0].x && trunc(i / SIZE) == (*it)->GetAfter()[0].y)) {
			tmp[(*it)->GetBody()[0].x + (SIZE * (*it)->GetBody()[0].y)].property = 0 | (1 << 2);
			tmp[(*it)->GetBody()[1].x + (SIZE * (*it)->GetBody()[1].y)].property = 0 | (1 << 2);
			meta = 0;
			break;
		}
	}
	if (dblThree) {
		filterFDT(islands);
		int j = 0;
		for (std::vector<Island *>::iterator it = islands.begin(); it != islands.end(); ++it) {
			for (std::vector<Coord>::const_iterator itt = (*it)->GetBody().begin(); itt != (*it)->GetBody().end(); ++itt) {
				if (findIntersec(islands, (*itt), j).size() > 0) {
					return 1;
				}
			}
			j++;
		}
	}
	return meta;
}

void Arbiter::generateMetadata() {
	int r = -1;
	for (size_t i = 0; i < (SIZE * SIZE); i++) {
		if (CAN_PUT_WXY(this->map[i])) {
			Case cpy[SIZE * SIZE];
			for (size_t j = 0; j < (SIZE * SIZE); j++)
				cpy[j].property = map[j].property;
			char tmp = this->map[i].property;
			if ((r = simulation(i, &(cpy[0]))) != -1)
				this->map[i].property = (r == 0) ? (GET_COLOR_WXY(this->map[i]) | (1 << 3)) : GET_COLOR_WXY(this->map[i]);
		}
	}
}
