#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_type.hpp"
#include <httplib.h>
#include <string>

namespace htmlTranslationModule {

class ServerWrapper
{
public:
  ServerWrapper() = default;

  ~ServerWrapper() = default;

  void Run();

  void StartServer();

  void StopServer();

protected:
    std::thread m_serverThread;
    httplib::Server m_server;
    std::atomic<sc_bool> m_isRunning;
 };
}
