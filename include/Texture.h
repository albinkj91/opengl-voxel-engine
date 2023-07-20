#include <string>

class Texture
{
public:
	Texture(string const& filename);

	unsigned char* load();
private:
	string filename;
}
