# groups.cmake

# group Source Group 1
add_library(Group_Source_Group_1 OBJECT
  "${SOLUTION_ROOT}/lab4task1.c"
)
target_include_directories(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Source_Group_1 PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
