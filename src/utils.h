/*
Copyright (C) 2018 Perpendicular Dimensions

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/* Replacement for strstr(haystack, needle) ==/!= nullptr */
template<typename T>
inline bool contains(const std::string &haystack, T needle) { return haystack.find(needle) != haystack.npos; }

template<typename T>
inline bool contains(const std::string_view &haystack, T needle) { return haystack.find(needle) != haystack.npos; }

inline bool contains(const char *haystack, const char *needle) { return strstr(haystack, needle); }

/* Quick and dirty split(string, separator) that returns string_views into the original string */
class split {
	std::string_view str;
	char sep;

	split(char sep): sep(sep) {}

	public:
	split(const std::string_view str, char sep): str(str), sep(sep) {
		auto beg = str.find_first_not_of(sep);
		if (beg && beg != str.npos)
			this->str.remove_prefix(beg);
	}

	split(const std::vector<char> &vec, char sep): str(vec.data(), vec.size()), sep(sep) {
		auto beg = str.find_first_not_of(sep);
		if (beg && beg != str.npos)
			this->str.remove_prefix(beg);
	}

	split begin() { return *this; };
	split end() { return split(sep); }

	bool operator!=(const split &other) const { return other.sep != sep || ((other.str.size() || str.size()) && other.str != str); }

	std::string_view operator*() {
		auto pos = str.find(sep);
		if (pos == str.npos)
			return str;
		else
			return str.substr(0, pos);
	}

	split &operator++() {
		auto pos = str.find(sep);
		if (pos == str.npos) {
			str = {};
		} else {
			auto beg = str.find_first_not_of(sep, pos);
			if (beg == str.npos)
				str = {};
			else
				str.remove_prefix(beg);
		}
		return *this;
	}

	std::vector<std::string_view> to_vector() {
		std::vector<std::string_view> tokens;
		for (auto &&token: *this)
			tokens.push_back(token);
		return tokens;
	}
};

/* Replacement for sscanf() that works with strings */
template<typename... Args>
int scan(const std::string &str, const char *format, Args... args) { return sscanf(str.c_str(), format, args...); }

template<typename... Args>
int scan(const std::string_view &sv, const char *format, Args... args) { return sscanf(std::string(sv).c_str(), format, args...); }

/* A version of stoi() that works on string_views. */
int stoi(std::string_view str);
