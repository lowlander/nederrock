//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "subtract_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Subtract_Parser_Node::Subtract_Parser_Node(Separator_Parser_Node_Vector&& separators_1,
                                           std::wstring&& keyword,
                                           Separator_Parser_Node_Vector&& separators_2)
  : m_separators_1(std::move(separators_1))
  , m_keyword(std::move(keyword))
  , m_separators_2(std::move(separators_2))
{
}

Subtract_Parser_Node_Ptr
Subtract_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto separators_1 = parse_zero_or_more<Separator_Parser_Node>(input);

  auto keyword = check_and_remove(input, { L"-", L"minus ", L"without " } );
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separators_2 = parse_zero_or_more<Separator_Parser_Node>(input);

  return std::make_shared<Subtract_Parser_Node>(std::move(separators_1),
                                            std::move(keyword),
                                            std::move(separators_2));
}

void Subtract_Parser_Node::dump(std::wostream& output) const
{
  for (const auto& s: m_separators_1) { s->dump(output); }
  output << m_keyword;
  for (const auto& s: m_separators_2) { s->dump(output); }
}


void Subtract_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"-");
  state->set_expression_list_operator(L"-");
}
