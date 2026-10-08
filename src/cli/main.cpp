// parse arguments and hand off to session / store / tui.

#include <CLI/CLI.hpp>

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  CLI::App app{"Mental-math trainer", BETAMAX_APP_NAME};
  app.set_version_flag("--version", std::string(BETAMAX_APP_NAME) + " " + BETAMAX_VERSION);

  std::vector<std::string> focus;
  int duration_s = 120;
  std::optional<std::uint64_t> seed;

  app.add_option("--focus", focus, "Restrict to these problem types (comma-separated)")
      ->delimiter(',');
  app.add_option("--duration", duration_s, "Session length in seconds")
      ->check(CLI::Range(1, 3600))
      ->capture_default_str();
  app.add_option("--seed", seed, "Seed for a reproducible session");

  CLI::App* stats = app.add_subcommand("stats", "Show history and stats");

  CLI11_PARSE(app, argc, argv);

  if (*stats) {
    std::cerr << "stats: not implemented yet (M4)\n";
    return 1;
  }

  std::cerr << "drill session: not implemented yet (M3/M5)\n";
  return 1;
}
