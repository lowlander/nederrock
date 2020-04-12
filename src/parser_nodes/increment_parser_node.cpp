//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "increment_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "variable_parser_node.hpp"
#include "noise_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <vector>
#include <string>

Increment_Parser_Node::Rest::Rest(std::wstring&& keyword,
                                  Noise_Parser_Node_Vector&& noise)
  : m_keyword(std::move(keyword))
  , m_noise(std::move(noise))
{
}

void Increment_Parser_Node::Rest::dump(std::wostream& output) const
{
  output << m_keyword;
  for (const auto& n: m_noise) { n->dump(output); }
}

Increment_Parser_Node::Increment_Parser_Node(std::wstring&& keyword,
                                             Separator_Parser_Node_Ptr&& separator_1,
                                             Variable_Parser_Node_Ptr&& variable,
                                             Separator_Parser_Node_Ptr&& separator_2,
                                             Rest_Vector&& rest)
  : m_keyword(std::move(keyword))
  , m_separator_1(std::move(separator_1))
  , m_variable(std::move(variable))
  , m_separator_2(std::move(separator_2))
  , m_rest(std::move(rest))
{
}

Increment_Parser_Node::Rest_Ptr
Increment_Parser_Node::parse_rest(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, L"up");
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto noise = parse_zero_or_more<Noise_Parser_Node>(input);

  return std::make_shared<Rest>(std::move(keyword),
                                std::move(noise));
}

Increment_Parser_Node_Ptr
Increment_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, L"build");
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_1 = Separator_Parser_Node::parse(input);
  if (separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto variable = Variable_Parser_Node::parse(input);
  if (variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_2 = Separator_Parser_Node::parse(input);
  if (separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  Rest_Vector rest;
  while (true) {
    auto tmp_offset = input.tellg();

    auto r = parse_rest(input);
    if (r == nullptr) {
      input.seekg(tmp_offset);
      break;
    }

    rest.push_back(std::move(r));
  }

  if (rest.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Increment_Parser_Node>(std::move(keyword),
                                                 std::move(separator_1),
                                                 std::move(variable),
                                                 std::move(separator_2),
                                                 std::move(rest));
}

void Increment_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
  m_separator_1->dump(output);
  m_variable->dump(output);
  m_separator_2->dump(output);
  for (const auto& r: m_rest) { r->dump(output); }
}

void Increment_Parser_Node::Rest::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L" ++ ");
}

void Increment_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_variable->generate_cpp(state);
  for (const auto& r: m_rest) { r->generate_cpp(state); }
}

void Increment_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_variable->collect_variables(state);
}
