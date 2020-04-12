function(add_rock_test NAME)
  add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.rock.cpp
  COMMAND
    nederrock -i ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.rock -o ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.rock.cpp
  DEPENDS
    nederrock
  )

  add_executable("${NAME}_exe" ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.rock.cpp)

  target_link_libraries(
    "${NAME}_exe" PRIVATE project_warnings)

  add_test(NAME "${NAME}_test"
          COMMAND "${NAME}_exe"
          -e ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.rock.out)
endfunction()

add_subdirectory(fixtures)
