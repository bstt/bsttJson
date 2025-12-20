# bsttRapidJson

# Description

bsttRapidJson is a **c++ library** that allows you to **read and write json data** with an easy way to convert to structured data. It is a wrapper around [rapidjson](https://github.com/Tencent/rapidjson).

# Features

- read and write json data
- convert structured data from and to json data
- easy to use (header only)

Note: This library is not intended to be the fastest or the most powerful, but the easiest to use.  
BUT, you can expect pretty good performance with the use of rapidjson under the hood.

# Installation

## Header only

Include the [`bsttRapidjson.hpp`](bsttRapidjson.hpp) anywhere you want to use it.

You will require the whole include folder of rapidjson.  
Get it from this project [here](rapidjson) or get the latest version [here](https://github.com/Tencent/rapidjson/tree/master/include/rapidjson).

You can use https://download-directory.github.io/ to download github folders.

```cpp
#include "bsttRapidjson.hpp"
```

### Requirements

c++17 or later required for compilation.  
No external dependencies (except rapidjson include folder).

# Example

Content of [example.cpp](example.cpp)

```cpp
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

```

Output:

```
{"name":"Alice","age":25,"isStudent":{"v":false},"hobbies":["reading","coding"],"bestFriend":{"name":"Bob","age":24,"isStudent":{"v":true},"hobbies":["gaming","music"]}}
```

# Usage

_The usage is not exhaustive._

```cpp
template <typename T> void toJson(rapidjson::Value&, const T&); // function to implement for custom types
template <typename T, typename... Args> void add(rapidjson::Value& json, const T& value, Args&&... args);

template <typename T, typename... Args>
void set(rapidjson::Value& json, const rapidjson::Value::StringRefType& key, const T& value, Args&&... args);
inline std::string toString(const rapidjson::Value& json);

namespace p
{
	template <typename T, typename U> T fromJson(const U& doc); // function to implement for custom types, keep namespace
} // namespace p

class DocWrapper
{
public:
	class ValueWrapper
	{

	public:
		ValueWrapper(const rapidjson::GenericValue<rapidjson::UTF8<>>& val);

		template <typename T> operator std::vector<T>() const;
		template <typename T> operator T() const;

		ValueWrapper operator[](const char* key) const;

		bool HasMember(const char* key) const;

		const rapidjson::GenericValue<rapidjson::UTF8<>>* val_; // has been set public for direct access if needed
	};

	operator ValueWrapper() const;

	rapidjson::GenericDocument<rapidjson::UTF8<>>& Parse(const char* json);

	bool HasMember(const char* key) const;

	ValueWrapper operator[](const char* key) const;

	rapidjson::Document doc_; // has been set public for direct access if needed
};

using ValueWrapper = DocWrapper::ValueWrapper;
```

**Note:** you should never manually call `fromJson` or `toJson` within the implementation of a `fromJson` or a `toJson`.

# Licence

MIT Licence. See [LICENSE file](LICENSE).
Please refer me with:

    Copyright (c) bstt All rights reserved.
