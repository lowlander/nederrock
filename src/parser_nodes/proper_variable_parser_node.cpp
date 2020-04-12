//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "proper_variable_parser_node.hpp"
#include "proper_noun_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <vector>

void Proper_Variable_Parser_Node::Noun::dump(std::wostream& output) const
{
  output << m_char;
  m_proper_noun->dump(output);
}

std::wstring Proper_Variable_Parser_Node::Noun::to_string() const
{
  std::wstring tmp;

  tmp += L"_";

  tmp += m_proper_noun->to_string();

  return tmp;
}

Proper_Variable_Parser_Node::Proper_Variable_Parser_Node(Proper_Noun_Parser_Node_Ptr&& proper_noun,
                                                         Proper_Variable_Parser_Node::Noun_Vector&& nouns)
  : m_proper_noun(std::move(proper_noun))
  , m_nouns(std::move(nouns))
{
}

Proper_Variable_Parser_Node_Ptr
Proper_Variable_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto proper_noun = Proper_Noun_Parser_Node::parse(input);
  if (proper_noun == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  Noun_Vector nouns;
  while (true) {
    auto tmp_offset = input.tellg();

    Noun noun;

    noun.m_char = static_cast<wchar_t>(input.get());
    if (noun.m_char != L' ') {
      input.seekg(tmp_offset);
      break;
    }

    noun.m_proper_noun = Proper_Noun_Parser_Node::parse(input);
    if (noun.m_proper_noun == nullptr) {
      input.seekg(tmp_offset);
      break;
    }

    nouns.push_back(std::move(noun));
  }

  return std::make_shared<Proper_Variable_Parser_Node>(std::move(proper_noun), std::move(nouns));
}

void Proper_Variable_Parser_Node::dump(std::wostream& output) const
{
  m_proper_noun->dump(output);
  for (const auto& n: m_nouns) { n.dump(output); }
}

std::wstring Proper_Variable_Parser_Node::to_string()
{
  std::wstring tmp;

  tmp += m_proper_noun->to_string();

  for (const auto& n: m_nouns) {
    tmp += n.to_string();
  }

  return tmp;
}
