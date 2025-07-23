#include "controller/inputcontroller.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include <string>

InputController::InputController() {

}

InputController::~InputController() {

}

void InputController::init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<KakouneClientProcess> kakoune_process) {
    m_kakoune_client = kakoune_client;
    m_kakoune_process = kakoune_process;
}

void InputController::onCharacterInsert(std::string character) {
    OutgoingRequest keys_request;
    keys_request.type = OutgoingRequestType::KEYS;
    keys_request.data = KeysRequestData{{character}};

    m_kakoune_process->sendRequest(keys_request);
}
