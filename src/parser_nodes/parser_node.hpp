//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_PARSER_NODE_HPP
#define NEDERROCK_SRC_PARSER_NODE_HPP

#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <cwctype>

#include "token_stream.hpp"


inline wchar_t to_lower(wchar_t c) {
  return static_cast<wchar_t>(std::towlower(static_cast<wint_t>(c)));
}

inline wchar_t to_lower(wint_t c) {
  return static_cast<wchar_t>(std::towlower(c));
}

inline bool iequals(std::wstring_view a, std::wstring_view b)
{
  if (a.size() != b.size()) {
    return false;
  }

  return std::equal(a.begin(), a.end(),
                    b.begin(), b.end(),
                    [](wchar_t ca, wchar_t cb) {
                        return to_lower(ca) == to_lower(cb);
                    });
}

template<class Type>
auto parse_zero_or_more(Token_Stream& input) -> typename std::vector<std::shared_ptr<Type>>
{
  typename std::shared_ptr<Type> tmp;
  typename std::vector<std::shared_ptr<Type>> res;

  while ((tmp = Type::parse(input)) != nullptr) {
    res.push_back(tmp);
  }

  return res;
}

inline std::wstring check_and_remove_single(Token_Stream& input, std::wstring_view str)
{
  std::wstring stack;

  auto offset = input.tellg();

  for (const auto& c: str) {
    const auto& tmp = input.get();
    if (tmp == WEOF || to_lower(tmp) != to_lower(c)) {
      // if there is no match, put all
      // the read chars back into the input stream
      input.seekg(offset);
      // return false to indicate there was no match
      return {};
    }

    stack += static_cast<wchar_t>(tmp);
  }

  // return true to indicate there was a match
  return stack;
}

inline std::wstring check_and_remove(Token_Stream& input, std::wstring_view str)
{
  return check_and_remove_single(input, str);
}

// TODO rename in get_one_of()

inline std::wstring check_and_remove(Token_Stream& input, std::initializer_list<std::wstring_view> str_list)
{
  auto offset = input.tellg();

  for (const auto& str: str_list) {
    auto res = check_and_remove_single(input, str);
    if (!res.empty()) {
      return res;
    }
  }

  input.seekg(offset);
  return {};
}

inline bool is_keyword(std::wstring_view s) {
  static const wchar_t* keywords[] = {
    L"mysterious",
    L"stronger",L"continue",
    L"between",L"greater",L"nothing",L"nowhere",L"smaller",L"whisper",L"without",
    L"ain\'t",L"around",L"bigger",L"listen",L"nobody", L"return", L"scream",L"taking",L"weaker",L"higher", L"strong",
    L"break",L"build",L"empty",L"false",L"great",L"knock",L"lower",L"right",L"round",L"shout",
    L"small",L"take",L"takes",L"times",L"until",L"unite",L"while",L"wrong",L"minus",
    L"aint",L"back",L"cast",L"burn",L"join",L"down",L"else",L"give",L"gone",L"high",L"into",L"less",L"lies",L"null",L"plus",L"says",L"than",L"them",L"they",L"true",L"weak",L"were",L"your",L"over",L"with",
    L"and",L"big",L"her",L"him",L"hir",L"it ",L"low",L"nor",L"not",L"put",L"say",L"she",L"the",L"top",L"ver",L"was",L"xem",L"yes",L"zie",L"zir",
    L"an",L"as",L"at",L"he",L"if",L"is",L"it",L"my",L"no",L"of",L"ok",L"or",L"to",L"up",L"ve", L"xe",L"ze",
    L"a"
  };

  for (const auto& k: keywords) {
    if (iequals(s, k)) {
      return true;
    }
  }

  return false;
}

inline std::wstring escape_variable_name(const std::wstring& name)
{
  std::wstring res;

  for (const auto& c: name) {
    if ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || (c == L'_')) {
      res += to_lower(c);
    } else {
      res += L'U' + std::to_wstring(static_cast<unsigned>(c));
    }
  }

  return res;
}

#endif // NEDERROCK_SRC_PARSER_NODE_HPP
