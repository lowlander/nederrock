//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "target_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "assignable_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Target_Parser_Node::Target_Parser_Node(Separator_Parser_Node_Ptr&& separator_1,
                                       std::wstring&& keyword,
                                       Separator_Parser_Node_Ptr&& separator_2,
                                       Assignable_Parser_Node_Ptr&& assignable)
  : m_separator_1(separator_1)
  , m_keyword(keyword)
  , m_separator_2(separator_2)
  , m_assignable(assignable)
{
}

Target_Parser_Node_Ptr
Target_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto separator_1 = Separator_Parser_Node::parse(input);
  if (separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto keyword = check_and_remove(input, L"into");
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator_2 = Separator_Parser_Node::parse(input);
  if (separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto assignable = Assignable_Parser_Node::parse(input);
  if (assignable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Target_Parser_Node>(std::move(separator_1),
                                              std::move(keyword),
                                              std::move(separator_2),
                                              std::move(assignable));
}

void Target_Parser_Node::dump(std::wostream& output) const
{
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_assignable->dump(output);
}

void Target_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_assignable->generate_cpp(state);
}

void Target_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_assignable->collect_variables(state);
}
