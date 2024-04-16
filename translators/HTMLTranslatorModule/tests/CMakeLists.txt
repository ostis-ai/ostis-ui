set(HTML_TRANSLATOR_AGENT_TEST_SOURCES
    "units/HTMLTranslationAgentTest.cpp"
    "units/HTMLTranslationTest.cpp"
)
add_executable(ostis-ui-html-translator-agent-test ${HTML_TRANSLATOR_AGENT_TEST_SOURCES})

target_link_libraries(ostis-ui-html-translator-agent-test gtest_main ostis-ui-html-translator ostis-specified-template-agent sc-memory sc-core sc-agents-common sc-builder-lib)

target_include_directories(
    ostis-ui-html-translator-agent-test
    PRIVATE ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MEMORY_SRC} ${SC_TOOLS_SRC}/sc-builder/src ${SC_KPM_SRC} ${TEMPLATE_AGENT_MODULE_SRC})


add_definitions(-DHTML_TRANSLATION_AGENT_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

add_test(NAME ostis-ui-html-translator-agent-test COMMAND ostis-ui-html-translator-agent-test)
