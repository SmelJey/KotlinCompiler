#pragma once

#include <string>
#include <sstream>
#include <fstream>

template<typename InputType>
class InputBuffer {
public:
    constexpr static int EOF_VAL = std::numeric_limits<int>::max();
    constexpr static int DEFAULT_CAPACITY = 256;

    InputBuffer(const std::string& filepath, int bufferSize = DEFAULT_CAPACITY)
        : myBuffer(bufferSize, 0), myCurrentIdx(), myCurrentEnd(), myBufferSize(bufferSize), myInputStream(filepath) {}

    int NextChar() {
        if (!returnCharsBuffer.empty()) {
            char returnChar = *returnCharsBuffer.rbegin();
            returnCharsBuffer.pop_back();
            return returnChar;
        }

        if (myCurrentIdx >= myCurrentEnd) {
            myCurrentIdx = 0;
            myInputStream.read(&myBuffer[0], myBufferSize);
            myCurrentEnd = myInputStream.gcount();

            if (myCurrentEnd == 0) {
                return EOF_VAL;
            }
        }

        return myBuffer[myCurrentIdx++];
    }

    void ReturnChar(char character) {
        returnCharsBuffer.push_back(character);
    }

    // TODO: make this work?
    /*template<std::enable_if<std::is_same<InputType, std::stringstream>, InputType>>
    void AppendText(std::string& src) {
        myInputStream << src;
    }*/

private:
    std::string returnCharsBuffer;
    std::string myBuffer;
    std::streamsize myCurrentIdx;
    std::streamsize myCurrentEnd;
    int myBufferSize;

    InputType myInputStream;
};