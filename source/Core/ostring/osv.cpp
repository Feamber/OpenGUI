#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Core/ostring/helpers.h"
#include <string_view>

_NS_OSTR_BEGIN

int string_view::compare(const string_view & rhs) const noexcept
{
	return _str.compare(rhs._str);
}

bool string_view::operator==(const string_view& rhs) const noexcept
{
	if (this->origin_length() != rhs.origin_length()) return false;
	return this->compare(rhs) == 0;
}

size_t string_view::length() const noexcept
{
	size_t count_sp = helper::string::count_surrogate_pair(_str.cbegin(), _str.cend());
	return _str.length() - count_sp;
}

string_view string_view::remove_prefix(size_t count) const noexcept
{
	string_view sv(*this);
	count = helper::string::codepoint_count_to_iterator(sv._str.cbegin(), count, sv._str.cend()) - sv._str.cbegin();
	sv._str.remove_prefix(count);
	return sv;
}

string_view string_view::remove_suffix(size_t count) const noexcept
{
	string_view sv(*this);
	count = helper::string::codepoint_count_to_iterator_backward(sv._str.crbegin(), count, sv._str.crend()) - sv._str.crbegin();
	sv._str.remove_suffix(count);
	return sv;
}

string_view string_view::left(size_t count) const noexcept
{
	return substring(0, count);
}

string_view string_view::right(size_t count) const noexcept
{
	string_view sv(*this);
	count = helper::string::codepoint_count_to_iterator_backward(sv._str.crbegin(), count, sv._str.crend()) - sv._str.crbegin();
	return sv._str.substr(origin_length() - count);
}

string_view string_view::substring(size_t offset, size_t count) const noexcept
{
	this->convert_codepoint_into_index(offset, count);
	return string_view(_str.substr(offset, count));
}

size_t string_view::index_of(const string_view& pattern, case_sensitivity cs) const noexcept
{
	auto& predicate = helper::character::case_predicate<wchar_t>(cs);

	auto it = std::search(
		_str.cbegin(), _str.cend()
		, pattern._str.cbegin(), pattern._str.cend(),
		predicate
	);

	if (it == _str.cend()) return SIZE_MAX;

	const size_t index_found = it - _str.cbegin();
	return this->position_index_to_codepoint(index_found);
}

size_t string_view::last_index_of(string_view pattern, case_sensitivity cs) const noexcept
{
	auto& predicate = helper::character::case_predicate<wchar_t>(cs);

	auto it = std::search(
		_str.crbegin(), _str.crend()
		, pattern._str.crbegin(), pattern._str.crend(),
		predicate
	);

	if (it == _str.crend()) return SIZE_MAX;
	const size_t index_found = _str.crend() - it - pattern.origin_length();
	return this->position_index_to_codepoint(index_found);
}

bool string_view::split(const string_view& splitter, string_view* lhs, string_view* rhs) const noexcept
{
	const size_t splitter_index = this->index_of(splitter);
	if (lhs) *lhs = this->substring(0, splitter_index);
	if (splitter_index == SIZE_MAX) return false;
	if (rhs) *rhs = this->substring(splitter_index + 1);
	return true;
}

size_t string_view::split(const string_view& splitter, std::vector<string_view>& str, bool cull_empty) const noexcept
{
	string_view lhs;
	string_view rhs = *this;
	size_t split_times = 0;
	while (rhs.split(splitter, &lhs, &rhs))
	{
		if (!cull_empty || !lhs.is_empty())
			str.push_back(std::move(lhs));
		++split_times;
	}
	if (!cull_empty || !rhs.is_empty())
		str.push_back(rhs);
	return split_times;
}

string_view string_view::trim_start() const noexcept
{
	size_t begin = 0;
	while (_str.data()[begin] == ' ') 
	{
		++begin;
	}
	return this->_str.substr(begin);
}

