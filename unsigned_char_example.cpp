#include "./bsttJson.hpp"

#include <iostream>
#include <vector>

FROM_TO_JSON_CAST(unsigned char, int)

int main()
{
	std::vector<unsigned char> ucharList = {'h', 'e', 'l', 'l', 'o'};
	Json json = ucharList; // DO NOT WRITE: Json json = toJson(ucharList)
	std::cout << Json(json).toString() << std::endl;
	// DO NOT WRITE: std::vector<unsigned char> ucharList2 = fromJson<std::vector<unsigned char>>(json);
	std::vector<unsigned char> ucharList2 = json;
	for (unsigned char c : ucharList2) std::cout << c;
	std::cout << std::endl;
	return 0;
}
