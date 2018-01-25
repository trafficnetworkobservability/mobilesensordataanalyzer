//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com)

//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html


//    This file is part of NeXTA Version 3 (Open-source).

//    NEXTA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    NEXTA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with NEXTA.  If not, see <http://www.gnu.org/licenses/>.
#pragma once
#include <vector>
#include <math.h>

void string_split(std::vector<std::string>& results, const std::string &source, const int fields);
// 
// enum empties_t { empties_ok, no_empties };
// template <typename Container>
// Container& string_split(
// 	Container&                            result,
// 	const typename Container::value_type& s,
// 	const typename Container::value_type& delimiters,
// 	empties_t                      empties = empties_ok)
// {
// 	result.clear();
// 	size_t current;
// 	size_t next = -1;
// 	do
// 	{
// 		if (empties == no_empties)
// 		{
// 			next = s.find_first_not_of(delimiters, next + 1);
// 			if (next == Container::value_type::npos) break;
// 			next -= 1;
// 		}
// 		current = next + 1;
// 		next = s.find_first_of(delimiters, current);
// 		result.push_back(s.substr(current, next - current));
// 	} while (next != Container::value_type::npos);
// 	return result;
// };

int g_read_integer(FILE* f, bool speicial_char_handling=false);
int g_read_integer_with_special_character(FILE* f, bool speicial_char_handling, char special_ch);

int read_2_integers_from_a_string(CString str, int &value1, int &value2);
int read_multiple_integers_from_a_string(CString str, std::vector<int> &vector);
bool g_detect_if_a_file_is_column_format(LPCTSTR lpszFileName);
int read_2_float_points_from_a_string(CString str, double &value1, double &value2);
int g_read_integer_with_char_O(FILE* f);

float g_read_float(FILE *f);
float g_read_float_from_a_line(FILE *f, int &end_of_line);

double  g_FindClosestYResolution(double Value);
double g_FindClosestMOEYResolution(double Value);

int  g_FindClosestTimeResolution(double Value);

int g_GetPrivateProfileInt( LPCTSTR section, LPCTSTR key, int def_value, LPCTSTR filename);
int g_WritePrivateProfileInt(LPCTSTR section, LPCTSTR key, int value, LPCTSTR filename);


std::string string_format(const std::string fmt, ...);
