#include "Resource.h"

Resource::Resource()
{
	if (!font.loadFromFile("Resources/Fonts/uni.ttf")) {
		std::cout << "[Failed to load CResource font]";
	}
}

void Resource::Draw()
{

}
