/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

/*
Purpose: transform sc-model of the UI to a state that's translatable to HTML 
and launch the process of getting the full HTML component tree 
1. Traverse through the model: create relations from sc-nodes of components to their respective templates
    1.1 Use rules to check which templates should be used for a respective component class
    1.2 generate a relation
2. Generate the variables needed for the templates
    2.1 Check specs of the template: required variables, optional variables
    2.2 Generate variable map for the templates (lazy evaluation: <name, sc-link | sc-action | sc-template>)
        2.2.1 OPTION 1: Template spec will define sc-templates /sc-actions to get the value of the variables
            2.2.1.1 Run a search / run agent against each required variable, add it to the map, 
                    oportunistically complete with optional variables
        2.2.1 OPTION 2: (slow but extensible) We could check each parameter that the component has 
            2.2.1.1 does the parameter have an html translation rule? 
                    we should know how can it be integrated into the component template
            2.2.1.2 each parameter that has an html template gets inserted into a variable map
    2.3 We'll get a tree of templates that depend on each other (something like an execution plan for the templater)
        root template -> variable <name, (sc-template | sc-action) -> nested template -> variable <name, (...)>  >
3. Run template evaluation agent for the root of the tree
*/

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "html-translator/HTMLTranslator.hpp"

#include "HTMLTranslatorAgent.hpp"

using namespace utils;

namespace htmlTranslationModule
{
SC_AGENT_IMPLEMENTATION(HTMLTranslatorAgent)
{
  ScAddr const actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);

  if (!CheckActionClass(actionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_WARNING("HTMLTranslatorAgent started");

  // Get root user interface element from sc-model
  ScAddr const rootUiElement = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!rootUiElement.IsValid())
  {
    SC_LOG_ERROR("Action doesn't have a user interface component.");
    SC_LOG_DEBUG("HTMLTranslatorAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr answerHTMLLink = HTMLTranslator::TranslateScToHTML(m_memoryCtx, rootUiElement);

  // Create agent answer and finish agent work
  AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, {answerHTMLLink}, true);
  SC_LOG_DEBUG("HTMLTranslatorAgent finished");
  return SC_RESULT_OK;
}

bool HTMLTranslatorAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(HTMLTranslatorKeynodes::action_translate_ui_model_to_html, actionNode, ScType::EdgeAccessConstPosPerm);
}

} // namespace htmlTranslationModule
