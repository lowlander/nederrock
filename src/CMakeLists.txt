#
# Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
#
# SPDX-License-Identifier: MIT
#

add_subdirectory(parser_nodes)

add_executable(
  nederrock

  main.cpp
  token_stream.cpp
  scope_state.cpp

  $<TARGET_OBJECTS:parser_nodes>
)

target_link_libraries(
  nederrock PRIVATE project_warnings)

install(TARGETS nederrock RUNTIME DESTINATION bin)
