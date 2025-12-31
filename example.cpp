#include "./bsttJson.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct Address
{
	std::string street;
	std::string city;
	std::string zipCode;
};

struct Person
{
	std::string name;
	int age;
	std::optional<Address> address;
	std::vector<std::string> hobbies;
};

template <> inline Json toJson<Address>(const Address& address)
{
	return JsonObj{{"street", address.street}, {"city", address.city}, {"zipCode", address.zipCode}};
}

template <> inline Address fromJson<Address>(const Json& json) { return Address{json["street"], json["city"], json["zipCode"]}; }

template <> inline Json toJson<Person>(const Person& person)
{
	Json json = JsonObj{{"name", person.name}, {"age", person.age}, {"hobbies", person.hobbies}};
	if (person.address.has_value()) json["address"] = person.address.value();
	return json;
}

template <> inline Person fromJson<Person>(const Json& json)
{
	Person person{json["name"], json["age"], {}, json["hobbies"]};
	// you can also use the method get (cf. below) for type checking
	// json.get("name", person.name, "age", person.age, "hobbies", person.hobbies);
	if (json.hasKey("address"))
	{
		person.address = Address(json["address"]);
	}
	return person;
}

int main()
{
	std::string jsonStr = R"(
    {
        "name": "Alice",
        "age": 25,
        "hobbies": ["reading", "coding"]
    }
    )";
	Json json2 = Json::parse(jsonStr);
	// browse hobbies without converting to Person
	const JsonArr& hobbies = json2["hobbies"];
	for (const Json& hobby : hobbies) std::cout << hobby << '\n';
	// to browse key-value pairs, use JsonObj (i.e. const JsonObj& obj = json2;)

	Person alice = json2; // convert json to Person

	alice.address = Address{"123 Main St", "New York", "10001"};

	Json json = alice; // convert Person to json

	std::cout << json.toString("    ", "\n") << '\n'; // use toString function to format json

	return 0;
}