//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "string_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

String_Parser_Node::String_Parser_Node(std::wstring&& str)
  : m_string(str)
{
}

String_Parser_Node_Ptr
String_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  std::wstring str;

  auto c = input.get();
  if (c != L'\"') {
    input.seekg(offset);
    return nullptr;
  }

  while ((c = input.get()) != L'\"') {
    str += c;
  }

  return std::make_shared<String_Parser_Node>(std::move(str));
}

void String_Parser_Node::dump(std::wostream& output) const
{
  output << L"L\"" << m_string << L"\"";
}

void String_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable(L\"");
  state->add_code(m_string);
  state->add_code(L"\")");
}
