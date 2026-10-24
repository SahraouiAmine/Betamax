// start the HTTP server and hand requests off to the api module.

#include <CLI/CLI.hpp>

#include <exception>
#include <httplib.h>
#include <iostream>
#include <string>

namespace {

int run(int argc, char** argv) {
    CLI::App app{"Mental-math trainer: HTTP server", BETAMAX_APP_NAME};
    app.set_version_flag("--version", std::string(BETAMAX_APP_NAME) + " " + BETAMAX_VERSION);

    // 0.0.0.0, not localhost: inside a container, localhost is unreachable
    // from outside. Fly (and most hosts) pass the port in $PORT.
    std::string host = "0.0.0.0";
    int port = 8080;
    app.add_option("--host", host, "Address to listen on")->capture_default_str();
    app.add_option("--port", port, "Port to listen on")
        ->envname("PORT")
        ->check(CLI::Range(1, 65535))
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    httplib::Server server;
    // Routes from src/api/ get registered here once they exist.

    std::cerr << BETAMAX_APP_NAME << ": listening on " << host << ':' << port << '\n';
    if (!server.listen(host, port)) {
        std::cerr << BETAMAX_APP_NAME << ": could not listen on " << host << ':' << port << '\n';
        return 1;
    }
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    try {
        return run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << BETAMAX_APP_NAME << ": " << e.what() << '\n';
        return 1;
    }
}
