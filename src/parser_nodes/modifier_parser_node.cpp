//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "modifier_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Modifier_Parser_Node::Modifier_Parser_Node(Separator_Parser_Node_Ptr&& separator_1,
                                           std::wstring&& keyword,
                                           Separator_Parser_Node_Ptr&& separator_2,
                                           Expression_Parser_Node_Ptr&& expression)
  : m_separator_1(std::move(separator_1))
  , m_keyword(std::move(keyword))
  , m_separator_2(std::move(separator_2))
  , m_expression(std::move(expression))
{
}

Modifier_Parser_Node_Ptr
Modifier_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto separator_1 = Separator_Parser_Node::parse(input);
  if (separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto keyword = check_and_remove(input, { L"with", L"using" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_2 = Separator_Parser_Node::parse(input);
  if (separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto expression = Expression_Parser_Node::parse(input);
  if (expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Modifier_Parser_Node>(std::move(separator_1),
                                                std::move(keyword),
                                                std::move(separator_2),
                                                std::move(expression));
}

void Modifier_Parser_Node::dump(std::wostream& output) const
{
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_expression->dump(output);
}

void Modifier_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"/* MODIFIER START */");
  m_expression->generate_cpp(state);
  state->add_code(L"/* MODIFIER END */");
}

void Modifier_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
}
