#include "godnet/net/message_buffer.hpp"

#include <cstring>
#include <cassert>

#include "godnet/util/endian.hpp"

namespace godnet
{

MessageBuffer::MessageBuffer(std::size_t length) noexcept
: start_(new char[length]{}),
  read_(start_),
  write_(start_),
  end_(start_ + length)
{
}

MessageBuffer::~MessageBuffer()
{
    delete[] start_;
}

std::uint8_t MessageBuffer::peekInt8() const noexcept
{
    assert(readByte() >= sizeof(std::uint8_t));
    return *reinterpret_cast<const std::uint8_t*>(readPeek());
}

std::uint16_t MessageBuffer::peekInt16() const noexcept
{
    assert(readByte() >= sizeof(std::uint16_t));
    return networkToHost(*reinterpret_cast<const std::uint16_t*>(readPeek()));
}

std::uint32_t MessageBuffer::peekInt32() const noexcept
{
    assert(readByte() >= sizeof(std::uint32_t));
    return networkToHost(*reinterpret_cast<const std::uint32_t*>(readPeek()));
}

std::uint64_t MessageBuffer::peekInt64() const noexcept
{
    assert(readByte() >= sizeof(std::uint64_t));
    return networkToHost(*reinterpret_cast<const std::uint64_t*>(readPeek()));
}

std::uint8_t MessageBuffer::readInt8() noexcept
{
    std::uint8_t val = peekInt8();
    retrieve(sizeof(val));
    return val;
}

std::uint16_t MessageBuffer::readInt16() noexcept
{
    std::uint16_t val = peekInt16();
    retrieve(sizeof(val));
    return val;
}

std::uint32_t MessageBuffer::readInt32() noexcept
{
    std::uint32_t val = peekInt32();
    retrieve(sizeof(val));
    return val;
}

std::uint64_t MessageBuffer::readInt64() noexcept
{
    std::uint64_t val = peekInt64();
    retrieve(sizeof(val));
    return val;
}

void MessageBuffer::writeInt8(std::uint8_t val) noexcept
{
    write(reinterpret_cast<const char*>(&val), sizeof(val));
}

void MessageBuffer::writeInt16(std::uint16_t val) noexcept
{
    val = hostToNetwork(val);
    write(reinterpret_cast<const char*>(&val), sizeof(val));
}

void MessageBuffer::writeInt32(std::uint32_t val) noexcept
{
    val = hostToNetwork(val);
    write(reinterpret_cast<const char*>(&val), sizeof(val));
}

void MessageBuffer::writeInt64(std::uint64_t val) noexcept
{
    val = hostToNetwork(val);
    write(reinterpret_cast<const char*>(&val), sizeof(val));
}

void MessageBuffer::write(const char* buf, std::size_t len) noexcept
{
    if (writeByte() < len)
    {
        reallocate(len);
    }
    std::memcpy(write_, buf, len);
    write_ += len;
}

void MessageBuffer::write(std::string_view buf) noexcept
{
    write(buf.data(), buf.size());
}

std::string MessageBuffer::read(std::size_t len) noexcept
{
    assert(len <= readByte());
    std::string ret(readPeek(), len);
    retrieve(len);
    return ret;
}

std::string MessageBuffer::readUntil(const char* end) noexcept
{
    assert(readPeek() <= end);
    assert(end <= writePeek());
    return read(end - readPeek());
}

std::string MessageBuffer::readAll() noexcept
{
    return read(readByte());
}

void MessageBuffer::retrieve(std::size_t len) noexcept
{
    assert(len <= readByte());
    if (len < readByte())
    {
        read_ += len;
    }
    else
    {
        retrieveAll();
    }
}

void MessageBuffer::retrieveUntil(const char* end) noexcept
{
    assert(readPeek() <= end);
    assert(end <= writePeek());
    retrieve(end - readPeek());
}

void MessageBuffer::retrieveAll() noexcept
{
    read_ = start_;
    write_ = start_;
}

void MessageBuffer::reallocate(std::size_t length) noexcept
{
    const std::size_t readSize = readByte();

    if (size() - readByte() < length)
    {
        size_t newSize = (size() + length) * 2;
        char* newData = new char[newSize];

        std::memcpy(newData, read_, readSize);
        delete[] start_;

        start_ = newData;
        read_ = newData;
        write_ = read_ + readSize;
        end_ = start_ + newSize;
    }
    else
    {
        std::memmove(start_, read_, readSize);

        read_ = start_;
        write_ = read_ + readSize;
    }
}

}