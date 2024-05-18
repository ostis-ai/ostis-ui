#include "sc-memory/sc_memory.hpp"
#include <httplib.h>

namespace htmlTranslationModule {

class HTTPRequestHandler
{
public:
  static ScAddr ConvertToScAddr(std::string const & str);
  static std::string GetFileMimetype(ScMemoryContext * context, ScAddr const & scFile);
  static void RetrieveCurrentUIHandler(httplib::Request const & req, httplib::Response & res);
  static void FileBySystemIdtfRequestHandler(httplib::Request const & req, httplib::Response & res) noexcept;
  static void FileByAddrRequestHandler(const httplib::Request& req, httplib::Response& res) noexcept;
  static void FileRetriever(ScMemoryContext * context, ScAddr & fileAddr, httplib::Request const & req, httplib::Response & res) noexcept;
};

}
