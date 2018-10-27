#include <fstream>
#include <string>
#include "Arbiter.hpp"

# define EXPORT_DLL __declspec(dllexport)

Arbiter Arbiter::m_instance = Arbiter();

extern "C"
{
	EXPORT_DLL void SetRules(bool brk, bool dbl) {
		Arbiter::Instance().SetRules(brk, dbl);
	}
	EXPORT_DLL void Play(unsigned int x, unsigned int y) {
		Arbiter::Instance().Play(x, y);
	}
	EXPORT_DLL char NextRound() {
		return Arbiter::Instance().NextRound();
	}
	EXPORT_DLL char PrevRound() {
		return Arbiter::Instance().PrevRound();
	}
	EXPORT_DLL char GetTurn() {
		return Arbiter::Instance().GetTurn();
	}
	EXPORT_DLL int GetMap(int x, int y) {
		return Arbiter::Instance().GetMap(x, y);
	}
	EXPORT_DLL char PutCheck(int x, int y, int m) {
		return Arbiter::Instance().PutCheck(x, y, m);
	}
	EXPORT_DLL int VictoryCheck() {
		return Arbiter::Instance().VictoryCheck();
	}
}



void printMap(int** m)
{
	std::cout << "   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18" << std::endl;
	for (size_t j = 0; j < SIZE; j++)
	{
		std::cout << j << " ";
		if (j < 10)
			std::cout << " ";
		for (size_t i = 0; i < SIZE; i++) {
			std::cout << m[j][i] << "  ";
		}
		std::cout << std::endl;
	}
}

int main (int ac, char **av) {
   std::string line;
   std::vector<int> map;
   std::ifstream myfile ("C:/Users/hdyla/Code/test");
   int j = 0;
   if (myfile.is_open())
   {
       std::cout << "   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18" << std::endl;
       while (getline(myfile,line))
       {
           std::cout << j << " ";
           if (j < 10)
           std::cout << " ";
           j++;
           for (size_t i = 0; i < SIZE; i++) {
               std::cout << ((int)line[i] - 48) << "  ";
               map.push_back((int)line[i] - 48);
           }
           std::cout << std::endl;
       }
       myfile.close();
   }
   else
   {
       std::cout << "Unable to open file";
       return -1;
   }
	int **Map = NULL;
	int Victory = 0;
	char **Put = NULL;
	char Turn = false;
	int x = 0;
	int y = 0;
	Arbiter::Instance().SetRules(true, true);
	
	for (size_t i = 0; i < (SIZE * SIZE); i++)
		{
		if (i == 0)
			std::cout << "BITE" << static_cast<int>(Arbiter::Instance().PutCheck(floor(i / SIZE), (i % SIZE), 1)) << std::endl;
		else
			std::cout << "BITE" << static_cast<int>(Arbiter::Instance().PutCheck(floor(i / SIZE), (i % SIZE), 0)) << std::endl;
	}

	return 0;
}
