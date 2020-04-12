//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "expression_list_separator_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

void Expression_List_Separator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  if (m_separator_1) m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
}

void Expression_List_Separator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  if (m_separator_1) m_separator_1->dump(output);
  output << m_keyword;
  if (m_separator_2) m_separator_2->dump(output);
}

Expression_List_Separator_Parser_Node::Expression_List_Separator_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Expression_List_Separator_Parser_Node::Expression_List_Separator_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Expression_List_Separator_Parser_Node_Ptr
Expression_List_Separator_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separator_1 = Separator_Parser_Node::parse(input);

  choice.m_keyword = check_and_remove(input, L", and");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Expression_List_Separator_Parser_Node>(std::move(choice));
}

Expression_List_Separator_Parser_Node_Ptr
Expression_List_Separator_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_separator_1 = Separator_Parser_Node::parse(input);

  choice.m_keyword = check_and_remove(input, { L"&", L",", L"'n'" } );
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);

  return std::make_shared<Expression_List_Separator_Parser_Node>(std::move(choice));
}

Expression_List_Separator_Parser_Node_Ptr
Expression_List_Separator_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

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

void Expression_List_Separator_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output);}, m_choice);
}
