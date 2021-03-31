#pragma once

#include <string_view>
#include <algorithm>
#include "fmt/format.h"
#include "definitions.h"
#include "char_types.h"
#include "helpers.h"
#include "coder.h"

_NS_OSTR_BEGIN

class OPEN_STRING_EXPORT string_view
{
public:

	constexpr string_view() noexcept = default;

	constexpr string_view(const string_view&) noexcept = default;

	constexpr string_view(const char16_t* str) noexcept
		: _str(str)
	{}

	constexpr string_view(const char16_t* str, size_t count) noexcept
		: _str(str, count)
	{}

	constexpr string_view(std::u16string_view sv) noexcept
		: _str(sv)
	{}
	
	[[nodiscard]] int compare(const string_view& rhs) const noexcept;

	[[nodiscard]] bool operator==(const string_view& rhs) const noexcept;

	[[nodiscard]] inline bool operator!=(const string_view& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	[[nodiscard]] inline bool operator<(const string_view& rhs) const noexcept
	{
		return compare(rhs) < 0;
	}

	[[nodiscard]] inline bool operator<=(const string_view& rhs) const noexcept
	{
		return compare(rhs) <= 0;
	}

	[[nodiscard]] inline bool operator>(const string_view& rhs) const noexcept
	{
		return compare(rhs) > 0;
	}

	[[nodiscard]] inline bool operator>=(const string_view& rhs) const noexcept
	{
		return compare(rhs) >= 0;
	}

	[[nodiscard]] inline constexpr size_t origin_length() const noexcept
	{
		return _str.length();
	}

	[[nodiscard]] size_t length() const noexcept;

	[[nodiscard]] inline constexpr bool empty() const noexcept
	{
		return origin_length() == 0;
	}

	void remove_prefix(size_t count) noexcept;

	void remove_suffix(size_t count) noexcept;

	[[nodiscard]] string_view remove_prefix_copy(size_t count) noexcept;

	[[nodiscard]] string_view remove_suffix_copy(size_t count) noexcept;
	
	[[nodiscard]] string_view substring(size_t offset = 0, size_t count = SIZE_MAX) const;

	[[nodiscard]] size_t index_of(const string_view& pattern, case_sensitivity cs = case_sensitivity::sensitive) const;

	[[nodiscard]] size_t last_index_of(string_view pattern, case_sensitivity cs = case_sensitivity::sensitive) const;

	template<typename F>
	[[nodiscard]] size_t search(F&& predicate) const
	{
		return std::find_if(_str.cbegin(), _str.cend(), std::forward<F>(predicate)) - _str.cbegin();
	}

	bool split(const string_view& splitter, string_view* lhs, string_view* rhs) const;

	size_t split(const string_view& splitter, std::vector<string_view>& str, bool cull_empty = false) const;

	[[nodiscard]] inline bool start_with(const string_view& sv_start) const
	{
		return substring(0, sv_start.length()) == sv_start;
	}

	[[nodiscard]] inline bool ends_with(const string_view& sv_start) const
	{
		return substring(length() - sv_start.length(), sv_start.length()) == sv_start;
	}

	[[nodiscard]] string_view trim_start() const;

	[[nodiscard]] string_view trim_end() const;

	[[nodiscard]] inline string_view trim() const
	{
		return trim_start().trim_end();
	}

	[[nodiscard]] int to_int() const;

	// format string
	// format rule: fmtlib @ https://github.com/fmtlib/fmt
	template<typename...Args>
	[[nodiscard]] std::u16string format(Args&&...args) const
	{
		return fmt::format(_str, std::forward<Args>(args)...);
	}

	[[nodiscard]] constexpr std::u16string_view raw() const
	{
		return _str;
	}

	[[nodiscard]] bool to_utf8(std::string& u8) const
	{
		return coder::convert_append(_str, u8);
	}

private:

	size_t position_codepoint_to_index(size_t codepoint_count_to_iterator) const;

	size_t position_index_to_codepoint(size_t index) const;

	void convert_codepoint_into_index(size_t& from, size_t& count) const;

private:

	std::u16string_view _str;
};

namespace literal
{
#pragma warning(push)
#pragma warning(disable: 455)
	[[nodiscard]] inline constexpr string_view operator""o(const char16_t* str, size_t len)
	{
		return string_view(str, len);
	}
#pragma warning(pop)
}

_NS_OSTR_END 

template<>
struct fmt::formatter<ostr::string_view, char16_t>
{
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<typename FormatContext>
	auto format(ostr::string_view sv, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), u"{}", sv.raw());
	}
};

