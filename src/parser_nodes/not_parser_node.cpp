//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "not_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "comparison_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

void Not_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword;
  m_separator->dump(output);
  m_not->dump(output);
}

void Not_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_comparison->dump(output);
}

Not_Parser_Node::Not_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Not_Parser_Node::Not_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Not_Parser_Node_Ptr
Not_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword = check_and_remove(input, L"not");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_not = Not_Parser_Node::parse(input);
  if (choice.m_not == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Not_Parser_Node>(std::move(choice));
}

Not_Parser_Node_Ptr
Not_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_comparison = Comparison_Parser_Node::parse(input);
  if (choice.m_comparison == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Not_Parser_Node>(std::move(choice));
}

Not_Parser_Node_Ptr
Not_Parser_Node::parse(Token_Stream& input)
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

void Not_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice);
}

void Not_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"!(");
  m_not->generate_cpp(state);
  state->add_code(L")");
}

void Not_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_comparison->generate_cpp(state);
}

void Not_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice);
}

void Not_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_not->collect_variables(state);
}

void Not_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_comparison->collect_variables(state);
}

void Not_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.collect_variables(state); }, m_choice);
}
