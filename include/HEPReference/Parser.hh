#pragma once

namespace HEPReference {
    class Parser {
        public:
            Parser(std::istream &input);
            virtual ~Parser();
    };
}
