#pragma once

#include <cstdlib>
#include <iostream>

template <typename std::size_t tp_buffer_size>
class ostream_peripheral
    : public std::ostream,
      protected std::streambuf {
  public:
    inline constexpr static std::size_t buffer_size = tp_buffer_size;

  private:
    char buffer[buffer_size];

  protected:
    inline virtual std::streambuf::int_type overflow(std::streambuf::int_type ch) override final {
        if (ch != std::streambuf::traits_type::eof()) {
            sync();
            *pptr() = ch;
            pbump(1);
        }
        return ch;
    }

    inline virtual int sync(void) override final {
        std::streamsize count = pptr() - pbase();
        int ret_val           = write_buffer(pbase(), count);
        setp(buffer, buffer + buffer_size - 1);
        return ret_val;
    }

  public:
    ostream_peripheral()
        : std::ostream(static_cast<std::streambuf *>(this)) {
        setp(buffer, buffer + buffer_size - 1);
    }

  protected:
    virtual int write_buffer(const char *buffer, std::size_t length) = 0;
};

template <typename std::size_t tp_buffer_size>
class istream_peripheral
    : public std::istream,
      protected std::streambuf {

  public:
    inline constexpr static std::size_t buffer_size = tp_buffer_size;

  private:
    char buffer[buffer_size];

  protected:
    virtual std::streambuf::int_type underflow() override {
        std::streamsize count = read_buffer(buffer, buffer_size);
        if (count <= 0) {
            return std::streambuf::traits_type::eof();
        }

        setg(buffer, buffer, buffer + count);
        return std::streambuf::traits_type::to_int_type(*gptr());
    }

    virtual std::streambuf::int_type uflow() override {
        std::streambuf::int_type ch = underflow(); 
        if (ch == std::streambuf::traits_type::eof()) {
            return std::streambuf::traits_type::eof();
        }
        gbump(1);
        return ch;
    }

    virtual std::streambuf::int_type pbackfail(std::streambuf::int_type ch) override {
        if (gptr() > eback()) {
            gbump(-1);
            if (ch != std::streambuf::traits_type::eof()) {
                *gptr() = ch;
            }
            return std::streambuf::traits_type::to_int_type(*gptr());
        }
        return std::streambuf::traits_type::eof();
    }

  public:
    istream_peripheral()
        : std::istream(static_cast<std::streambuf *>(this)) {
            setg(buffer, buffer, buffer); 
    }

  protected:
    /**
     * @brief read data from peripheral
     *
     * @param buffer
     * @param buffer_size
     * @return std::streamsize
     */
    virtual std::streamsize read_buffer(const char *buffer, std::size_t buffer_size) = 0;
};