//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "product_parser_node.hpp"
#include "expression_list_parser_node.hpp"
#include "divide_parser_node.hpp"
#include "multiply_parser_node.hpp"
#include "simple_expression_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <vector>
#include <variant>

void Product_Parser_Node::Rest::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice->dump(output); }, m_mul_div);
  m_expression_list->dump(output);
}

void Product_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_simple_expression->dump(output);
  for (const auto& r: m_rest) { r->dump(output); }
}

void Product_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_expression_list->dump(output);
}

void Product_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_simple_expression->dump(output);
}

Product_Parser_Node::Product_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Product_Parser_Node::Product_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Product_Parser_Node::Product_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Product_Parser_Node::Rest::Ptr
Product_Parser_Node::parse_rest(Token_Stream& input)
{
  auto offset = input.tellg();

  auto rest = std::make_shared<Rest>();

  auto mul = Multiply_Parser_Node::parse(input);
  if (mul) {
    rest->m_mul_div = mul;
  } else {
    input.seekg(offset);

    auto div = Divide_Parser_Node::parse(input);
    if (div) {
      rest->m_mul_div = div;
    } else {
      input.seekg(offset);
      return nullptr;
    }
  }

  rest->m_expression_list = Expression_List_Parser_Node::parse(input);
  if (rest->m_expression_list == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return rest;
}

Product_Parser_Node_Ptr
Product_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_simple_expression = Simple_Expression_Parser_Node::parse(input);
  if (choice.m_simple_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  while (auto rest_entry = parse_rest(input)) {
    choice.m_rest.push_back(rest_entry);
  }

  if (choice.m_rest.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Product_Parser_Node>(std::move(choice));
}

Product_Parser_Node_Ptr
Product_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_expression_list = Expression_List_Parser_Node::parse(input);
  if (choice.m_expression_list == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Product_Parser_Node>(std::move(choice));
}

Product_Parser_Node_Ptr
Product_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_simple_expression = Simple_Expression_Parser_Node::parse(input);
  if (choice.m_simple_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Product_Parser_Node>(std::move(choice));
}

Product_Parser_Node_Ptr
Product_Parser_Node::parse(Token_Stream& input)
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

  node = parse_choice_3(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Product_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Product_Parser_Node::Rest::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice->generate_cpp(state); }, m_mul_div);
  m_expression_list->generate_cpp(state);
}

void Product_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_simple_expression->generate_cpp(state);
  for (const auto& r: m_rest) { r->generate_cpp(state); }
}

void Product_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_expression_list->generate_cpp(state);
}

void Product_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_simple_expression->generate_cpp(state);
}

void Product_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Product_Parser_Node::Rest::collect_variables(Scope_State_Ptr state) const
{
  m_expression_list->collect_variables(state);
}

void Product_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_simple_expression->collect_variables(state);
  for (const auto& r: m_rest) { r->collect_variables(state); }
}

void Product_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_expression_list->collect_variables(state);
}

void Product_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  m_simple_expression->collect_variables(state);
}

void Product_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
