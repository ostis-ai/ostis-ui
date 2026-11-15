set(SPECIFIED_TEMPLATE_AGENT_SOURCES
    "agent/SpecifiedStringTemplateAgent.cpp"
    "renderer/StringTemplateRenderer.cpp"
    "renderer/TemplateActionGenerator.cpp"
    "renderer/JsonToXMLAgent.cpp"
    "renderer/XMLToJsonAgent.cpp"
    "SpecifiedStringTemplateModule.cpp"
)
set(SPECIFIED_TEMPLATE_AGENT_HEADERS
    "agent/SpecifiedStringTemplateAgent.hpp"
    "keynodes/SpecifiedStringTemplateKeynodes.hpp"
    "renderer/StringTemplateRenderer.hpp"
    "SpecifiedStringTemplateModule.hpp"
    "renderer/JsonToXMLAgent.hpp"
    "renderer/XMLToJsonAgent.hpp"
    "renderer/TemplateActionGenerator.hpp"
)

add_library( 
    ostis-specified-template-agent 
    SHARED ${SPECIFIED_TEMPLATE_AGENT_SOURCES} ${SPECIFIED_TEMPLATE_AGENT_HEADERS}
)

target_include_directories(ostis-specified-template-agent
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

find_package(nlohmann_json REQUIRED)
find_package(inja REQUIRED)

target_link_libraries(ostis-specified-template-agent
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PRIVATE nlohmann_json::nlohmann_json
    LINK_PRIVATE pantor::inja
)

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()
