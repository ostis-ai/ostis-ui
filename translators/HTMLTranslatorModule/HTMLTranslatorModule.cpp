/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "agents/HTMLTranslatorAgent.hpp"

#include "HTMLTranslatorModule.hpp"

using namespace htmlTranslationModule;

SC_IMPLEMENT_MODULE(HTMLTranslatorModule)

sc_result HTMLTranslatorModule::InitializeImpl()
{
  if (!HTMLTranslatorKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(HTMLTranslatorAgent);
  m_server = std::make_unique<ServerWrapper>();
  m_server->StartServer();

  return SC_RESULT_OK;
}

sc_result HTMLTranslatorModule::ShutdownImpl()
{
  if (m_server)
    m_server->StopServer();
  m_server.reset();
  SC_AGENT_UNREGISTER(HTMLTranslatorAgent);

  return SC_RESULT_OK;
}
