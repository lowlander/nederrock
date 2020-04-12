//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "letter_parser_node.hpp"
#include "uppercase_letter_parser_node.hpp"
#include "lowercase_letter_parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Letter_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_uppercase_letter->dump(output);
}

wchar_t Letter_Parser_Node::Choice_1::to_wchar_t() const
{
  return m_uppercase_letter->to_wchar_t();
}

void Letter_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_lowercase_letter->dump(output);
}

wchar_t Letter_Parser_Node::Choice_2::to_wchar_t() const
{
  return m_lowercase_letter->to_wchar_t();
}

Letter_Parser_Node::Letter_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Letter_Parser_Node::Letter_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Letter_Parser_Node_Ptr
Letter_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_uppercase_letter = Uppercase_Letter_Parser_Node::parse(input);
  if (choice.m_uppercase_letter == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Letter_Parser_Node>(std::move(choice));
}

Letter_Parser_Node_Ptr
Letter_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_lowercase_letter = Lowercase_Letter_Parser_Node::parse(input);
  if (choice.m_lowercase_letter == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Letter_Parser_Node>(std::move(choice));
}

Letter_Parser_Node_Ptr
Letter_Parser_Node::parse(Token_Stream& input)
{
  auto offset =input.tellg();

  auto node = parse_choice_1(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_2(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Letter_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

wchar_t Letter_Parser_Node::to_wchar_t() const
{
  return std::visit([](auto&& choice) -> wchar_t { return choice.to_wchar_t(); }, m_choice);
}
