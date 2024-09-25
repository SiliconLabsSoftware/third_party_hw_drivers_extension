
#ifndef MIKROE_SSD1351_H_
#define MIKROE_SSD1351_H_

#include "oledc.h"
#include "sl_status.h"
#include "drv_spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup ssd1351 SSD1351 - OLED C Click
 * @brief  Driver for the Mikroe OLED C Click
 *
 *    @n @section ssd1351_example SSD1351 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_spidrv_instances.h"
 * #include "mikroe_ssd1351.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_ssd1351_init(sl_spidrv_mikroe_handle);
 *      mikroe_ssd1351_default_cfg();
 *
 *      while (true) {
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/**
 * \defgroup font_dir Font Direction
 * \{
 */
#define MIKROE_SSD1351_FO_HORIZONTAL         0x00
#define MIKROE_SSD1351_FO_VERTICAL           0x01
#define MIKROE_SSD1351_FO_VERTICAL_COLUMN    0x02

/** \} */

/**
 * \defgroup remamp_set Remamp Settings
 * \{
 */
#define MIKROE_SSD1351_RMP_INC_HOR           0x00
#define MIKROE_SSD1351_RMP_INC_VER           0x01
#define MIKROE_SSD1351_RMP_COLOR_NOR         0x00
#define MIKROE_SSD1351_RMP_COLOR_REV         0x02
#define MIKROE_SSD1351_RMP_SEQ_RGB           0x00
#define MIKROE_SSD1351_RMP_SEQ_BGR           0x04
#define MIKROE_SSD1351_RMP_SCAN_NOR          0x00
#define MIKROE_SSD1351_RMP_SCAN_REV          0x10
#define MIKROE_SSD1351_RMP_SPLIT_DISABLE     0x00
#define MIKROE_SSD1351_RMP_SPLIT_ENABLE      0x20
#define MIKROE_SSD1351_COLOR_65K             0x00
#define MIKROE_SSD1351_COLOR_262K            0x80
#define MIKROE_SSD1351_IMG_HEAD              0x06

/** \} */

/**
 * \defgroup dev_proper Device Properties
 * \{
 */
#define MIKROE_SSD1351_SCREEN_WIDTH          0x60
#define MIKROE_SSD1351_SCREEN_HEIGHT         0x60
#define MIKROE_SSD1351_SCREEN_SIZE           0x2400
#define MIKROE_SSD1351_ROW_OFF               0x00
#define MIKROE_SSD1351_COL_OFF               0x10

/** \} */

/**
 * \defgroup commands Commands
 * \{
 */
#define MIKROE_SSD1351_SET_COL_ADDRESS       0x15
#define MIKROE_SSD1351_SET_ROW_ADDRESS       0x75
#define MIKROE_SSD1351_WRITE_RAM             0x5C
#define MIKROE_SSD1351_READ_RAM              0x5D
#define MIKROE_SSD1351_SET_REMAP             0xA0
#define MIKROE_SSD1351_SET_START_LINE        0xA1
#define MIKROE_SSD1351_SET_OFFSET            0xA2
#define MIKROE_SSD1351_MODE_OFF              0xA4
#define MIKROE_SSD1351_MODE_ON               0xA5
#define MIKROE_SSD1351_MODE_NORMAL           0xA6
#define MIKROE_SSD1351_MODE_INVERSE          0xA7
#define MIKROE_SSD1351_FUNCTION              0xAB
#define MIKROE_SSD1351_SLEEP_ON              0xAE
#define MIKROE_SSD1351_SLEEP_OFF             0xAF
#define MIKROE_SSD1351_NOP                   0xB0
#define MIKROE_SSD1351_SET_RESET_PRECH       0xB1
#define MIKROE_SSD1351_ENHANCEMENT           0xB2
#define MIKROE_SSD1351_CLOCK_DIV             0xB3
#define MIKROE_SSD1351_VSL                   0xB4
#define MIKROE_SSD1351_GPIO                  0xB5
#define MIKROE_SSD1351_SETSEC_PRECH          0xB6
#define MIKROE_SSD1351_GREY_SCALE            0xB8
#define MIKROE_SSD1351_LUT                   0xB9
#define MIKROE_SSD1351_PRECH_VOL             0xBB
#define MIKROE_SSD1351_VCOMH                 0xBE
#define MIKROE_SSD1351_CONTRAST              0xC1
#define MIKROE_SSD1351_MASTER_CONTRAST       0xC7
#define MIKROE_SSD1351_MUX_RATIO             0xCA
#define MIKROE_SSD1351_COMMAND_LOCK          0xFD
#define MIKROE_SSD1351_SCROLL_HOR            0x96
#define MIKROE_SSD1351_START_MOV             0x9E
#define MIKROE_SSD1351_STOP_MOV              0x9F

#define MIKROE_SSD1351_DEFAULT_MUX_RATIO     95
#define MIKROE_SSD1351_DEFAULT_START_LINE    0x80
#define MIKROE_SSD1351_DEFAULT_OFFSET        0x20

#define MIKROE_SSD1351_DEFAULT_OLED_LOCK     0x12
#define MIKROE_SSD1351_DEFAULT_CMD_LOCK      0xB1
#define MIKROE_SSD1351_DEFAULT_DIVSET        0xF1
#define MIKROE_SSD1351_DEFAULT_PRECHARGE     0x32
#define MIKROE_SSD1351_DEFAULT_VCOMH         0x05
#define MIKROE_SSD1351_DEFAULT_MASTER_CONT   0xCF
#define MIKROE_SSD1351_DEFAULT_PRECHARGE_2   0x01

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_init(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the SPI instance used by platform functions
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_spi_instance(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    Default Configuration function.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_default_cfg(void);

/***************************************************************************//**
 * @brief
 *    Enable/Disable display.
 *
 * @param[in] state  1 - ON / 0 - OFF.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_enable(bool state);

/***************************************************************************//**
 * @brief
 *    Hardware Reset.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_reset(void);

/***************************************************************************//**
 * @brief
 *    Command Send with more argument.
 *
 * @param[in] command  Command to be sent.
 * @param[in] args  Pointer to command argument.
 * @param[in] args_len  Argument size in bytes.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_more_arg_commands(uint8_t command,
                                             uint8_t *args,
                                             uint16_t args_len);

/***************************************************************************//**
 * @brief
 *    Command Send with one argument.
 *
 * @param[in] ctx  Context object.
 * @param[in] cmd  Command to be sent.
 * @param[in] args One command argument.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_one_arg_commands(uint8_t command, uint8_t args);

/***************************************************************************//**
 * @brief
 *    Fill Screen.
 *
 * @param[in] color  RGB color.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_fill_screen(uint16_t color);

/***************************************************************************//**
 * @brief
 *    Draw Rectangle.
 *
 * @param[in] col_off  Column offset from the left border of the screen.
 * @param[in] row_off  Row offset from the top border of the screen.
 * @param[in] col_end  Column end offset also counted from the left border.
 * @param[in] row_end  Row offset also counted from the top border.
 * @param[in] color  RGB color.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_rectangle(uint8_t col_off,
                                     uint8_t row_off,
                                     uint8_t col_end,
                                     uint8_t row_end,
                                     uint16_t color);

/***************************************************************************//**
 * @brief
 *    Draw BMP Image.
 *
 * @param[in] col_off  Column offset from the left border of the screen.
 * @param[in] row_off  Row offset from the top border of the screen.
 * @param[in] img  Pointer to bmp image.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_image(const uint8_t *img,
                                 uint8_t col_off,
                                 uint8_t row_off);

/***************************************************************************//**
 * @brief
 *    Draw Text.
 *
 * @param[in] text  Text string.
 * @param[in] x  Column offset from the left border of the screen.
 * @param[in] y  Row offset from the top border of the screen.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_text(uint8_t *text, uint16_t x, uint16_t y);

/***************************************************************************//**
 * @brief
 *    Font Setup.
 *
 * @param[in] font_s  Pointer to font definition.
 * @param[in] color  Text color.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_font(const uint8_t *font_s, uint16_t color);

/***************************************************************************//**
 * @brief
 *    Set Contrast.
 *
 * @param[in] contrast  0-255.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_contrast(uint8_t contrast);

#ifdef __cplusplus
}
#endif

#endif
