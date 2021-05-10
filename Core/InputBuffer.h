#pragma once

#include <string>

constexpr int BUFFER_EOF = std::numeric_limits<int>::max();

template<typename InputType>
class InputBuffer {
public:
    constexpr static int DEFAULT_CAPACITY = 4093;
    constexpr static int DEFAULT_LOOKAHEAD_BUFFER_SIZE = 3;

    InputBuffer(const std::string& filepath, std::size_t lookAheadBufferSize = DEFAULT_LOOKAHEAD_BUFFER_SIZE, std::size_t bufferSize = DEFAULT_CAPACITY)
        : myBuffer(bufferSize + lookAheadBufferSize, 0), myCurrentIdx(), myCurrentEnd(), myBufferSize(bufferSize + lookAheadBufferSize),
        myLookAheadBufferSize(lookAheadBufferSize), myInputStream(filepath) {
        ReadFromStream();
    }

    InputBuffer(std::streambuf* streamBuf, std::size_t lookAheadBufferSize = DEFAULT_LOOKAHEAD_BUFFER_SIZE, std::size_t bufferSize = DEFAULT_CAPACITY)
        : myBuffer(bufferSize + lookAheadBufferSize, 0), myCurrentIdx(), myCurrentEnd(), myBufferSize(bufferSize + lookAheadBufferSize),
        myLookAheadBufferSize(lookAheadBufferSize), myInputStream() {
        myInputStream.std::istream::rdbuf(streamBuf);
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

private:
    bool ReadFromStream() {
        size_t leftCharCount = myCurrentEnd - myCurrentIdx;
        if (leftCharCount == myBufferSize || myInputStream.eof()) {
            return false;
        }

        for (size_t i = 0; i < leftCharCount; i++) {
            myBuffer[i] = myBuffer[i + myCurrentIdx + 1];
        }

        myInputStream.read(&myBuffer[leftCharCount], myBufferSize - leftCharCount);
        myCurrentIdx = 0;
        myCurrentEnd = static_cast<size_t>(myInputStream.gcount()) + leftCharCount;
        return myInputStream.gcount() != 0;
    }

    std::string returnCharsBuffer;
    std::string myBuffer;

    size_t myCurrentIdx;
    size_t myCurrentEnd;
    size_t myBufferSize;
    size_t myLookAheadBufferSize;

    InputType myInputStream;
};
