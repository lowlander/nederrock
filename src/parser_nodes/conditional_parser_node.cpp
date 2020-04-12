//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "conditional_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_parser_node.hpp"
#include "consequent_parser_node.hpp"
#include "alternate_parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Conditional_Parser_Node::Conditional_Parser_Node(std::wstring&& keyword,
                                                 Separator_Parser_Node_Ptr&& separator,
                                                 Expression_Parser_Node_Ptr&& expression,
                                                 Consequent_Parser_Node_Ptr&& consequent,
                                                 Alternate_Parser_Node_Ptr&& alternate)
  : m_keyword(std::move(keyword))
  , m_separator(std::move(separator))
  , m_expression(std::move(expression))
  , m_consequent(std::move(consequent))
  , m_alternate(std::move(alternate))
{
}

Conditional_Parser_Node_Ptr
Conditional_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, L"if");
  if (keyword.empty()){
    input.seekg(offset);
    return nullptr;
  }

  auto separator = Separator_Parser_Node::parse(input);
  if (separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto expression = Expression_Parser_Node::parse(input);
  if (separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto consequent = Consequent_Parser_Node::parse(input);

  auto alternate = Alternate_Parser_Node::parse(input);

  return std::make_shared<Conditional_Parser_Node>(std::move(keyword),
                                                   std::move(separator),
                                                   std::move(expression),
                                                   std::move(consequent),
                                                   std::move(alternate));
}

void Conditional_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
  m_separator->dump(output);
  m_expression->dump(output);
  if (m_consequent) { m_consequent->dump(output); }
  if (m_alternate) { m_alternate->dump(output); }
}

void Conditional_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"if (");
  m_expression->generate_cpp(state);
  state->add_code(L"){\n");
  if (m_consequent) {
    m_consequent->generate_cpp(state);
  } else {
    state->add_code(L"/* empty if block */");
  }
  state->add_code(L"} else {\n");
  if (m_alternate) {
    m_alternate->generate_cpp(state);
  } else {
    state->add_code(L"/* empty else block */");
  }
  state->add_code(L"} /* end if */\n");
}

void Conditional_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
  if (m_consequent) { m_consequent->collect_variables(state); }
  if (m_alternate) { m_alternate->collect_variables(state); }
}
