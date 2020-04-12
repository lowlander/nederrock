//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "output_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Output_Parser_Node::Output_Parser_Node(std::wstring&& keyword,
                                       Separator_Parser_Node_Ptr&&  separator,
                                       Expression_Parser_Node_Ptr&& expresion)
  : m_keyword(std::move(keyword))
  , m_separator(std::move(separator))
  , m_expression(std::move(expresion))
{
}

Output_Parser_Node_Ptr
Output_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"say", L"shout", L"whisper", L"scream" } );
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator = Separator_Parser_Node::parse(input);
  if (separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto expression = Expression_Parser_Node::parse(input);
  if (expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Output_Parser_Node>(std::move(keyword),
                                              std::move(separator),
                                              std::move(expression));
}

void Output_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
  m_separator->dump(output);
  m_expression->dump(output);
}

void Output_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::print(");
  m_expression->generate_cpp(state);
  state->add_code(L")");
}

void Output_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
}
