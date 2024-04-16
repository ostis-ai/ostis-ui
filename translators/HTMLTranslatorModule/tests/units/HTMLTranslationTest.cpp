/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scs_loader.hpp"
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

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "ontology.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + scsTestFileName);

  ScAddr rootUiElement = context.HelperFindBySystemIdtf("button");
  EXPECT_TRUE(context.IsElement(rootUiElement));

  ScAddr resultLink = HTMLTranslator::TranslateScToHTML(context, rootUiElement);
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

TEST_F(HTMLTranslatorTest, TranslateButton)
{
  TestHTMLTranslator(*m_ctx, "test_translate_button.scs");
}

} // namespace htmlTranslatorTest
