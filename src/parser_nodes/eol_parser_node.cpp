//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "eol_parser_node.hpp"
#include "noise_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Eol_Parser_Node::Eol_Parser_Node(Noise_Parser_Node_Vector&& noise)
  : m_noise(std::move(noise))
{
}

Eol_Parser_Node_Ptr
Eol_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto noise = parse_zero_or_more<Noise_Parser_Node>(input);

  auto c = input.get();

  if (c == L'\n') {
    return std::make_shared<Eol_Parser_Node>(std::move(noise));
  }

  input.seekg(offset);
  return nullptr;
}

void Eol_Parser_Node::dump(std::wostream& output) const
{
  for (const auto& n: m_noise) { n->dump(output); }
  output << L'\n';
}
