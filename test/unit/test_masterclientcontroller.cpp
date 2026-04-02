#include "application/model/focusedclientstack.hpp"
#include "catch2/catch_test_macros.hpp"
#include "application/controllers/masterclientcontroller.hpp"
#include "mock_kakounesession.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/panelayout.hpp"
#include "application/model/kakouneclient.hpp"

TEST_CASE("MasterClientController updates master clients correctly", "[MasterClientController]") {
    MockKakouneSession session;
    ClientManager client_manager(&session);
    FocusedClientStack focused_client_stack;
    PaneLayout pane_layout;

    MasterClientController controller;
    controller.init(&session, &pane_layout);

    SECTION("Initial state with no clients") {
        REQUIRE(session.commands_sent.empty());
    }

    SECTION("Adding clients updates master list") {
        auto client1 = client_manager.createClient(std::nullopt, {});
        client1->client_name = "editor1";

        pane_layout.init(&client_manager, &focused_client_stack);
        pane_layout.setBounds({0, 0, 100, 100});
        pane_layout.arrange();

        REQUIRE(session.commands_sent.size() == 1);
        REQUIRE(session.commands_sent[0] == "set-option global kakodemon_masters editor1");
    }

    SECTION("Multiple masters are handled correctly") {
        auto client1 = client_manager.createClient(std::nullopt, {});
        client1->client_name = "editor1";

        auto client2 = client_manager.createClient(std::nullopt, {});
        client2->client_name = "editor2";

        auto client3 = client_manager.createClient(std::nullopt, {});
        client3->client_name = "editor3";

        pane_layout.init(&client_manager, &focused_client_stack);
        pane_layout.setNumMasters(2);
        pane_layout.setBounds({0, 0, 100, 100});
        pane_layout.arrange();

        REQUIRE(session.commands_sent.size() == 1);
        REQUIRE(session.commands_sent[0] == "set-option global kakodemon_masters editor1 editor2");
    }

    SECTION("Changing number of masters updates the list") {
        auto client1 = client_manager.createClient(std::nullopt, {});
        client1->client_name = "editor1";

        auto client2 = client_manager.createClient(std::nullopt, {});
        client2->client_name = "editor2";

        pane_layout.init(&client_manager, &focused_client_stack);
        pane_layout.setNumMasters(1);
        pane_layout.setBounds({0, 0, 100, 100});
        pane_layout.arrange();

        session.commands_sent.clear();

        pane_layout.setNumMasters(2);
        pane_layout.arrange();

        REQUIRE(session.commands_sent.size() == 1);
        REQUIRE(session.commands_sent[0] == "set-option global kakodemon_masters editor1 editor2");
    }

    SECTION("Removing clients updates the list") {
        auto client1 = client_manager.createClient(std::nullopt, {});
        client1->client_name = "editor1";

        auto client2 = client_manager.createClient(std::nullopt, {});
        client2->client_name = "editor2";

        pane_layout.init(&client_manager, &focused_client_stack);
        pane_layout.setNumMasters(2);
        pane_layout.setBounds({0, 0, 100, 100});
        pane_layout.arrange();

        session.commands_sent.clear();

        client_manager.removeClient(client1);
        pane_layout.arrange();

        REQUIRE(session.commands_sent.size() == 1);
        REQUIRE(session.commands_sent[0] == "set-option global kakodemon_masters editor2");
    }

    SECTION("Renaming clients updates the list") {
        auto client1 = client_manager.createClient(std::nullopt, {});
        client1->client_name = "editor1";

        pane_layout.init(&client_manager, &focused_client_stack);
        pane_layout.setBounds({0, 0, 100, 100});
        pane_layout.arrange();

        session.commands_sent.clear();

        client_manager.renameClient(client1->client_id, "renamed_editor");
        pane_layout.arrange();

        REQUIRE(session.commands_sent.size() == 1);
        REQUIRE(session.commands_sent[0] == "set-option global kakodemon_masters renamed_editor");
    }

    SECTION("Caching prevents redundant commands when master list doesn't change") {
        auto client1 = client_manager.createClient(std::nullopt, {});
        client1->client_name = "editor1";

        pane_layout.init(&client_manager, &focused_client_stack);
        pane_layout.setBounds({0, 0, 100, 100});
        pane_layout.arrange();

        session.commands_sent.clear();

        pane_layout.arrange();

        REQUIRE(session.commands_sent.empty());
    }
}
