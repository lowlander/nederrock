//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_INCREMENT_PARSER_NODE_HPP
#define NEDERROCK_SRC_INCREMENT_PARSER_NODE_HPP

#include "increment_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "variable_parser_node_pre.hpp"
#include "noise_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <vector>
#include <string>

class Increment_Parser_Node {
public:
  struct Rest {
    Rest(std::wstring&& keyword,
         Noise_Parser_Node_Vector&& noise);

    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    std::wstring              m_keyword;
    Noise_Parser_Node_Vector  m_noise;
  };

  using Rest_Ptr = std::shared_ptr<Rest>;
  using Rest_Vector = std::vector<Rest_Ptr>;
public:
  Increment_Parser_Node(std::wstring&& keyword,
                        Separator_Parser_Node_Ptr&& separator_1,
                        Variable_Parser_Node_Ptr&& variable,
                        Separator_Parser_Node_Ptr&& m_separator_2,
                        Rest_Vector&& rest);

  static Rest_Ptr parse_rest(Token_Stream& input);

  static Increment_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::wstring              m_keyword;
  Separator_Parser_Node_Ptr m_separator_1;
  Variable_Parser_Node_Ptr  m_variable;
  Separator_Parser_Node_Ptr m_separator_2;
  Rest_Vector               m_rest;
};

#endif // NEDERROCK_SRC_INCREMENT_PARSER_NODE_HPP
