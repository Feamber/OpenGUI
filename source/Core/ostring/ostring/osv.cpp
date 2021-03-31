
#include "ostring/osv.h"
#include <string_view>

_NS_OSTR_BEGIN

int string_view::compare(const string_view & rhs) const noexcept
{
	return _str.compare(rhs._str);
}

bool string_view::operator==(const string_view& rhs) const noexcept
{
	if (length() != rhs.length()) return false;
	return compare(rhs) == 0;
}

size_t string_view::length() const noexcept
{
	size_t count_sp = helper::string::count_surrogate_pair(_str.cbegin(), _str.cend());
	return _str.length() - count_sp;
}

void string_view::remove_prefix(size_t count) noexcept
{
	count = helper::string::codepoint_count_to_iterator(_str.cbegin(), count, _str.cend()) - _str.cbegin();
	_str.remove_prefix(count);
}

void string_view::remove_suffix(size_t count) noexcept
{
	count = helper::string::codepoint_count_to_iterator_backward(_str.crbegin(), count, _str.crend()) - _str.crbegin();
	_str.remove_suffix(count);
}

string_view string_view::remove_prefix_copy(size_t count) noexcept
{
	string_view sv(*this);
	sv.remove_prefix(count);
	return sv;
}

string_view string_view::remove_suffix_copy(size_t count) noexcept
{
	string_view sv(*this);
	sv.remove_suffix(count);
	return sv;
}

string_view string_view::substring(size_t offset, size_t count) const
{
	convert_codepoint_into_index(offset, count);
	return string_view(_str.substr(offset, count));
}

size_t string_view::index_of(const string_view& pattern, case_sensitivity cs) const
{
	auto& predicate = helper::character::case_predicate<wchar_t>(cs);

	auto it = std::search(
		_str.cbegin(), _str.cend()
		, pattern._str.cbegin(), pattern._str.cend(),
		predicate
	);

	if (it == _str.cend()) return SIZE_MAX;

	const size_t index_found = it - _str.cbegin();
	return position_index_to_codepoint(index_found);
}

size_t string_view::last_index_of(string_view pattern, case_sensitivity cs) const
{
	auto& predicate = helper::character::case_predicate<wchar_t>(cs);

	auto it = std::search(
		_str.crbegin(), _str.crend()
		, pattern._str.crbegin(), pattern._str.crend(),
		predicate
	);

	if (it == _str.crend()) return SIZE_MAX;
	const size_t index_found = _str.crend() - it - pattern.length();
	return position_index_to_codepoint(index_found);
}

bool string_view::split(const string_view& splitter, string_view* lhs, string_view* rhs) const
{
	const size_t splitter_index = index_of(splitter);
	if (splitter_index == SIZE_MAX) return false;
	if (lhs) *lhs = substring(0, splitter_index);
	if (rhs) *rhs = substring(splitter_index + 1);
	return true;
}

size_t string_view::split(const string_view& splitter, std::vector<string_view>& str, bool cull_empty) const
{
	string_view lhs;
	string_view rhs = *this;
	size_t split_times = 0;
	while (rhs.split(splitter, &lhs, &rhs))
	{
		if (!cull_empty || !lhs.empty())
			str.push_back(std::move(lhs));
		++split_times;
	}
	if (!cull_empty || !rhs.empty())
		str.push_back(rhs);
	return split_times;
}

string_view string_view::trim_start() const
{
	size_t begin = 0;
	while (_str.data()[begin] == ' ')
		++begin;
	return substring(begin);
}

string_view string_view::trim_end() const
{
	size_t end = index_of(u" ");
	return substring(0, end);
}

int string_view::to_int() const
{
	int value = 0;
	string_view sv = trim();
	for (const auto& c : sv._str)
		value = value * 10 + c - u'0';
	return value;
}

size_t string_view::position_codepoint_to_index(size_t codepoint_count_to_iterator) const
{
	auto from_it = helper::string::codepoint_count_to_iterator(_str.cbegin(), codepoint_count_to_iterator, _str.cend());
	return from_it - _str.cbegin();
}

size_t string_view::position_index_to_codepoint(size_t index) const
{
	return index - helper::string::count_surrogate_pair(_str.cbegin(), _str.cbegin() + index);
}

void string_view::convert_codepoint_into_index(size_t& from, size_t& count) const
{
	const size_t index = position_codepoint_to_index(from);
	const size_t b = std::min(count, _str.size() - from);
	const size_t real_size = position_codepoint_to_index(from + b) - index;
	from = index;
	count = real_size;
}

_NS_OSTR_END
