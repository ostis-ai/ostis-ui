/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_test.hpp"
#include "sc_repo_path_collector.hpp"
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
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = HTML_TRANSLATION_AGENT_TEST_SRC_PATH "/test-structures/";
std::string const PROJECT_REPO_PATH = PROJECT_REPO_PATH_PATH;
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

  ScRepoPathCollector collector;
  ScRepoPathCollector::Sources excludeSources;
  ScRepoPathCollector::Sources checkSources;
  ScRepoPathCollector::Sources buildSources;
  collector.ParseRepoPath(PROJECT_REPO_PATH, excludeSources, checkSources);
  collector.CollectBuildSources("", excludeSources, checkSources, buildSources);
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + scsTestFileName);
  // load KB sources of the project
  for (std::string const & source : buildSources)
  {
    loader.loadScsFile(context, source);
  }

  // Call the agent, get and validate result
  ScAddr actionNode = context.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(context.IsElement(actionNode));
  ScAddr rootUiElement = utils::IteratorUtils::getAnyByOutRelation(&context, actionNode, CoreKeynodes::rrel_1);
  EXPECT_TRUE(context.IsElement(rootUiElement));
  ScAddr result = utils::AgentUtils::applyActionAndGetResultIfExists(&context, HTMLTranslatorKeynodes::action_translate_sc_node_to_html, {rootUiElement}, WAIT_TIME);
  EXPECT_TRUE(context.IsElement(result));
  ScAddr resultLink = utils::IteratorUtils::getAnyFromSet(&context, result);
  EXPECT_TRUE(context.IsElement(resultLink));

  std::string resultLinkContent;
  context.GetLinkContent(resultLink, resultLinkContent);
  EXPECT_NE(resultLinkContent, "");

  // Check if the result is correct
  ScAddr stringTemplateExpectedResult = utils::IteratorUtils::getAnyByOutRelation(&context, actionNode, context.HelperResolveSystemIdtf("rrel_expected_result"));
  std::string stringTemplateExpectedResultContent;
  context.GetLinkContent(stringTemplateExpectedResult, stringTemplateExpectedResultContent);
  EXPECT_EQ(stringTemplateExpectedResultContent, resultLinkContent);

  ShutdownTest();
}

TEST_F(HTMLTranslatorAgentTest, TranslateTextOutput)
{
  TestHTMLTranslatorAgent(*m_ctx, "test_translate_text_output.scs");
}

TEST_F(HTMLTranslatorAgentTest, TranslateButtonDecomposition)
{
  TestHTMLTranslatorAgent(*m_ctx, "test_translate_button_decomposition.scs");
}

} // namespace htmlTranslatorAgentTest
