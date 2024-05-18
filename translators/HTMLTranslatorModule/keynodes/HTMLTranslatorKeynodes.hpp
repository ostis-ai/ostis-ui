/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "generated/HTMLTranslatorKeynodes.generated.hpp"

namespace htmlTranslationModule
{
class HTMLTranslatorKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_translate_sc_node_to_html"), ForceCreate)
  static ScAddr action_translate_sc_node_to_html;

  SC_PROPERTY(Keynode("nrel_decomposition"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_decomposition;

  SC_PROPERTY(Keynode("concept_user_interface_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_user_interface_component;

  SC_PROPERTY(Keynode("nrel_html_template"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_html_template;

  SC_PROPERTY(Keynode("nrel_html_representation"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_html_representation;

  SC_PROPERTY(Keynode("nrel_mimetype"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_mimetype;

  SC_PROPERTY(Keynode("action_evaluate_specified_string_template"), ForceCreate)
  static ScAddr action_evaluate_specified_string_template;

  SC_PROPERTY(Keynode("format_html"), ForceCreate(ScType::NodeConstClass))
  static ScAddr format_html;

  SC_PROPERTY(Keynode("servable_content"), ForceCreate(ScType::NodeConstClass))
  static ScAddr servable_content;

  SC_PROPERTY(Keynode("nrel_format"), ForceCreate(ScType::NodeConstClass))
  static ScAddr nrel_format;

  SC_PROPERTY(Keynode("ostis_ui_current_ui_model"), ForceCreate(ScType::NodeConst))
  static ScAddr ostis_ui_current_ui_model;
};

}  // namespace htmlTranslationModule