string_view string_view::trim_end() const noexcept
{
	size_t end = _str.size() - 1;
	while (_str.data()[end] == ' ') 
	{
		--end;
	}
	return this->_str.substr(0, end + 1);
}

int string_view::to_int() const noexcept
{
	int value = 0;
	string_view sv = this->trim();
	for (const auto& c : sv._str)
		value = value * 10 + c - u'0';
	return value;
}

float string_view::to_float() const noexcept
{
	float value = 0;
	bool is = false;
	float v = 10;
	string_view sv = this->trim();
	for (const auto& c : sv._str)
	{
		if(c == u'.')
		{
			is = true;
			continue;
		}
		if(!is)
			value = value * 10 + c - u'0';
		else
		{
			value += (float)(c - u'0') / v;
			v *= 10;
		};
	}
	return value;
}

bool string_view::to_bool() const noexcept
{
	string_view sv = this->trim();

	if(sv.length() == 1 && (sv._str.front() == '0' || sv._str.front() == '1'))
		return true;
	else if(sv.length() == 4
			&& (sv._str[0] == 't' || sv._str[0] == 'T')
			&& (sv._str[1] == 'r' || sv._str[1] == 'R')
			&& (sv._str[2] == 'u' || sv._str[2] == 'U')
			&& (sv._str[3] == 'e' || sv._str[3] == 'E'))
		return true;
	return false;
}

bool string_view::is_number() const noexcept
{
	bool is = false;
	string_view sv = this->trim();
	for (auto i = sv._str.begin(); i != sv._str.end(); ++i)
	{
		const auto& c = *i;
		if(helper::character::is_number(c))
			continue;
		else if(i == sv._str.begin() && (c == '-' || c == '+'))
			continue;
		else if(c == '.' && !is)
		{
			is = true;
			continue;
		}
		else
			return false;
	}
	return true;
}

bool string_view::is_integer() const noexcept
{
	string_view sv = this->trim();
	for (auto i = sv._str.begin(); i != sv._str.end(); ++i)
	{
		const auto& c = *i;
		if(helper::character::is_number(c))
			continue;
		else if(i == sv._str.begin() && (c == '-' || c == '+'))
			continue;
		else
			return false;
	}
	return true;
}

bool string_view::is_bool() const noexcept
{
	string_view sv = this->trim();

	if(sv.length() == 1 && (sv._str.front() == '0' || sv._str.front() == '1'))
		return true;
	else if(sv.length() == 4
			&& (sv._str[0] == 't' || sv._str[0] == 'T')
			&& (sv._str[1] == 'r' || sv._str[1] == 'R')
			&& (sv._str[2] == 'u' || sv._str[2] == 'U')
			&& (sv._str[3] == 'e' || sv._str[3] == 'E'))
		return true;
	else if(sv.length() == 5
			&& (sv._str[0] == 'f' || sv._str[0] == 'F')
			&& (sv._str[1] == 'a' || sv._str[1] == 'A')
			&& (sv._str[2] == 'l' || sv._str[2] == 'L')
			&& (sv._str[3] == 's' || sv._str[3] == 'S')
			&& (sv._str[4] == 'e' || sv._str[4] == 'E'))
		return true;
	return false;
}

size_t string_view::position_codepoint_to_index(size_t codepoint_count_to_iterator) const noexcept
{
	auto from_it = helper::string::codepoint_count_to_iterator(_str.cbegin(), codepoint_count_to_iterator, _str.cend());
	return from_it - _str.cbegin();
}

size_t string_view::position_index_to_codepoint(size_t index) const noexcept
{
	return index - helper::string::count_surrogate_pair(_str.cbegin(), _str.cbegin() + index);
}

void string_view::convert_codepoint_into_index(size_t& from, size_t& count) const noexcept
{
	const size_t index = this->position_codepoint_to_index(from);
	const size_t b = std::min(count, _str.size() - from);
	const size_t real_size = this->position_codepoint_to_index(from + b) - index;
	from = index;
	count = real_size;
}

_NS_OSTR_END
