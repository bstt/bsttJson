# bsttJson

# Description

bsttJson is a **c++ library** that allows you to **read and write json data** with an easy way to convert to structured data.

# Features

- read and write json data
- convert structured data from and to json data
- easy to use (header only)

Note: This library is not intended to be the fastest or the most powerful, but to be easy to use.

Other note: the keys are alphabetically ordered.

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

### Requirements

c++17 or later required for compilation.  
No external dependencies.

# Example

Content of [main.cpp](main.cpp)
```cpp
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
	Person person{json["name"], json["name"], json["isStudent"], json["score"], nullptr};
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
```

Output:
```
{
    "age": 18,
    "friend": {
        "age": 20,
        "isStudent": true,
        "name": "bstt",
        "score": 100
    },
    "isStudent": true,
    "name": "rui",
    "score": 99.3
}
```

# Usage

*The usage is not exhaustive.*

```cpp
using JsonObj = std::map<std::string, struct Json>;
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

	// Value accessors

	template <typename T, typename... Args> void get(const std::string& key, T& value, Args&&... args) const;

	template <typename T, typename... Args> void set(const std::string& key, const T& value, Args&&... args);

	template <typename T, typename... Args> bool tryGet(const std::string& key, T& value, Args&&... args) const;

	// Array functions

	const Json& operator[](size_t index) const { return arr[index]; }
	Json& operator[](size_t index);
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
};
```

# Licence

MIT Licence. See [LICENSE file](LICENSE).
Please refer me with:

	Copyright (c) bstt All rights reserved.