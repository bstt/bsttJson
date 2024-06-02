#include "./bsttJson.hpp"

#include <iostream>

struct Person
{
	std::string name;
	int age;
	bool isStudent;
	double score;
	Person* friend_;
};

template <> inline Json toJson<Person>(const Person& person)
{
	Json json = JsonObj{{"name", person.name}, {"age", person.age}, {"isStudent", person.isStudent}, {"score", person.score}};
	if (person.friend_) json["friend"] = *person.friend_;
	return json;
}
template <> inline Person fromJson<Person>(const Json& json)
{
	Person person{json["name"], json["age"], json["isStudent"], json["score"], nullptr};
	// you can also use the method get (cf. below) for type checking
	// json.get("name", person.name, "age", person.age, "isStudent", person.isStudent, "score", person.score);
	json.tryGet("friend", *person.friend_);
	return person;
}

int main()
{
	Person bstt{"bstt", 20, true, 100.0, nullptr};

	Json json2 = Json::parse(R"({"age": 18,"isStudent": true,"name": "rui","score": 99.3})");
	Person rui = json2;

	rui.friend_ = &bstt;

	Json json = rui;

	std::cout << json.toString("    ", "\n") << std::endl;

	return 0;
}