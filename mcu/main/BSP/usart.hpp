#include "hal_header.hpp"
#include "stream_peripheral.hpp"

template <std::size_t tp_i_buffer_size, std::size_t tp_o_buffer_size>
class USART : public istream_peripheral<tp_i_buffer_size>,
              public ostream_peripheral<tp_o_buffer_size> {

  public:
    inline static constexpr std::size_t
        input_buffer_size  = tp_i_buffer_size,
        output_buffer_size = tp_o_buffer_size;
    char *input_buffer_ptr;

    using CallbackFunction = void(USART *);

  private:
    char
        input_buffer[input_buffer_size],
        output_buffer[output_buffer_size];

    USART_HandleTypeDef &handle;

  protected:
    inline virtual int write_buffer(const char *buffer, std::size_t length) override final {
        std::copy_n(buffer, length, output_buffer);
        while (HAL_USART_GetState(&handle) != HAL_USART_StateTypeDef::HAL_USART_STATE_READY)
            ;
        HAL_USART_Transmit(&handle, static_cast<std::uint8_t *>(output_buffer), length);
        return 0;
    }

    inline virtual std::streamsize read_buffer(const char *buffer, std::size_t buffer_size) override final {
        std::streamsize count = input_buffer_ptr - input_buffer;
        std::copy(input_buffer, input_buffer_ptr - 1, output_buffer);
        return count;
    }

  public:
    USART(USART_HandleTypeDef &handle)
        : handle(handle),
          input_buffer_ptr(input_buffer) {}

    inline void receive_data_cb(char data) {
    }

    inline void on_buffer_half_filled(CallbackFunction *cb) {
    }

    inline void on_buffer_filled(CallbackFunction *cb) {
    }

    void
};
