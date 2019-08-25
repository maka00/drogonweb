#include <iostream>
#include<clara.hpp>
#include<spdlog/spdlog.h>
#include<spdlog/sinks/stdout_color_sinks.h>
#include <drogon/HttpController.h>
#include <drogon/WebSocketController.h>
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
                // http http://127.0.0.1:8088/demo/v1/User/get/1
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
class EchoWebsock : public drogon::WebSocketController<EchoWebsock> {
public:
    WS_PATH_LIST_BEGIN
        // list path definations here;
        WS_PATH_ADD("/echo");
    WS_PATH_LIST_END

    void handleNewMessage(const WebSocketConnectionPtr &ptr, std::string &&msg,
                          const WebSocketMessageType &type) override {
        LOG_DEBUG << "new websocket message" << msg;

    }

    void handleNewConnection(const HttpRequestPtr &ptr, const WebSocketConnectionPtr &connectionPtr) override {
        LOG_DEBUG << "new connection ";
        LOG_DEBUG << connectionPtr->peerAddr().toIp();
        connectionPtr->send("Hello");
    }

    void handleConnectionClosed(const WebSocketConnectionPtr &ptr) override {
        LOG_DEBUG << "connection closed";
    }
};

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
    //drogon::app().addListener("0.0.0.0",port);
    drogon::app().loadConfigFile("../etc/config.json");
    drogon::app().run();
	spdlog::get("main")->info("end");

}