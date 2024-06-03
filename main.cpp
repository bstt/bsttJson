#include "./bsttJson.hpp"

#include <string>
#include <vector>

#include <iostream>

struct Person
{
	std::string name;
	int age;
	bool isStudent;
	std::vector<double> scoreList;
	Person* friend_;
};

template <> inline Json toJson<Person>(const Person& person)
{
	Json json
		= JsonObj{{"name", person.name}, {"age", person.age}, {"isStudent", person.isStudent}, {"scoreList", person.scoreList}};
	if (person.friend_) json["friend"] = *person.friend_;
	return json;
}
template <> inline Person fromJson<Person>(const Json& json)
{
	Person person{json["name"], json["age"], json["isStudent"], json["scoreList"], nullptr};
	// you can also use the method get (cf. below) for type checking
	// json.get("name", person.name, "age", person.age, "isStudent", person.isStudent, "scoreList", person.scoreList);
	json.tryGet("friend", *person.friend_);
	return person;
}

int main()
{
	Person bstt{"bstt", 20, true, {100.0, 95.3, 98.7}, nullptr};

	Json json2 = Json::parse(R"({"age": 18,"isStudent": true,"name": "rui","scoreList": [99.3, 97.5, 96.8]})");
	// browse scoreList without converting to Person
	const JsonArr& scoreList = json2["scoreList"];
	for (const Json& score : scoreList) std::cout << score << std::endl;
	// to browse key-value pairs, use JsonObj (i.e. const JsonObj& obj = json2;)

	Person rui = json2; // convert json to Person

	rui.friend_ = &bstt;

	Json json = rui; // convert Person to json

	std::cout << json.toString("    ", "\n") << std::endl; // use toString function to format json

	return 0;
}