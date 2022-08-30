#include <string>
#include <map>;
namespace client{

class Player 
{
private:
	int id;
	Colour colour;
	std::string name; 
	std::pair<int, int> pos;
public:
	Player(int id, Colour colour);
	Player(int id, Colour colour, std::string name);
	Colour getColour();
	std::string getName();
	int getId();
	void setId(int id);
	void setName(std::string name);
	bool hasId();
	void setPos(int x, int y);
	std::pair<int, int> getPos();
};
enum Colour
{
	BLUE, RED, GREEN, MAGENTA, ORANGE, PINK, WHITE, BLACK, CYAN, DARK_GRAY

};
static std::map<Colour, std::string> const colourTable = {
	{BLUE,"blue"}, {RED,"red"}, {GREEN,"green"}, {MAGENTA,"magenta"}, {ORANGE, "orange"}, {PINK, "pink"}, {WHITE,"white"}, {BLACK,"black"}, {CYAN,"cyan"}, {DARK_GRAY, "DARK_GRAY"}
};
}