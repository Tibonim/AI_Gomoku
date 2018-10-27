#ifndef ISLAND_HPP
# define ISLAND_HPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <utility>
#include <tuple>
#include <algorithm>
#include "Case.hpp"


class                   Island {
private:
    std::vector<Coord>  body;
	Coord				beforEx[3];
	Coord				afterEx[3];
    Color               color;
public:
    Island(Color col);
    void AddBack(unsigned int x, unsigned int y, Color c);
    void AddBack(Coord const&);
    void AddFront(unsigned int x, unsigned int y, Color c);
    void AddFront(Coord const&);
    Color GetColor();
    std::vector<Coord> const &GetBody();
    Coord const *GetBefore();
    void SetBefore(unsigned int x, unsigned int y, Color c, int id);
    Coord const *GetAfter();
    void SetAfter(unsigned int x, unsigned int y, Color c, int id);
    virtual ~Island();
};

#endif /* !ISLAND_HPP */
