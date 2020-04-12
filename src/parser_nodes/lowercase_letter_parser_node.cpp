//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "lowercase_letter_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <locale>

Lowercase_Letter_Parser_Node::Lowercase_Letter_Parser_Node(wchar_t c)
  : m_char(c)
{
}

Lowercase_Letter_Parser_Node_Ptr
Lowercase_Letter_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto c = input.get();
  if (std::iswalpha(c) && std::iswlower(c)) {
    return std::make_shared<Lowercase_Letter_Parser_Node>(c);
  }

  input.seekg(offset);
  return nullptr;
}

void Lowercase_Letter_Parser_Node::dump(std::wostream& output) const
{
  output << m_char;
}

wchar_t Lowercase_Letter_Parser_Node::to_wchar_t() const
{
  return m_char;
}
