//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "whitespace_parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Whitespace_Parser_Node::Whitespace_Parser_Node(wchar_t c)
  : m_char(c)
{
}

Whitespace_Parser_Node_Ptr
Whitespace_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto c = input.get();
  if (c == L' ' || c == L'\t') {
    return std::make_shared<Whitespace_Parser_Node>(c);
  } else {
    input.seekg(offset);
    return nullptr;
  }
}

void
Whitespace_Parser_Node::dump(std::wostream& output) const
{
  output << m_char;
}
