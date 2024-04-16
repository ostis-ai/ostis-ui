/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "HTMLTranslator.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "keynodes/HTMLTranslatorKeynodes.hpp"

using namespace utils;

namespace htmlTranslationModule
{
ScAddr HTMLTranslator::TranslateScToHTML(ScMemoryContext & context, ScAddr const & rootUiElement)
{
  if (!context.IsElement(rootUiElement))
  {
    SC_LOG_ERROR("HTMLTranslator: ui element is invalid.");
    return {};
  }

  // Check if the element is translated or have hardcoded html representation
  ScAddr answerHTMLLink = IteratorUtils::getAnyByOutRelation(&context, rootUiElement, HTMLTranslatorKeynodes::nrel_html_representation);
  if (context.IsElement(answerHTMLLink))
  {
    return answerHTMLLink;
  }

  ScAddr uiHTMLTemplateLink = GetUIComponentHTMLTemplate(context, rootUiElement);
  if (!context.IsElement(uiHTMLTemplateLink))
  {
    return answerHTMLLink;
  }

  answerHTMLLink = GetTemplateAgentAnswerLink(context, rootUiElement, uiHTMLTemplateLink);
  if (!context.IsElement(answerHTMLLink))
  {
    return answerHTMLLink;
  }

  // Get root element decomposition and iterate over all the child elements
//  ScAddrVector uiElementDecomposition = IteratorUtils::getAllByOutRelation(&context, rootUiElement, HTMLTranslatorKeynodes::nrel_decomposition);
//  for (ScAddr const & childUIElement : uiElementDecomposition)
//  {
//
//  }

  return answerHTMLLink;
}

ScAddr HTMLTranslator::GetUIComponentHTMLTemplate(ScMemoryContext & context, ScAddr const & uiComponent)
{
  ScAddr HTMLTemplateLink;
  std::string const UIComponentClassAlias = "_ui_component_class";
  std::string const HTMLTemplateLinkAlias = "_html_template_link";

  ScTemplate HTMLTemplateTemplate;
  HTMLTemplateTemplate.Triple(
        ScType::NodeVarClass >> UIComponentClassAlias,
        ScType::EdgeAccessVarPosPerm,
        uiComponent);
  HTMLTemplateTemplate.Quintuple(
        HTMLTranslatorKeynodes::concept_user_interface_component,
        ScType::EdgeDCommonVar,
        UIComponentClassAlias,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_inclusion);
  HTMLTemplateTemplate.Quintuple(
        UIComponentClassAlias,
        ScType::EdgeDCommonVar,
        ScType::LinkVar >> HTMLTemplateLinkAlias,
        ScType::EdgeAccessVarPosPerm,
        HTMLTranslatorKeynodes::nrel_html_template);

  // Search only the first template result
  context.HelperSmartSearchTemplate(HTMLTemplateTemplate,
  [&HTMLTemplateLinkAlias, &HTMLTemplateLink](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest
  {
    item.Get(HTMLTemplateLinkAlias, HTMLTemplateLink);
    return ScTemplateSearchRequest::STOP;
  });

  return HTMLTemplateLink;
}

ScAddr HTMLTranslator::GetTemplateAgentAnswerLink(ScMemoryContext & context, ScAddr const & uiElement, ScAddr const & uiHTMLTemplateLink)
{
  ScAddr const replacementConstantsSet = context.CreateNode(ScType::NodeConst);
  GenerationUtils::generateRelationBetween(&context, replacementConstantsSet, uiElement, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddrVector const argumentsVector {uiHTMLTemplateLink, replacementConstantsSet, HTMLTranslatorKeynodes::format_html};
  ScAddr const templateAgentAnswer = AgentUtils::applyActionAndGetResultIfExists(
        &context, HTMLTranslatorKeynodes::action_evaluate_specified_string_template, argumentsVector, 300);
  return IteratorUtils::getAnyFromSet(&context, templateAgentAnswer);
}

} // namespace htmlTranslationModule
