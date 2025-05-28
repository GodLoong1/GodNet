#ifndef GODNET_NET_MESSAGE_BUFFER_HPP
#define GODNET_NET_MESSAGE_BUFFER_HPP

#include <cstdint>
#include <string>
#include <string_view>

#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class MessageBuffer : Noncopyable
{
public:
    explicit MessageBuffer(std::size_t length = 1024) noexcept;
    ~MessageBuffer();

    std::size_t headByte() const noexcept
    {
        return read_ - start_;
    }

    std::size_t readByte() const noexcept
    {
        return write_ - read_;
    }

    std::size_t writeByte() const noexcept
    {
        return end_ - write_;
    }

    const char* readPeek() const noexcept
    {
        return read_;
    }

    const char* writePeek() const noexcept
    {
        return write_;
    }

    char* mutWritePeek() noexcept
    {
        return write_;
    }

    bool empty() const noexcept
    {
        return read_ == write_;
    }

    const char* data() const noexcept
    {
        return readPeek();
    }

    std::size_t size() const noexcept
    {
        return readByte();
    }

    std::uint8_t peekInt8() const noexcept;
    std::uint16_t peekInt16() const noexcept;
    std::uint32_t peekInt32() const noexcept;
    std::uint64_t peekInt64() const noexcept;

    std::uint8_t readInt8() noexcept;
    std::uint16_t readInt16() noexcept;
    std::uint32_t readInt32() noexcept;
    std::uint64_t readInt64() noexcept;

    void writeInt8(std::uint8_t val) noexcept;
    void writeInt16(std::uint16_t val) noexcept;
    void writeInt32(std::uint32_t val) noexcept;
    void writeInt64(std::uint64_t val) noexcept;

    void write(const char* buf, std::size_t len) noexcept;
    void write(std::string_view buf) noexcept;

    std::string read(std::size_t len) noexcept;
    std::string readUntil(const char* end) noexcept;
    std::string readAll() noexcept;

    void retrieve(std::size_t len) noexcept;
    void retrieveUntil(const char* end) noexcept;
    void retrieveAll() noexcept;

    void advanceWrite(std::size_t len) noexcept;

private:
    void reallocate(std::size_t length) noexcept;

    char* start_;
    char* read_;
    char* write_;
    char* end_;
};

}

#endif
