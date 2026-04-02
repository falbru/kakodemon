#include <catch2/catch_test_macros.hpp>
#include "application/model/focusedclientstack.hpp"

static KakouneClient* const A = reinterpret_cast<KakouneClient*>(0x1);
static KakouneClient* const B = reinterpret_cast<KakouneClient*>(0x2);
static KakouneClient* const C = reinterpret_cast<KakouneClient*>(0x3);

TEST_CASE("FocusedClientStack - focused() returns nullptr when empty", "[focusedclientstack]")
{
    FocusedClientStack stack;
    REQUIRE(stack.focused() == nullptr);
}

TEST_CASE("FocusedClientStack - focus() sets focused client", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    REQUIRE(stack.focused() == A);
}

TEST_CASE("FocusedClientStack - focus() replaces focused client", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.focus(B);
    REQUIRE(stack.focused() == B);
}

TEST_CASE("FocusedClientStack - no duplicates in stack", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.focus(B);
    stack.focus(A);
    stack.focus(A);
    REQUIRE(stack.focused() == A);

    stack.remove(A);
    REQUIRE(stack.focused() == B);
}

TEST_CASE("FocusedClientStack - remove() focused client falls back to previous", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.focus(B);
    stack.remove(B);
    REQUIRE(stack.focused() == A);
}

TEST_CASE("FocusedClientStack - remove() non-focused client does not change focus", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.focus(B);
    stack.remove(A);
    REQUIRE(stack.focused() == B);
}

TEST_CASE("FocusedClientStack - remove() last client results in nullptr", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.remove(A);
    REQUIRE(stack.focused() == nullptr);
}

TEST_CASE("FocusedClientStack - remove() non-existent client does nothing", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.remove(B);
    REQUIRE(stack.focused() == A);
}

TEST_CASE("FocusedClientStack - stack order preserved across multiple focuses", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.focus(B);
    stack.focus(C);

    stack.remove(C);
    REQUIRE(stack.focused() == B);

    stack.remove(B);
    REQUIRE(stack.focused() == A);

    stack.remove(A);
    REQUIRE(stack.focused() == nullptr);
}

TEST_CASE("FocusedClientStack - onFocusChanged fires when focus changes", "[focusedclientstack]")
{
    FocusedClientStack stack;
    KakouneClient* notified_previous = reinterpret_cast<KakouneClient*>(0xFF);
    KakouneClient* notified_next = nullptr;
    stack.onFocusChanged([&](KakouneClient* previous, KakouneClient* next) {
        notified_previous = previous;
        notified_next = next;
    });

    stack.focus(A);
    REQUIRE(notified_previous == nullptr);
    REQUIRE(notified_next == A);

    stack.focus(B);
    REQUIRE(notified_previous == A);
    REQUIRE(notified_next == B);
}

TEST_CASE("FocusedClientStack - onFocusChanged fires with nullptr when last client removed", "[focusedclientstack]")
{
    FocusedClientStack stack;
    KakouneClient* notified_previous = nullptr;
    KakouneClient* notified_next = reinterpret_cast<KakouneClient*>(0xFF);
    stack.onFocusChanged([&](KakouneClient* previous, KakouneClient* next) {
        notified_previous = previous;
        notified_next = next;
    });

    stack.focus(A);
    stack.remove(A);
    REQUIRE(notified_previous == A);
    REQUIRE(notified_next == nullptr);
}

TEST_CASE("FocusedClientStack - onFocusChanged does not fire when non-focused client removed", "[focusedclientstack]")
{
    FocusedClientStack stack;
    stack.focus(A);
    stack.focus(B);

    int call_count = 0;
    stack.onFocusChanged([&](KakouneClient*, KakouneClient*) { call_count++; });

    stack.remove(A);
    REQUIRE(call_count == 0);
    REQUIRE(stack.focused() == B);
}

TEST_CASE("FocusedClientStack - removeObserver stops notifications", "[focusedclientstack]")
{
    FocusedClientStack stack;
    int call_count = 0;
    domain::ObserverId id = stack.onFocusChanged([&](KakouneClient*, KakouneClient*) { call_count++; });

    stack.focus(A);
    REQUIRE(call_count == 1);

    stack.removeObserver(id);
    stack.focus(B);
    REQUIRE(call_count == 1);
}
