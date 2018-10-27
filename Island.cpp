#include "Island.hpp"

Island::Island(Color col) : color(col) {
    Coord init = {0, 0, Color::illegal};
	beforEx[0] = init;
	beforEx[1] = init;
	beforEx[2] = init;
	afterEx[0] = init;
	afterEx[1] = init;
	afterEx[2] = init;
}

Island::~Island()
{

}

void Island::AddBack(unsigned int x, unsigned int y, Color c) {
	unsigned int col = static_cast<unsigned int>(c);
    Coord tmp = {x, y, col};
    body.push_back(tmp);
};

void Island::AddBack(Coord const& old) {
    Coord tmp = old;
    body.push_back(tmp);
};

void Island::AddFront(unsigned int x, unsigned int y, Color c) {
	unsigned int col = static_cast<unsigned int>(c);
    Coord tmp = {x, y, col};
    body.insert (body.begin(), tmp);
};

void Island::AddFront(Coord const& old) {
    Coord tmp = old;
    body.insert (body.begin(), tmp);
};

Color Island::GetColor() {
    return color;
};

std::vector<Coord> const &Island::GetBody() {
    return body;
}

Coord const *Island::GetBefore() {
    return beforEx;
};


void Island::SetBefore(unsigned int x, unsigned int y, Color c, int id) {
	unsigned int col = static_cast<unsigned int>(c);
    beforEx[id] = {x, y, col};
}

Coord const *Island::GetAfter() {
    return afterEx;
};

void Island::SetAfter(unsigned int x, unsigned int y, Color c, int id) {
	unsigned int col = static_cast<unsigned int>(c);
    afterEx[id] = {x, y, col};
}