/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-memory/utils/sc_log.hpp"
#include "sc-search/search_keynodes.h"
#include "scs_loader.hpp"
#include "sc_repo_path_collector.hpp"
#include "sc_test.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/kpm/sc_agent.hpp"

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

#include "agents/HTMLTranslatorAgent.hpp"
#include "agent/SpecifiedStringTemplateAgent.hpp"

#include "html-translator/HTMLTranslator.hpp"

using namespace specifiedStringTemplateModule;
using namespace htmlTranslationModule;

namespace htmlTranslatorTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = HTML_TRANSLATION_AGENT_TEST_SRC_PATH "/test-structures/";
std::string const PROJECT_REPO_PATH = PROJECT_REPO_PATH_PATH;

using HTMLTranslatorTest = ScMemoryTest;

void InitializeTest()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  HTMLTranslatorKeynodes::InitGlobal();
  SpecifiedStringTemplateKeynodes::InitGlobal();

  SC_AGENT_REGISTER(HTMLTranslatorAgent);
  SC_AGENT_REGISTER(SpecifiedStringTemplateAgent);
}

void ShutdownTest()
{
  SC_AGENT_UNREGISTER(HTMLTranslatorAgent);
  SC_AGENT_UNREGISTER(SpecifiedStringTemplateAgent);
}

void TestHTMLTranslator(ScMemoryContext & context, std::string const & scsTestFileName)
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
  for (auto const & source : buildSources) 
  {
    loader.loadScsFile(context, source);
  }  

  ScAddr actionNode = context.HelperFindBySystemIdtf("test_action_node");
  ScAddr rootUiElement = utils::IteratorUtils::getAnyByOutRelation(&context, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  EXPECT_TRUE(context.IsElement(rootUiElement));
  ScAddr resultLink;
  resultLink = HTMLTranslator::TranslateScToHTML(context, rootUiElement);
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

TEST_F(HTMLTranslatorTest, TranslateTextOutput)
{
  TestHTMLTranslator(*m_ctx, "test_translate_text_output.scs");
}

TEST_F(HTMLTranslatorTest, TranslateDecompositionButton)
{
  TestHTMLTranslator(*m_ctx, "test_translate_button_decomposition.scs");
}
} // namespace htmlTranslatorTest
