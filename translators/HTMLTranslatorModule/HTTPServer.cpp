#include <httplib.h>
#include <HTTPServer.hpp>
#include <string>
#include <HTTPRequestHandler.hpp>


namespace htmlTranslationModule
{
void ServerWrapper::StartServer()
{
  // Instantiate the server
  m_isRunning = SC_TRUE;
  m_serverThread = std::thread(&ServerWrapper::Run, this);
}

void ServerWrapper::Run()
{
  // Define a handler for the root path
  m_server.Get(
        "/", [&](const httplib::Request &req, httplib::Response &res) -> void {
          HTTPRequestHandler::RetrieveCurrentUIHandler(req, res);
        });

  // an endpoint to resolve sc-files using ScAddr
  m_server.Get("/files/by_sc_addr/:addr",
             [&](const httplib::Request &req, httplib::Response &res) -> void {
               HTTPRequestHandler::FileByAddrRequestHandler(req, res);
             });
  // an endpoint to resolve sc-files using system identifier
  m_server.Get("/files/by_system_idtf/:system_idtf",
             [&](const httplib::Request &req, httplib::Response &res) -> void {
               HTTPRequestHandler::FileBySystemIdtfRequestHandler(req, res);
             });

  // Start the server
  SC_LOG_INFO("[ostis-ui] HTTP-server server is running on port 8080");
  m_server.listen("localhost", 8080);
}

void ServerWrapper::StopServer()
{
  m_server.stop();
  m_isRunning = SC_FALSE;
  if (m_serverThread.joinable())
    m_serverThread.join();

  SC_LOG_INFO("[ostis-ui] HTTP-server server is stopped");
}

} // namespace htmlTranslationModule
