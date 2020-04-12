//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "expression_list_parser_node.hpp"
#include "expression_list_separator_parser_node.hpp"
#include "simple_expression_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Expression_List_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_simple_expression->dump(output);
  m_expression_list_separator->dump(output);
  m_expression_list->dump(output);
}

void Expression_List_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_simple_expression->dump(output);
}

Expression_List_Parser_Node::Expression_List_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Expression_List_Parser_Node::Expression_List_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Expression_List_Parser_Node_Ptr
Expression_List_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_simple_expression = Simple_Expression_Parser_Node::parse(input);
  if (choice.m_simple_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_expression_list_separator = Expression_List_Separator_Parser_Node::parse(input);
  if (choice.m_expression_list_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_expression_list = Expression_List_Parser_Node::parse(input);
  if (choice.m_expression_list == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Expression_List_Parser_Node>(std::move(choice));
}

Expression_List_Parser_Node_Ptr
Expression_List_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_simple_expression = Simple_Expression_Parser_Node::parse(input);
  if (choice.m_simple_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Expression_List_Parser_Node>(std::move(choice));
}

// simple_expression expression_list_separator expression_list / simple_expression
Expression_List_Parser_Node_Ptr
Expression_List_Parser_Node::parse(Token_Stream& input)
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

void Expression_List_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output);}, m_choice);
}

void Expression_List_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_simple_expression->generate_cpp(state);
  auto op = state->get_expression_list_operator();
  state->add_code(op);
  m_expression_list->generate_cpp(state);
}

void Expression_List_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_simple_expression->generate_cpp(state);
}

void Expression_List_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state);}, m_choice);
}


void Expression_List_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_simple_expression->collect_variables(state);
  m_expression_list->collect_variables(state);
}

void Expression_List_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_simple_expression->collect_variables(state);
}

void Expression_List_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.collect_variables(state);}, m_choice);
}
