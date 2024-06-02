# bsttJson

WARNING: This library will be deleted soon.

# Description

bsttJson is a **c++ library** that allows you to **read and write json data** with an easy way to convert to structured data.

# Features

- read and write json data
- convert structured data from and to json data
- easy to use (header only)

Note: This library is not intended to be the fastest or the most powerful, but the easiest to use.

# Installation

## Header only

Include the [`bsttJson.hpp`](bsttJson.hpp) anywhere you want to use it.

```cpp
#include "bsttJson.hpp"
```

### Use of namespace

If you want to use the namespace `bsttJson` you can define `USE_BSTT_NAMESPACE` before including the header.

```cpp
#define USE_BSTT_NAMESPACE
#include "bsttJson.hpp"
```

### Sort the keys

If you want to sort the keys of the objects alphabetically, you can define `SORT_JSON_OBJECT_KEYS` before including the header.

### Requirements

c++17 or later required for compilation.  
No external dependencies.

# Example

Content of [example.cpp](example.cpp)

```cpp
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
```

Output:

```
"reading"
"coding"
{
    "name": "Alice",
    "age": 25,
    "hobbies": [
        "reading",
        "coding"
    ],
    "address": {
        "street": "123 Main St",
        "city": "New York",
        "zipCode": "10001"
    }
}
```

# Usage

_The usage is not exhaustive._

```cpp
// CastType should be: bool, int, int64_t, size_t, double, std::string
#define FROM_TO_JSON_CAST(Type, CastType) // basic impl of fromJson<Type> and toJson<Type>
#ifdef SORT_JSON_OBJECT_KEYS
	using JsonObj = std::map<std::string, struct Json>;
#else
	using JsonObj = std::vector<std::pair<std::string, struct Json>>;
#endif
using JsonArr = std::vector<struct Json>;
template <typename T> T fromJson(const Json&);
template <typename T> Json toJson(const T&);
struct Json
{
	enum class Type
	{
		Null,
		Bool,
		Number,
		String,
		Array,
		Object
	};

	template <typename T> static Json::Type typeToType(const T&);
	static std::string typeToString(Type type);

	// Parsing functions

	static Json parse(const std::string_view& str);
	static bool tryParse(const std::string_view& str, Json& json);
	static bool tryParse(const std::string_view& str, Json& json, std::string& error);
	static Json parseFile(const std::string& fileName);
	static bool tryParseFile(const std::string& fileName, Json& json);
	static bool tryParseFile(const std::string& fileName, Json& json, std::string& error);

	// Constructors

	Json(); // default is null
	Json(const Json& v);
	template <typename T> Json(const T& v);

	// Converters

	template <typename T> Json& operator=(const T& t);
	template <typename T> operator T() const;

	operator bool&();
	operator double&();
	operator std::string&();
	operator JsonObj&();
	operator JsonArr&();

	// Value accessors

	template <typename T, typename... Args> void get(const std::string& key, T& value, Args&&... args) const;

	template <typename T, typename... Args> void set(const std::string& key, const T& value, Args&&... args);

	template <typename T, typename... Args> bool tryGet(const std::string& key, T& value, Args&&... args) const;

	bool hasKey(const std::string& key) const;

	// Array functions

	const Json& back();
	Json& back();
	const Json& operator[](size_t index) const;
	Json& operator[](size_t index); // resize the array if needed
	template <typename T> void emplace_back(const T& t);
	void resize(size_t size);

	// Object functions

	const Json& operator[](const std::string& key) const;
	Json& operator[](const std::string& key);

	// Display functions

	std::string toString(const std::string& tab = "", const std::string& newLine = "") const;
	friend std::ostream& operator<<(std::ostream& os, const Json& v);
	std::ostream& display(std::ostream& os, const std::string& tab = "", const std::string& newLine = "", size_t currentTabCount = 0) const;

	// Getters

	Type getType() const;
	size_t size() const;
};
```

**Note:** you should never manually call `fromJson` or `toJson`.

## Specific usage

For basic type like `unsigned char`, you can use the macro `FROM_TO_JSON_CAST` to quickly define the functions `fromJson` and `toJson`.

Content of [unsigned_char_example.cpp](unsigned_char_example.cpp)

```c++
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
```

Output:

```
[104, 101, 108, 108, 111]
hello
```

# Licence

MIT Licence. See [LICENSE file](LICENSE).
Please refer me with:

    Copyright (c) bstt All rights reserved.
