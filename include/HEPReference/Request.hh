#ifndef HEPREFERENCE_REQUEST
#define HEPREFERENCE_REQUEST

#include <cpr/cpr.h>
#include <string>
#include "spdlog/spdlog.h"

namespace HEPReference {

class Request {
    protected:
        cpr::Response Get(const std::string &url) {
            spdlog::trace("Sending get request: {}", url);
            m_session.SetOption(cpr::Url(url));
            return m_session.Get();
        }

        template<typename T>
        void SetOption(const T &opt) {
            m_session.SetOption(opt);
        }

    private:
        cpr::Session m_session;
};

}

#endif
