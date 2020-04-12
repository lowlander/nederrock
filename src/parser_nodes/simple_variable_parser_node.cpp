//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "simple_variable_parser_node.hpp"
#include "letter_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Simple_Variable_Parser_Node::Simple_Variable_Parser_Node(Letter_Parser_Node_Vector&& letters)
  : m_letters(std::move(letters))
{
}

Simple_Variable_Parser_Node_Ptr
Simple_Variable_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto letters = parse_zero_or_more<Letter_Parser_Node>(input);
  if (letters.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  std::wstring tmp;
  for (const auto& l: letters) {
    tmp += l->to_wchar_t();
  }

  if (is_keyword(tmp)) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Simple_Variable_Parser_Node>(std::move(letters));
}

void Simple_Variable_Parser_Node::dump(std::wostream& output) const
{
  for (const auto& l: m_letters) { l->dump(output); }
}

std::wstring Simple_Variable_Parser_Node::to_string()
{
  std::wstring tmp;

  for (const auto& l: m_letters) {
    tmp += l->to_wchar_t();
  }

  return tmp;
}
