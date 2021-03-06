
#include "Platform.h"
#include "Dictionary.h"
#include "Strict.h"

#include "Exceptions.h"
#include "DateUtils.h"
#include "CellUtils.h"
#include "DateTimeUtils.h"

void Dictionary_::Insert(const String_& key, const Cell_& val)
{
	const String_& k = String::Condensed(key);
	REQUIRE(!val_.count(k), "Duplicate key '" + k + "'");
	val_.insert(make_pair(k, val));
}

const Cell_& Dictionary::BlankCell()
{
	RETURN_STATIC(Cell_);
}

const Cell_& Dictionary_::At(const String_& key, bool optional) const
{
	const String_& k = String::Condensed(key);
	auto p = val_.find(k);
	if (p != val_.end())
		return p->second;
	REQUIRE(optional, "No value for key '" + k + "'");
	return Dictionary::BlankCell();
}

bool Dictionary_::Has(const String_& key) const
{
	return val_.count(String::Condensed(key)) > 0;
}

Handle_<Storable_> Dictionary::FindHandleBase
	(const std::map<String_, Handle_<Storable_>>& handles, 
	 const String_& key,
	 bool quiet)
{
	auto p = handles.find(String::Condensed(key));
	if (p == handles.end())
	{
		REQUIRE(quiet, "No handle found at '" + key + "'");
		return Handle_<Storable_>();
	}
	return p->second;
}

String_ Dictionary::ToString(const Dictionary_& d)
{
	String_ retval;
	for (const auto& kv : d)
	{
		if (!retval.empty())
			retval += ';';
		retval += kv.first;
		retval += '=';
		retval += Cell::CoerceToString(kv.second);
	}
	return retval;
}
Dictionary_ Dictionary::FromString(const String_& s)
{
	Vector_<String_> entries = String::Split(s, ';', false);
	Dictionary_ retval;
	for (const auto& kev : entries)
	{
		Vector_<String_> kv = String::Split(kev, '=', true);
		REQUIRE(kv.size() == 2, "Dictionary entry must have the form 'key=value'");
		REQUIRE(!kv.front().empty(), "Empty dictionary key");
		const String_& vs = kv.back();
		Cell_ value;
		if (String::IsNumber(vs))
			value = String::ToDouble(vs);
		else if (Date::IsDateString(vs))
			value = Date::FromString(vs);
		else if (DateTime::IsDateTimeString(vs))
			value = DateTime::FromString(vs);
		else if (!vs.empty())
			value = vs;
		retval.Insert(kv.front(), value);
	}
	return retval;
}