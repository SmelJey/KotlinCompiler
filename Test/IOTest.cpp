#include "IOTest.h"

#include <fstream>
#include <iostream>

IOTest::~IOTest() = default;

IOTest::IOTest(const std::string& inputFilepath) : myFilepath(TestDirectory + inputFilepath) {
    std::cout << myFilepath;
}

void IOTest::Run() {
    std::ifstream goldStream(myFilepath + ".gold");
    std::ofstream outputStream(myFilepath + ".gold.last");

    REQUIRE(!goldStream.eof());

    std::string token1 = NextToken(), token2 = NextTokenFromGold(goldStream);
    while (!token1.empty() && !token2.empty()) {
        CHECK(token1 == token2);
        outputStream << "@@ " << token1 << std::endl;
        token1 = NextToken(), token2 = NextTokenFromGold(goldStream);
    }
    CHECK(token1 == token2);
    while (!token1.empty()) {
        outputStream << "@@ " << token1 << std::endl;
        token1 = NextToken();
    }

    outputStream.close();
}

std::string IOTest::NextTokenFromGold(std::ifstream& ifs) {
    std::string token = myLastToken;
    if (token.empty()) {
        std::getline(ifs, token);
    }
    myLastToken = "";

    while (std::getline(ifs, myLastToken)) {
        if (myLastToken.size() >= 3 && myLastToken.substr(0, 3) == "@@ ") {
            break;
        }

        token.append("\n");
        token.append(myLastToken);
    }

    if (token.empty()) {
        return token;
    }

    return token.substr(3);
}

std::string IOTest::GetFilepath() const {
    return myFilepath;
}
