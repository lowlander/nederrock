//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "function_parser_node.hpp"
#include "variable_parser_node.hpp"
#include "variable_list_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "eol_parser_node.hpp"
#include "block_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Function_Parser_Node::Function_Parser_Node(Variable_Parser_Node_Ptr&& variable,
                                           Separator_Parser_Node_Ptr&& separator_1,
                                           std::wstring&& keyword,
                                           Separator_Parser_Node_Ptr&& separator_2,
                                           Variable_List_Parser_Node_Ptr&& variable_list,
                                           Eol_Parser_Node_Ptr&& eol_1,
                                           Block_Parser_Node_Ptr&& block,
                                           Eol_Parser_Node_Ptr&& eol_2)
  : m_variable(std::move(variable))
  , m_separator_1(std::move(separator_1))
  , m_keyword(std::move(keyword))
  , m_separator_2(std::move(separator_2))
  , m_variable_list(std::move(variable_list))
  , m_eol_1(std::move(eol_1))
  , m_block(std::move(block))
  , m_eol_2(std::move(eol_2))
{
}

Function_Parser_Node_Ptr
Function_Parser_Node::parse(Token_Stream& input)
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

  auto keyword = check_and_remove(input, L"takes");
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_2 = Separator_Parser_Node::parse(input);
  if (separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto variable_list = Variable_List_Parser_Node::parse(input);
  if (variable_list == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto eol_1 = Eol_Parser_Node::parse(input);
  if (eol_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto block = Block_Parser_Node::parse(input);
  if (block == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto eol_2 = Eol_Parser_Node::parse(input);
  if (eol_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Function_Parser_Node>(std::move(variable),
                                                std::move(separator_1),
                                                std::move(keyword),
                                                std::move(separator_2),
                                                std::move(variable_list),
                                                std::move(eol_1),
                                                std::move(block),
                                                std::move(eol_2));
}

void Function_Parser_Node::dump(std::wostream& output) const
{
  m_variable->dump(output);
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_variable_list->dump(output);
  m_eol_1->dump(output);
  m_block->dump(output);
  m_eol_2->dump(output);
}

void Function_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  auto func_state = std::make_shared<Scope_State>(state);

  auto variable_count = m_variable_list->get_variable_count();

  state->add_function(m_variable->to_string());

  state->add_code(L"/* F1 */");

  state->add_code(L"std::function<rockstar::Variable (");
  if (variable_count > 0) {
    state->add_code(L"rockstar::Variable");
    for (size_t i = 1; i < variable_count; i++) {
      state->add_code(L", rockstar::Variable");
    }
  }

  state->add_code(L")> ");
  state->add_code(m_variable->to_string());
  state->add_code(L";\n");

  state->add_code(L"/* F2 */");

  state->add_code(m_variable->to_string());
  state->add_code(L" = [&](");
  m_variable_list->generate_cpp(state);
  state->add_code(L") -> rockstar::Variable\n{\n");

  state->add_code(L"/* F3 */");

  m_block->collect_variables(func_state);

  m_block->generate_cpp(func_state);

  state->add_code( func_state->get_code() );

  state->add_code(L"/* F4 */");

  state->add_code(L"return rockstar::Mysterious();\n");

  state->add_code(L"\n}; /* end function */\n");

  state->add_code(L"/* F5 */");
}

void Function_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  state->add_function(m_variable->to_string());
  m_variable_list->collect_variables(state);
}
