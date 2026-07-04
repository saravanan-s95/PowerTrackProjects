include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Microwave_Oven_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(Microwave_Oven_default_default_XC8_FILE_TYPE_assemble)
add_library(Microwave_Oven_default_default_XC8_assemble OBJECT ${Microwave_Oven_default_default_XC8_FILE_TYPE_assemble})
    Microwave_Oven_default_default_XC8_assemble_rule(Microwave_Oven_default_default_XC8_assemble)
    list(APPEND Microwave_Oven_default_library_list "$<TARGET_OBJECTS:Microwave_Oven_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(Microwave_Oven_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(Microwave_Oven_default_default_XC8_assemblePreprocess OBJECT ${Microwave_Oven_default_default_XC8_FILE_TYPE_assemblePreprocess})
    Microwave_Oven_default_default_XC8_assemblePreprocess_rule(Microwave_Oven_default_default_XC8_assemblePreprocess)
    list(APPEND Microwave_Oven_default_library_list "$<TARGET_OBJECTS:Microwave_Oven_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(Microwave_Oven_default_default_XC8_FILE_TYPE_compile)
add_library(Microwave_Oven_default_default_XC8_compile OBJECT ${Microwave_Oven_default_default_XC8_FILE_TYPE_compile})
    Microwave_Oven_default_default_XC8_compile_rule(Microwave_Oven_default_default_XC8_compile)
    list(APPEND Microwave_Oven_default_library_list "$<TARGET_OBJECTS:Microwave_Oven_default_default_XC8_compile>")

endif()


# Main target for this project
add_executable(Microwave_Oven_default_image_5SuR20Gv ${Microwave_Oven_default_library_list})

set_target_properties(Microwave_Oven_default_image_5SuR20Gv PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${Microwave_Oven_default_output_dir}")
target_link_libraries(Microwave_Oven_default_image_5SuR20Gv PRIVATE ${Microwave_Oven_default_default_XC8_FILE_TYPE_link})

# Add the link options from the rule file.
Microwave_Oven_default_link_rule( Microwave_Oven_default_image_5SuR20Gv)


