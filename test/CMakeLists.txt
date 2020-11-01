find_package(Boost REQUIRED unit_test_framework)

add_executable(mqtt_test mqtt_test.cc)
target_link_libraries(mqtt_test 
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

add_executable(redis_test redis_test.cc)
target_link_libraries(redis_test 
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)