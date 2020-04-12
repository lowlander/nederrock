//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "arithmetic_parser_node.hpp"
#include "product_parser_node.hpp"
#include "add_parser_node.hpp"
#include "subtract_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <vector>
#include <variant>

void Arithmetic_Parser_Node::Rest::dump(std::wostream& output) const
{
  std::visit([&output](auto&& node) { node->dump(output); }, m_add_sub);
  m_product->dump(output);
}

void Arithmetic_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_product->dump(output);
  for (const auto& r: m_rest) {
    r->dump(output);
  }
}

void Arithmetic_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_product->dump(output);
}

Arithmetic_Parser_Node::Arithmetic_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Arithmetic_Parser_Node::Arithmetic_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Arithmetic_Parser_Node::Rest_Ptr
Arithmetic_Parser_Node::parse_rest(Token_Stream& input)
{
  auto offset = input.tellg();

  auto rest = std::make_shared<Rest>();

  auto add = Add_Parser_Node::parse(input);
  if (add) {
    rest->m_add_sub = add;
  } else {
    input.seekg(offset);

    auto sub = Subtract_Parser_Node::parse(input);
    if (sub) {
      rest->m_add_sub = sub;
    } else {
      input.seekg(offset);
      return nullptr;
    }
  }

  rest->m_product = Product_Parser_Node::parse(input);
  if (rest->m_product == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return rest;
}

Arithmetic_Parser_Node_Ptr
Arithmetic_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_product = Product_Parser_Node::parse(input);
  if (choice.m_product == nullptr) {
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

  return std::make_shared<Arithmetic_Parser_Node>(std::move(choice));
}

Arithmetic_Parser_Node_Ptr
Arithmetic_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_product = Product_Parser_Node::parse(input);
  if (choice.m_product == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Arithmetic_Parser_Node>(std::move(choice));
}

Arithmetic_Parser_Node_Ptr
Arithmetic_Parser_Node::parse(Token_Stream& input)
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

void Arithmetic_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice);
}

void Arithmetic_Parser_Node::Rest::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice->generate_cpp(state); }, m_add_sub);
  m_product->generate_cpp(state);
}

void Arithmetic_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_product->generate_cpp(state);
  for (const auto& r: m_rest) { r->generate_cpp(state); }
}

void Arithmetic_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_product->generate_cpp(state);
}

void Arithmetic_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice);
}

void Arithmetic_Parser_Node::Rest::collect_variables(Scope_State_Ptr state) const
{
  m_product->collect_variables(state);
}

void Arithmetic_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_product->collect_variables(state);
  for (const auto& r: m_rest) { r->collect_variables(state); }
}

void Arithmetic_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_product->collect_variables(state);
}

void Arithmetic_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.collect_variables(state); }, m_choice);
}
