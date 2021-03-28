#pragma once

#include <string>

constexpr int BUFFER_EOF = std::numeric_limits<int>::max();

template<typename InputType>
class InputBuffer {
public:
    constexpr static int DEFAULT_CAPACITY = 4094;
    constexpr static int DEFAULT_LOOKAHEAD_BUFFER_SIZE = 2;

    InputBuffer(const std::string& filepath, std::size_t lookAheadBufferSize = DEFAULT_LOOKAHEAD_BUFFER_SIZE, std::size_t bufferSize = DEFAULT_CAPACITY)
        : myBuffer(bufferSize + lookAheadBufferSize, 0), myCurrentIdx(), myCurrentEnd(), myBufferSize(bufferSize + lookAheadBufferSize),
        myLookAheadBufferSize(lookAheadBufferSize), myInputStream(filepath) {
        ReadFromStream();
    }

    InputBuffer(std::streambuf* streamBuf, std::size_t lookAheadBufferSize = DEFAULT_LOOKAHEAD_BUFFER_SIZE, std::size_t bufferSize = DEFAULT_CAPACITY)
        : myBuffer(bufferSize + lookAheadBufferSize, 0), myCurrentIdx(), myCurrentEnd(), myBufferSize(bufferSize + lookAheadBufferSize),
        myLookAheadBufferSize(lookAheadBufferSize), myInputStream() {
        myInputStream.rdbuf(streamBuf);
        ReadFromStream();
    }

    int NextChar() {
        int curChar = GetChar();
        if (!returnCharsBuffer.empty()) {
            returnCharsBuffer.pop_back();
            return curChar;
        }

        myCurrentIdx++;
        ReadFromStream();
        return curChar;
    }

    int LookAhead(std::size_t offset) {
        if (offset > myLookAheadBufferSize) {
            return BUFFER_EOF;
        }

        if (returnCharsBuffer.size() > offset) {
            return returnCharsBuffer[returnCharsBuffer.size() - 1 - offset];
        }

        offset -= returnCharsBuffer.size();

        if (myCurrentIdx + offset > myCurrentEnd) {
            return BUFFER_EOF;
        }

        return myBuffer[myCurrentIdx + offset];
    }

    int GetChar() {
        if (!returnCharsBuffer.empty()) {
            return *returnCharsBuffer.rbegin();
        }

        if (myCurrentIdx >= myCurrentEnd) {
            return BUFFER_EOF;
        }
        return myBuffer[myCurrentIdx];
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
    bool ReadFromStream() {
        int leftCharCount = myCurrentEnd - myCurrentIdx;
        if (leftCharCount == myBufferSize || myInputStream.eof()) {
            return false;
        }

        for (int i = 0; i < leftCharCount; i++) {
            myBuffer[i] = myBuffer[i + myCurrentIdx + 1];
        }

        myInputStream.read(&myBuffer[leftCharCount], myBufferSize - leftCharCount);
        myCurrentIdx = 0;
        myCurrentEnd = myInputStream.gcount() + leftCharCount;
        return myInputStream.gcount() != 0;
    }

    std::string returnCharsBuffer;
    std::string myBuffer;
    std::streamsize myCurrentIdx;
    std::streamsize myCurrentEnd;
    std::size_t myBufferSize;
    std::size_t myLookAheadBufferSize;

    InputType myInputStream;
};
