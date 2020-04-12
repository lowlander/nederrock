//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "function_call_parser_node.hpp"
#include "variable_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_list_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Function_Call_Parser_Node::Function_Call_Parser_Node(Variable_Parser_Node_Ptr&& variable,
                                                     Separator_Parser_Node_Ptr&& separator_1,
                                                     std::wstring&& keyword,
                                                     Separator_Parser_Node_Ptr&& separator_2,
                                                     Expression_List_Parser_Node_Ptr&& expression_list)
  : m_variable(std::move(variable))
  , m_separator_1(std::move(separator_1))
  , m_keyword(std::move(keyword))
  , m_separator_2(std::move(separator_2))
  , m_expression_list(std::move(expression_list))
{
}

Function_Call_Parser_Node_Ptr
Function_Call_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto variable = Variable_Parser_Node::parse(input);
  if (variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_1 = Separator_Parser_Node::parse(input);
  if (separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto keyword = check_and_remove(input, L"taking");
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_2 = Separator_Parser_Node::parse(input);
  if (separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto expression_list = Expression_List_Parser_Node::parse(input);
  if (expression_list == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Function_Call_Parser_Node>(std::move(variable),
                                                     std::move(separator_1),
                                                     std::move(keyword),
                                                     std::move(separator_2),
                                                     std::move(expression_list));
}

void Function_Call_Parser_Node::dump(std::wostream& output) const
{
  m_variable->dump(output);
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_expression_list->dump(output);
}

void Function_Call_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_variable->generate_cpp(state);
  state->add_code(L"(");
  state->set_expression_list_operator(L",");
  m_expression_list->generate_cpp(state);
  state->add_code(L")");
}

void Function_Call_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_variable->collect_variables(state);
  m_expression_list->collect_variables(state);
}
