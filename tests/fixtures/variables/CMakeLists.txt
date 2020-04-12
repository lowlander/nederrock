add_rock_test(common_variables)
add_rock_test(globalVariables)
add_rock_test(poeticStrings)
add_rock_test(pronouns)
add_rock_test(proper_variables)
add_rock_test(simple_pronouns)
add_rock_test(simple_variables)

if (NOT ${MSVC})
#
# UTF8 output does not yet work on windows, skip test
#
add_rock_test(umlauts)
endif()
