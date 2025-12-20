#include "bsttRapidjson.hpp"

#include <iostream>
#include <string>
#include <vector>

struct IsStudent
{
	bool v;
};

struct Person
{
	std::string name;
	int age;
	IsStudent isStudent;
	std::vector<std::string> hobbies;
	Person* bestFriend = nullptr;
};

using namespace rapidjson;

template <> void toJson(Value& value, const IsStudent& a)
{
	value.SetObject();
	set(value, "v", a.v);
}

template <> void toJson(Value& value, const Person& p)
{
	value.SetObject();
	set(value, "name", p.name, "age", p.age, "isStudent", p.isStudent, "hobbies", p.hobbies);
	if (p.bestFriend) set(value, "bestFriend", *p.bestFriend);
}

// fromJson specializations must be defined with p:: namespace

template <> IsStudent p::fromJson(const ValueWrapper& doc) { return IsStudent{doc}; }

template <> Person p::fromJson(const ValueWrapper& doc)
{
	Person res{doc["name"], doc["age"], doc["isStudent"], doc["hobbies"]};
	if (doc.HasMember("bestFriend")) res.bestFriend = new Person(doc["bestFriend"]);
	return res;
}

int main()
{
	std::string json = R"(
    {
        "name": "Alice",
        "age": 25,
        "isStudent": false,
        "hobbies": ["reading", "coding"]
    }
    )";
	DocWrapper doc;
	doc.Parse(json.c_str());
	auto p = p::fromJson<Person>(doc);
	p.bestFriend = new Person{"Bob", 24, {true}, {"gaming", "music"}};
	Value v;
	toJson(v, p);
	std::string serialized = toString(v);
	std::cout << serialized << "\n";
}
