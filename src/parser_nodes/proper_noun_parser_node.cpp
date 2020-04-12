//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "proper_noun_parser_node.hpp"
#include "uppercase_letter_parser_node.hpp"
#include "letter_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Proper_Noun_Parser_Node::Proper_Noun_Parser_Node(Uppercase_Letter_Parser_Node_Ptr&& uppercase_letter,
                                                 Letter_Parser_Node_Vector&& letters)
  : m_uppercase_letter(std::move(uppercase_letter))
  , m_letters(std::move(letters))
{
}

Proper_Noun_Parser_Node_Ptr
Proper_Noun_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto uppercase_letter = Uppercase_Letter_Parser_Node::parse(input);
  if (uppercase_letter == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto letters = parse_zero_or_more<Letter_Parser_Node>(input);

  std::wstring tmp;
  tmp += uppercase_letter->to_wchar_t();

  for (const auto& l: letters) {
    tmp += l->to_wchar_t();
  }

  if (is_keyword(tmp)) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Proper_Noun_Parser_Node>(std::move(uppercase_letter), std::move(letters));
}

void Proper_Noun_Parser_Node::dump(std::wostream& output) const
{
  m_uppercase_letter->dump(output);
  for (const auto& l: m_letters) { l->dump(output); }
}

std::wstring Proper_Noun_Parser_Node::to_string()
{
  std::wstring tmp;

  tmp += m_uppercase_letter->to_wchar_t();

  for (const auto& l: m_letters) {
    tmp += l->to_wchar_t();
  }

  return tmp;
}
