//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "common_variable_parser_node.hpp"
#include "common_prefix_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "letter_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Common_Variable_Parser_Node::Common_Variable_Parser_Node(Common_Prefix_Parser_Node_Ptr&& common_prefix,
                                                         Separator_Parser_Node_Vector&& separators,
                                                         Letter_Parser_Node_Vector&& letters)
  : m_common_prefix(std::move(common_prefix))
  , m_separators(std::move(separators))
  , m_letters(std::move(letters))
{
}

Common_Variable_Parser_Node_Ptr
Common_Variable_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto common_prefix = Common_Prefix_Parser_Node::parse(input);
  if (common_prefix == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto separators = parse_zero_or_more<Separator_Parser_Node>(input);
  if (separators.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto leters = parse_zero_or_more<Letter_Parser_Node>(input);
  if (leters.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Common_Variable_Parser_Node>(std::move(common_prefix),
                                                        std::move(separators),
                                                        std::move(leters));
}

void Common_Variable_Parser_Node::dump(std::wostream& output) const
{
  m_common_prefix->dump(output);
  for (const auto& s: m_separators) { s->dump(output); }
  for (const auto& l: m_letters) { l->dump(output); }
}

std::wstring Common_Variable_Parser_Node::to_string()
{
  std::wstring tmp;

  tmp += m_common_prefix->to_string();

  tmp += L"_";

  for (const auto& l: m_letters) {
    tmp += l->to_wchar_t();
  }

  return tmp;
}
