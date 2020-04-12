//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "program_parser_node.hpp"
#include "line_parser_node.hpp"

#include "parser_node.hpp"
#include "scope_state.hpp"

#include <istream>
#include <vector>

Program_Parser_Node::Program_Parser_Node(Line_Parser_Node_Vector&& lines)
  : m_lines(std::move(lines))
{
}

Program_Parser_Node_Ptr
Program_Parser_Node::parse(Token_Stream& input)
{
  auto lines = parse_zero_or_more<Line_Parser_Node>(input);
  if (lines.empty()) {
    return nullptr;
  }

  return std::make_shared<Program_Parser_Node>(std::move(lines));
}

void Program_Parser_Node::dump(std::wostream& output) const
{
  for (const auto& l: m_lines) { l->dump(output); }
}

void Program_Parser_Node::generate_cpp(std::wostream& output) const
{
  auto state = std::make_shared<Scope_State>();

  for (const auto& l: m_lines) {
    l->collect_variables(state);
  }

  for (const auto& l: m_lines) {
    l->generate_cpp(state);
  }

  output << L"void rockstar_main()\n";
  output << L"{\n";

  output << state->get_code();

  output << L"}\n";
  output << L"\n";
}
