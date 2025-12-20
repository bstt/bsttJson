#pragma once

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <string>
#include <vector>

#ifdef USE_BSTT_NAMESPACE
namespace bstt
{
#endif

	inline rapidjson::MemoryPoolAllocator<>& getAllocator()
	{
		static rapidjson::Document doc;
		return doc.GetAllocator();
	}

	template <typename T> void toJson(rapidjson::Value&, const T&)
	{
		static_assert(false, "ToJson not implemented for this type");
	}

	template <typename T> void add(rapidjson::Value& json, const T& value) { json.PushBack(value, getAllocator()); }
	inline void add(rapidjson::Value& json, const std::string& value)
	{
		rapidjson::Value val(value.c_str(), getAllocator());
		json.PushBack(val, getAllocator());
	}
	inline void add(rapidjson::Value& json, rapidjson::Value& value) { json.PushBack(value, getAllocator()); }
	template <typename T> void add(rapidjson::Value& json, const std::vector<T>& value)
	{
		for (const auto& v : value) add(json, v);
	}
	template <typename T, typename... Args> void add(rapidjson::Value& json, const T& value, Args&&... args)
	{
		add(json, value);
		add(json, std::forward(args)...);
	}

	template <typename T> void set(rapidjson::Value& json, const rapidjson::Value::StringRefType& key, const T& value)
	{
		if constexpr (std::is_constructible_v<rapidjson::Value, T>)
		{
			json.AddMember(key, value, getAllocator());
		}
		else
		{
			rapidjson::Value val;
			toJson(val, value);
			json.AddMember(key, val, getAllocator());
		}
	}
	inline void set(rapidjson::Value& json, const rapidjson::Value::StringRefType& key, const std::string& value)
	{
		rapidjson::Value val(value.c_str(), getAllocator());
		json.AddMember(key, val, getAllocator());
	}
	inline void set(rapidjson::Value& json, const rapidjson::Value::StringRefType& key, rapidjson::Value& value)
	{
		json.AddMember(key, value, getAllocator());
	}
	template <typename T>
	void set(rapidjson::Value& json, const rapidjson::Value::StringRefType& key, const std::vector<T>& value)
	{
		rapidjson::Value arr(rapidjson::Type::kArrayType);
		add(arr, value);
		json.AddMember(key, arr, getAllocator());
	}
	template <typename T, typename... Args>
	void set(rapidjson::Value& json, const rapidjson::Value::StringRefType& key, const T& value, Args&&... args)
	{
		set(json, key, value);
		set(json, std::forward<Args>(args)...);
	}

	inline std::string toString(const rapidjson::Value& json)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		json.Accept(writer);
		return buffer.GetString();
	}

	namespace p
	{
		template <typename T, typename U> T fromJson(const U& doc) { return U::template fromJson<T>(doc); }
	} // namespace p

	class DocWrapper
	{
	public:
		class ValueWrapper
		{

		public:
			ValueWrapper(const rapidjson::GenericValue<rapidjson::UTF8<>>& val) : val_(&val) {}

			operator int() const { return val_->GetInt(); }
			operator unsigned() const { return val_->GetUint(); }
			operator int64_t() const { return val_->GetInt64(); }
			operator uint64_t() const { return val_->GetUint64(); }
			operator double() const { return val_->GetDouble(); }
			operator float() const { return val_->GetFloat(); }
			operator bool() const { return val_->GetBool(); }
			operator std::string() const { return val_->GetString(); }
			template <typename T> operator std::vector<T>() const
			{
				std::vector<T> res;
				for (const auto& v : val_->GetArray()) res.push_back(static_cast<T>(ValueWrapper(v)));
				return res;
			}
			template <typename T> operator T() const { return p::fromJson<T>(*this); }

			template <typename T> T static fromJson(const ValueWrapper& doc) { return p::fromJson<T>(doc); }

			ValueWrapper operator[](const char* key) const { return ValueWrapper{(*val_)[key]}; }

			bool HasMember(const char* key) const { return val_->HasMember(key); }

			const rapidjson::GenericValue<rapidjson::UTF8<>>* val_; // has been set public for direct access if needed
		};

		template <typename T> T static fromJson(const ValueWrapper& doc) { return p::fromJson<T>(doc); }

		operator ValueWrapper() const { return ValueWrapper{doc_}; }

		rapidjson::GenericDocument<rapidjson::UTF8<>>& Parse(const char* json) { return doc_.Parse(json); }

		bool HasMember(const char* key) const { return doc_.HasMember(key); }

		ValueWrapper operator[](const char* key) const { return ValueWrapper{(doc_)[key]}; }

		rapidjson::Document doc_; // has been set public for direct access if needed
	};

	using ValueWrapper = DocWrapper::ValueWrapper;

#ifdef USE_BSTT_NAMESPACE
} // namespace bstt
#endif