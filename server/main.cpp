#include <iostream>
#include<clara.hpp>
#include<spdlog/spdlog.h>
#include<spdlog/sinks/stdout_color_sinks.h>
#include <drogon/HttpController.h>
#include <drogon/HttpAppFramework.h>
auto console = spdlog::stdout_color_mt("main");
using namespace drogon;
namespace demo {
    namespace v1 {
        class User : public drogon::HttpController<User> {
        public:
            METHOD_LIST_BEGIN
                //list path definitions here;
                //PATH_ADD("/path","filter1","filter2",HttpMethod1,HttpMethod2...);
                METHOD_ADD(User::getter, "get/{1}", Get);
            METHOD_LIST_END

            void getter(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int p1) const {
                //write your application logic here
                LOG_DEBUG << "got request with id=" << p1;
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k200OK);
                resp->setContentTypeCode(CT_TEXT_HTML);
                resp->setBody(fmt::format("Hello World!\nThis is a sample => {}",std::to_string(p1)));
                callback(resp);
            }
        };
    }
}
int main(int argc, char* argv[]) {
	int16_t port = 8088;
    bool help = false;
	auto cli = clara::Help(help)
	        | clara::Opt(port, "port")
	        ["-port"]["--port"]
            ("port to serve at");
	auto result = cli.parse(clara::Args(argc, argv));
    if (!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::exit(1);
    }
    if(help) {
        std::cerr << cli <<std::endl;
        std::exit(0);
    }
    spdlog::get("main")->info("begin");
    spdlog::get("main")->info(fmt::format("running at port={}",port));
    // drogon::app().addListener("0.0.0.0",port);
    drogon::app().loadConfigFile("../etc/config.json");
    drogon::app().run();
	spdlog::get("main")->info("end");

}