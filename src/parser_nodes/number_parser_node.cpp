//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "number_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Number_Parser_Node::Number_Parser_Node(std::wstring&& str)
  : m_string(str)
{
}

Number_Parser_Node_Ptr Number_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  std::wstring s;

  bool allow_minus = true;
  bool allow_dot = true;

  while (true) {
    auto tmp_offset = input.tellg();
    auto c = input.get();

    if (c == L'-') {
      if (allow_minus == false) {
        input.seekg(tmp_offset);
        break;
      }
    } else if (c == L'.') {
      if (allow_dot == false) {
        input.seekg(tmp_offset);
        break;
      }
      allow_dot = false;
    } else if (c == L'0' || c == L'1' || c == L'2' || c == L'3' || c == L'4' || c == L'5' || c == L'6' || c == L'7' || c == L'8' || c == L'9') {
      // nr OK
    } else {
      input.seekg(tmp_offset);
      break;
    }

    s += static_cast<wchar_t>(c);
    allow_minus = false;
  }



  if (s.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  if (s.size() == 1 && (s[0] == L'-' || s[0] == L'.')) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Number_Parser_Node>(std::move(s));
}

void Number_Parser_Node::dump(std::wostream& output) const
{
  output << m_string;
}

void Number_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable(L\"");
  state->add_code(m_string);
  state->add_code(L"\",");
  state->add_code(m_string);
  state->add_code(L")");
}
