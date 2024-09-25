/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"

sl_status_t sensirion_i2c_init(mikroe_i2c_handle_t i2c_handle)
{
  return sensirion_i2c_hal_init(i2c_handle);
}

sl_status_t sensirion_i2c_generate_crc(const uint8_t *data,
                                       uint16_t count,
                                       uint8_t *crc_val)
{
  if ((NULL != data) && (NULL != crc_val)) {
    uint16_t current_byte;
    uint8_t crc = CRC8_INIT;
    uint8_t crc_bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte) {
      crc ^= (data[current_byte]);
      for (crc_bit = 8; crc_bit > 0; --crc_bit) {
        if (crc & 0x80) {
          crc = (crc << 1) ^ CRC8_POLYNOMIAL;
        } else {
          crc = (crc << 1);
        }
      }
    }
    *crc_val = crc;
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_check_crc(const uint8_t *data, uint16_t count,
                                    uint8_t checksum)
{
  uint8_t crc_val = 0;
  sl_status_t stt = sensirion_i2c_generate_crc(data, count, &crc_val);
  if ((SL_STATUS_OK != stt) || (crc_val != checksum)) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

sl_status_t sensirion_i2c_general_call_reset(void)
{
  const uint8_t data = 0x06;
  return sensirion_i2c_hal_write(0, &data, (uint16_t)sizeof(data));
}

sl_status_t sensirion_i2c_fill_cmd_send_buf(uint8_t *buf,
                                            uint16_t cmd,
                                            const uint16_t *args,
                                            uint8_t num_args,
                                            uint16_t *buff_size)
{
  if ((NULL != buf) && (NULL != buff_size)) {
    uint8_t i;
    uint16_t idx = 0;
    uint8_t crc;
    sl_status_t stt;

    buf[idx++] = (uint8_t)((cmd & 0xFF00) >> 8);
    buf[idx++] = (uint8_t)((cmd & 0x00FF) >> 0);

    if ((num_args > 0) && (NULL != args)) {
      for (i = 0; i < num_args; ++i) {
        buf[idx++] = (uint8_t)((args[i] & 0xFF00) >> 8);
        buf[idx++] = (uint8_t)((args[i] & 0x00FF) >> 0);

        stt = sensirion_i2c_generate_crc((uint8_t *)&buf[idx - 2],
                                         SENSIRION_WORD_SIZE,
                                         &crc);
        if (stt) {
          return stt;
        }
        buf[idx++] = crc;
      }
    }

    *buff_size = idx;
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_read_words_as_bytes(uint8_t address, uint8_t *data,
                                              uint16_t num_words)
{
  if (NULL != data) {
    sl_status_t ret;
    uint16_t i = 0, j = 0;
    uint16_t size = num_words * (SENSIRION_WORD_SIZE + CRC8_LEN);
    uint16_t word_buf[SENSIRION_MAX_BUFFER_WORDS];
    uint8_t * const buf8 = (uint8_t *)word_buf;

    ret = sensirion_i2c_hal_read(address, buf8, size);
    if (ret != SL_STATUS_OK) {
      return ret;
    }

    /* check the CRC for each word */
    for (i = 0; i < size; i += (SENSIRION_WORD_SIZE + CRC8_LEN)) {
      ret = sensirion_i2c_check_crc(&buf8[i], SENSIRION_WORD_SIZE,
                                    buf8[i + SENSIRION_WORD_SIZE]);
      if (ret != SL_STATUS_OK) {
        return ret;
      }

      data[j++] = buf8[i];
      data[j++] = buf8[i + 1];
    }
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_read_words(uint8_t address, uint16_t *data_words,
                                     uint16_t num_words)
{
  if (NULL != data_words) {
    sl_status_t ret;
    uint8_t i;

    ret = sensirion_i2c_read_words_as_bytes(address, (uint8_t *)data_words,
                                            num_words);
    if (ret != SL_STATUS_OK) {
      return ret;
    }

    for (i = 0; i < num_words; ++i) {
      const uint8_t *word_bytes = (uint8_t *)&data_words[i];
      data_words[i] = ((uint16_t)word_bytes[0] << 8) | word_bytes[1];
    }
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_write_cmd(uint8_t address, uint16_t command)
{
  uint8_t buf[SENSIRION_COMMAND_SIZE] = { 0 };
  uint16_t buff_size = 0;

  sl_status_t stt = sensirion_i2c_fill_cmd_send_buf(buf,
                                                    command,
                                                    NULL,
                                                    0,
                                                    &buff_size);
  if (stt) {
    return stt;
  }

  return sensirion_i2c_hal_write(address, buf, SENSIRION_COMMAND_SIZE);
}

sl_status_t sensirion_i2c_write_cmd_with_args(uint8_t address, uint16_t command,
                                              const uint16_t *data_words,
                                              uint16_t num_words)
{
  uint8_t buf[SENSIRION_MAX_BUFFER_WORDS] = { 0 };
  uint16_t buf_size = 0;

  sl_status_t stt = sensirion_i2c_fill_cmd_send_buf(buf,
                                                    command,
                                                    data_words,
                                                    num_words,
                                                    &buf_size);
  if (stt) {
    return stt;
  }

  return sensirion_i2c_hal_write(address, buf, buf_size);
}

sl_status_t sensirion_i2c_delayed_read_cmd(uint8_t address,
                                           uint16_t cmd,
                                           uint32_t delay_us,
                                           uint16_t *data_words,
                                           uint16_t num_words)
{
  sl_status_t stt;
  uint8_t buf[SENSIRION_COMMAND_SIZE];
  uint16_t buff_size = 0;

  stt = sensirion_i2c_fill_cmd_send_buf(buf, cmd, NULL, 0, &buff_size);

  if (stt != SL_STATUS_OK) {
    return stt;
  }

  stt = sensirion_i2c_hal_write(address, buf, SENSIRION_COMMAND_SIZE);
  if (stt != SL_STATUS_OK) {
    return stt;
  }

  if (delay_us) {
    sensirion_i2c_hal_sleep_usec(delay_us);
  }

  return sensirion_i2c_read_words(address, data_words, num_words);
}

sl_status_t sensirion_i2c_read_cmd(uint8_t address, uint16_t cmd,
                                   uint16_t *data_words, uint16_t num_words)
{
  return sensirion_i2c_delayed_read_cmd(address, cmd, 0, data_words,
                                        num_words);
}

sl_status_t sensirion_i2c_add_command_to_buffer(uint8_t *buffer,
                                                uint16_t offset,
                                                uint16_t command,
                                                uint16_t *offset_out)
{
  if ((NULL != buffer) && (NULL != offset_out)) {
    buffer[offset++] = (uint8_t)((command & 0xFF00) >> 8);
    buffer[offset++] = (uint8_t)((command & 0x00FF) >> 0);
    *offset_out = offset;

    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_add_uint32_t_to_buffer(uint8_t *buffer,
                                                 uint16_t offset,
                                                 uint32_t data,
                                                 uint16_t *offset_out)
{
  if ((NULL != buffer) && (NULL != offset_out)) {
    uint8_t crc;
    sl_status_t stt;

    buffer[offset++] = (uint8_t)((data & 0xFF000000) >> 24);
    buffer[offset++] = (uint8_t)((data & 0x00FF0000) >> 16);
    stt = sensirion_i2c_generate_crc(&buffer[offset - SENSIRION_WORD_SIZE],
                                     SENSIRION_WORD_SIZE, &crc);
    if (stt) {
      return stt;
    }
    buffer[offset++] = crc;

    buffer[offset++] = (uint8_t)((data & 0x0000FF00) >> 8);
    buffer[offset++] = (uint8_t)((data & 0x000000FF) >> 0);
    stt = sensirion_i2c_generate_crc(&buffer[offset - SENSIRION_WORD_SIZE],
                                     SENSIRION_WORD_SIZE, &crc);
    if (stt) {
      return stt;
    }
    buffer[offset++] = crc;

    *offset_out = offset;
    return SL_STATUS_OK;
  }

  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_add_int32_t_to_buffer(uint8_t *buffer,
                                                uint16_t offset,
                                                int32_t data,
                                                uint16_t *offset_out)
{
  return sensirion_i2c_add_uint32_t_to_buffer(buffer,
                                              offset,
                                              (uint32_t)data,
                                              offset_out);
}

sl_status_t sensirion_i2c_add_uint16_t_to_buffer(uint8_t *buffer,
                                                 uint16_t offset,
                                                 uint16_t data,
                                                 uint16_t *offset_out)
{
  if ((NULL != buffer) && (NULL != offset_out)) {
    sl_status_t stt;
    uint8_t crc;

    buffer[offset++] = (uint8_t)((data & 0xFF00) >> 8);
    buffer[offset++] = (uint8_t)((data & 0x00FF) >> 0);
    stt = sensirion_i2c_generate_crc(&buffer[offset - SENSIRION_WORD_SIZE],
                                     SENSIRION_WORD_SIZE, &crc);
    if (stt) {
      return stt;
    }
    buffer[offset++] = crc;

    *offset_out = offset;
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_add_int16_t_to_buffer(uint8_t *buffer,
                                                uint16_t offset,
                                                int16_t data,
                                                uint16_t *offset_out)
{
  return sensirion_i2c_add_uint16_t_to_buffer(buffer,
                                              offset,
                                              (uint16_t)data,
                                              offset_out);
}

sl_status_t sensirion_i2c_add_float_to_buffer(uint8_t *buffer, uint16_t offset,
                                              float data, uint16_t *offset_out)
{
  if ((NULL != buffer) && (NULL != offset_out)) {
    sl_status_t stt;
    uint8_t crc;

    union {
      uint32_t uint32_data;
      float float_data;
    } convert;

    convert.float_data = data;

    buffer[offset++] = (uint8_t)((convert.uint32_data & 0xFF000000) >> 24);
    buffer[offset++] = (uint8_t)((convert.uint32_data & 0x00FF0000) >> 16);
    stt = sensirion_i2c_generate_crc(&buffer[offset - SENSIRION_WORD_SIZE],
                                     SENSIRION_WORD_SIZE, &crc);
    if (stt) {
      return stt;
    }
    buffer[offset++] = crc;

    buffer[offset++] = (uint8_t)((convert.uint32_data & 0x0000FF00) >> 8);
    buffer[offset++] = (uint8_t)((convert.uint32_data & 0x000000FF) >> 0);
    stt = sensirion_i2c_generate_crc(&buffer[offset - SENSIRION_WORD_SIZE],
                                     SENSIRION_WORD_SIZE, &crc);
    if (stt) {
      return stt;
    }
    buffer[offset++] = crc;

    *offset_out = offset;
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_add_bytes_to_buffer(uint8_t *buffer,
                                              uint16_t offset,
                                              const uint8_t *data,
                                              uint16_t data_length,
                                              uint16_t *offset_out)
{
  if ((NULL != buffer) && (NULL != data) && (NULL != offset_out)) {
    uint16_t i;
    sl_status_t stt;
    uint8_t crc;

    if (data_length % SENSIRION_WORD_SIZE != 0) {
      return BYTE_NUM_ERROR;
    }

    for (i = 0; i < data_length; i += 2) {
      buffer[offset++] = data[i];
      buffer[offset++] = data[i + 1];
      stt = sensirion_i2c_generate_crc(&buffer[offset - SENSIRION_WORD_SIZE],
                                       SENSIRION_WORD_SIZE, &crc);
      if (stt) {
        return stt;
      }
      buffer[offset++] = crc;
    }

    *offset_out = offset;
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

sl_status_t sensirion_i2c_write_data(uint8_t address, const uint8_t *data,
                                     uint16_t data_length)
{
  return sensirion_i2c_hal_write(address, data, data_length);
}

sl_status_t sensirion_i2c_read_data_inplace(uint8_t address, uint8_t *buffer,
                                            uint16_t expected_data_length)
{
  if (NULL != buffer) {
    sl_status_t stt;
    uint16_t i, j;
    uint16_t size = (expected_data_length / SENSIRION_WORD_SIZE)
                    * (SENSIRION_WORD_SIZE + CRC8_LEN);

    if (expected_data_length % SENSIRION_WORD_SIZE != 0) {
      return SL_STATUS_FAIL;
    }

    stt = sensirion_i2c_hal_read(address, buffer, size);
    if (stt) {
      return stt;
    }

    for (i = 0, j = 0; i < size; i += (SENSIRION_WORD_SIZE + CRC8_LEN)) {
      stt = sensirion_i2c_check_crc(&buffer[i], SENSIRION_WORD_SIZE,
                                    buffer[i + SENSIRION_WORD_SIZE]);
      if (stt) {
        return stt;
      }
      buffer[j++] = buffer[i];
      buffer[j++] = buffer[i + 1];
    }

    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}
