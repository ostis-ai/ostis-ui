/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_test.hpp"
#include "scs_loader.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-memory/kpm/sc_agent.hpp"

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "agents/HTMLTranslatorAgent.hpp"
#include "agent/SpecifiedStringTemplateAgent.hpp"
#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

using namespace specifiedStringTemplateModule;
using namespace htmlTranslationModule;
using namespace scAgentsCommon;

namespace htmlTranslatorAgentTest
{
std::string const TEST_FILES_DIR_PATH = HTML_TRANSLATION_AGENT_TEST_SRC_PATH "/test-structures/";
size_t const WAIT_TIME = 1000;
using HTMLTranslatorAgentTest = ScMemoryTest;

void InitializeTest()
{
  CoreKeynodes::InitGlobal();
  HTMLTranslatorKeynodes::InitGlobal();
  SpecifiedStringTemplateKeynodes::InitGlobal();

  ScAgentInit(true);
  SC_AGENT_REGISTER(HTMLTranslatorAgent);
  SC_AGENT_REGISTER(SpecifiedStringTemplateAgent);
}

void ShutdownTest()
{
  SC_AGENT_UNREGISTER(HTMLTranslatorAgent);
  SC_AGENT_UNREGISTER(SpecifiedStringTemplateAgent);
}

void TestHTMLTranslatorAgent(ScMemoryContext & context, std::string const & scsTestFileName)
{
  InitializeTest();

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "ontology.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + scsTestFileName);

  // Call the agent, get and validate result
  ScAddr test_action_node = context.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(context.IsElement(test_action_node));
  ScAddr action_translate_ui_model_to_html = context.HelperFindBySystemIdtf("action_translate_ui_model_to_html");
  EXPECT_TRUE(context.IsElement(action_translate_ui_model_to_html));
  context.CreateEdge(ScType::EdgeAccessConstPosPerm, action_translate_ui_model_to_html, test_action_node);
  ScAddr result = utils::AgentUtils::applyActionAndGetResultIfExists(&context, test_action_node, WAIT_TIME);
  EXPECT_TRUE(context.IsElement(result));
  ScAddr resultLink = utils::IteratorUtils::getAnyFromSet(&context, result);
  EXPECT_TRUE(context.IsElement(resultLink));

  std::string resultLinkContent;
  context.GetLinkContent(resultLink, resultLinkContent);
  EXPECT_NE(resultLinkContent, "");

  // Check if the result is correct
  ScAddr string_template_expected_result = context.HelperFindBySystemIdtf("string_template_expected_result");
  std::string string_template_expected_result_content;
  context.GetLinkContent(string_template_expected_result, string_template_expected_result_content);
  EXPECT_EQ(string_template_expected_result_content, resultLinkContent);

  ShutdownTest();
}

TEST_F(HTMLTranslatorAgentTest, TranslateButton)
{
//  TestHTMLTranslatorAgent(*m_ctx, "test_translate_button.scs");
  ScMemoryContext & context = *m_ctx;
  InitializeTest();

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "ontology.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_translate_button.scs");

  // Call the agent, get and validate result
  ScAddr test_action_node = context.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(context.IsElement(test_action_node));
  ScAddr action_translate_ui_model_to_html = context.HelperFindBySystemIdtf("action_translate_ui_model_to_html");
  EXPECT_TRUE(context.IsElement(action_translate_ui_model_to_html));
  context.CreateEdge(ScType::EdgeAccessConstPosPerm, action_translate_ui_model_to_html, test_action_node);
  ScAddr result = utils::AgentUtils::applyActionAndGetResultIfExists(&context, test_action_node, WAIT_TIME);
  EXPECT_TRUE(context.IsElement(result));
  ScAddr resultLink = utils::IteratorUtils::getAnyFromSet(&context, result);
  EXPECT_TRUE(context.IsElement(resultLink));

  std::string resultLinkContent;
  context.GetLinkContent(resultLink, resultLinkContent);
  EXPECT_NE(resultLinkContent, "");

  // Check if the result is correct
  ScAddr string_template_expected_result = context.HelperFindBySystemIdtf("string_template_expected_result");
  std::string string_template_expected_result_content;
  context.GetLinkContent(string_template_expected_result, string_template_expected_result_content);
  EXPECT_EQ(string_template_expected_result_content, resultLinkContent);

  ShutdownTest();
}

} // namespace htmlTranslatorAgentTest
