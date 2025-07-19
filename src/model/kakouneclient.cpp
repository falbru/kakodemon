#include "kakouneclient.hpp"
#include <memory>

KakouneClient::KakouneClient(const std::string& session_name) {
    m_process = std::make_unique<KakouneClientProcess>(session_name);
}

KakouneClient::~KakouneClient() {

}

void KakouneClient::init() {
    m_process->start();
}

void KakouneClient::pollForRequests() {
    m_process->pollForRequests();
}

std::optional<IncomingRequest> KakouneClient::getNextRequest() {
    return m_process->getNextRequest();

}

void KakouneClient::setWindowContent(const std::vector<Line>& line) {
    m_window_content = line;
}

const std::vector<Line>& KakouneClient::getWindowContent() const {
    return m_window_content;
}

void KakouneClient::sendRequest(const OutgoingRequest& request) {
    m_process->sendRequest(request);
}
