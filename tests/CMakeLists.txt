project(unit_tests C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

add_executable(test_user_mqttFunctions
    test_user_mqttFunctions.c
    stub_user_system.c
    ../Core/Src/user_mqttFunctions.c
)

target_include_directories(test_user_mqttFunctions BEFORE PRIVATE . ../Core/Inc)

add_test(NAME test_user_mqttFunctions COMMAND test_user_mqttFunctions)
