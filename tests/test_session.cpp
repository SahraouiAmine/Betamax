// Tests for issue #22: a drill session times, scores and records attempts.
//
// Decision 3: a correct answer auto-advances as soon as it is typed; Enter on a
// wrong answer records it and the problem stays. Time is passed in explicitly,
// so no test sleeps.

#include "core/problem.hpp"
#include "core/rational.hpp"
#include "session/session.hpp"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

using namespace std::chrono_literals;

namespace {

using Clock = std::chrono::steady_clock;
const Clock::time_point t0{};

// Hands out problems in order and counts how many were requested.
struct FakeSource {
    std::vector<Problem> problems;
    std::size_t taken = 0;

    Problem operator()() { return problems.at(taken++); }
};

Problem problem(std::string prompt, std::int64_t answer) {
    return Problem{.prompt = std::move(prompt), .answer = Rational(answer)};
}

FakeSource three_problems() {
    return {.problems = {problem("12 + 7", 19), problem("6 × 7", 42), problem("9 − 4", 5)}};
}

} // namespace

TEST_CASE("A session starts on the first problem with a score of zero", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);

    REQUIRE(session.current().prompt == "12 + 7");
    REQUIRE(session.score() == 0);
    REQUIRE(session.attempts().empty());
}

TEST_CASE("Typing the correct answer scores and advances", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0);

    REQUIRE(session.try_answer("19", t0 + 2s));
    REQUIRE(session.score() == 1);
    REQUIRE(session.current().prompt == "6 × 7");
}

TEST_CASE("A partial or wrong answer while typing records nothing", "[session]") {
    // "1" is on the way to "19": the player is still typing.
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0);

    REQUIRE_FALSE(session.try_answer("1", t0 + 1s));
    REQUIRE_FALSE(session.try_answer("abc", t0 + 1s));
    REQUIRE(session.score() == 0);
    REQUIRE(session.attempts().empty());
    REQUIRE(session.current().prompt == "12 + 7");
}

TEST_CASE("Submitting a wrong answer records it and the problem stays", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0);

    session.submit("18", t0 + 3s);

    REQUIRE(session.score() == 0);
    REQUIRE(session.current().prompt == "12 + 7");
    REQUIRE(session.attempts().size() == 1);
    const Attempt& a = session.attempts().front();
    REQUIRE(a.prompt == "12 + 7");
    REQUIRE(a.user_answer == "18");
    REQUIRE(a.outcome == Outcome::Wrong);
}

TEST_CASE("Submitting the correct answer counts like typing it", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0);

    session.submit("19", t0 + 2s);

    REQUIRE(session.score() == 1);
    REQUIRE(session.current().prompt == "6 × 7");
    REQUIRE(session.attempts().back().outcome == Outcome::Correct);
}

TEST_CASE("A correct answer is recorded with its response time", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0 + 1s);

    REQUIRE(session.try_answer("19", t0 + 2500ms));

    REQUIRE(session.attempts().size() == 1);
    const Attempt& a = session.attempts().front();
    REQUIRE(a.prompt == "12 + 7");
    REQUIRE(a.correct_answer == Rational(19));
    REQUIRE(a.user_answer == "19");
    REQUIRE(a.outcome == Outcome::Correct);
    REQUIRE(a.response_ms == 1500);
}

TEST_CASE("Response time runs from when the problem was first shown", "[session]") {
    // A redraw after a wrong answer must not reset the clock.
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0);

    session.submit("18", t0 + 1s);
    session.mark_shown(t0 + 1100ms);
    REQUIRE(session.try_answer("19", t0 + 3s));

    REQUIRE(session.attempts().at(0).response_ms == 1000);
    REQUIRE(session.attempts().at(1).response_ms == 3000);
}

TEST_CASE("Each new problem gets its own shown time", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    session.mark_shown(t0);
    REQUIRE(session.try_answer("19", t0 + 2s));

    session.mark_shown(t0 + 2s);
    REQUIRE(session.try_answer("42", t0 + 5s));

    REQUIRE(session.attempts().at(1).response_ms == 3000);
}

TEST_CASE("Problems are requested one at a time, only when needed", "[session]") {
    // Stage 2 seam: a background producer will fill the source ahead of time.
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);
    REQUIRE(source.taken == 1);

    session.mark_shown(t0);
    session.submit("18", t0 + 1s);
    REQUIRE(source.taken == 1);

    REQUIRE(session.try_answer("19", t0 + 2s));
    REQUIRE(source.taken == 2);
}

TEST_CASE("The session ends when the duration elapses", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 120s, t0);

    REQUIRE_FALSE(session.is_over(t0 + 119'999ms));
    REQUIRE(session.is_over(t0 + 120s));
}

TEST_CASE("Answers after the end are not counted", "[session]") {
    FakeSource source = three_problems();
    Session session(std::ref(source), 60s, t0);
    session.mark_shown(t0);

    REQUIRE_FALSE(session.try_answer("19", t0 + 61s));
    session.submit("18", t0 + 61s);

    REQUIRE(session.score() == 0);
    REQUIRE(session.attempts().empty());
}
