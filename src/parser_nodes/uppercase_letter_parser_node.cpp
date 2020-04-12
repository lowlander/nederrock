//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "uppercase_letter_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <locale>

Uppercase_Letter_Parser_Node::Uppercase_Letter_Parser_Node(wchar_t c)
  : m_char(c)
{
}

Uppercase_Letter_Parser_Node_Ptr
Uppercase_Letter_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto c = input.get();
  if (std::iswalpha(c) && std::iswupper(c)) {
    return std::make_shared<Uppercase_Letter_Parser_Node>(c);
  }

  input.seekg(offset);
  return nullptr;
}

void Uppercase_Letter_Parser_Node::dump(std::wostream& output) const
{
  output << m_char;
}

wchar_t Uppercase_Letter_Parser_Node::to_wchar_t() const
{
  return m_char;
}
