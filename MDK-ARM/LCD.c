#include "LCD.h"
#include <stdio.h>
extern I2C_HandleTypeDef hi2c1;
uint8_t lcdCommandBuffer[6] = {0x00};

static LCDParams lcdParams;

static bool lcdWriteByte(uint8_t rsRwBits, uint8_t * data);

/**
 * @brief  Turn display on and init it params
 * @note   We gonna make init steps according to datasheep page 46.
 *         There are 4 steps to turn 4-bits mode on,
 *         then we send initial params.
 * @param  hi2c    I2C struct to which display is connected
 * @param  address Display I2C 7-bit address
 * @param  lines   Number of lines of display
 * @param  columns Number of colums
 * @return         true if success
 */
bool lcdInit(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t lines, uint8_t columns) {


    uint8_t lcdData = LCD_BIT_5x8DOTS;

    lcdParams.hi2c      = hi2c;
    lcdParams.address   = address << 1;
    lcdParams.lines     = lines;
    lcdParams.columns   = columns;
    lcdParams.backlight = LCD_BIT_BACKIGHT_ON;

    lcdCommandBuffer[0] = LCD_BIT_E | (0x03 << 4);
    lcdCommandBuffer[1] = lcdCommandBuffer[0];
    lcdCommandBuffer[2] = (0x03 << 4);

    /* First 3 steps of init cycles. They are the same. */
    for (uint8_t i = 0; i < 3; ++i) 
    {
        if (HAL_I2C_Master_Transmit(lcdParams.hi2c, lcdParams.address, (uint8_t*)lcdCommandBuffer, 3,100) != HAL_OK) {
            return false;
        }

        while (HAL_I2C_GetState(lcdParams.hi2c) != HAL_I2C_STATE_READY) {
            HAL_Delay(1);
        }
    }

    /* Lets turn to 4-bit at least */
    lcdCommandBuffer[0] = LCD_BIT_BACKIGHT_ON | LCD_BIT_E | (LCD_MODE_4BITS << 4);
    lcdCommandBuffer[1] = lcdCommandBuffer[0];
    lcdCommandBuffer[2] = LCD_BIT_BACKIGHT_ON | (LCD_MODE_4BITS << 4);

    if (HAL_I2C_Master_Transmit(lcdParams.hi2c, lcdParams.address, (uint8_t*)lcdCommandBuffer, 3,100) != HAL_OK) {
        return false;
    }

    while (HAL_I2C_GetState(lcdParams.hi2c) != HAL_I2C_STATE_READY) {
            HAL_Delay(1);
    }

    /* Lets set display params */
    /* First of all lets set display size */
    lcdData |= LCD_MODE_4BITS;

    if (lcdParams.lines > 1) {
        lcdData |= LCD_BIT_2LINE;
    }

    lcdWriteByte((uint8_t)0x00, &lcdData);  // TODO: Make 5x10 dots font usable for some 1-line display

    /* Now lets set display, cursor and blink all on */
    lcdDisplayOn();

    /* Set cursor moving to the right */
    lcdCursorDirToRight();

    /* Clear display and Set cursor at Home */
    lcdDisplayClear();
    lcdCursorHome();

    return true;
}

/**
 * @brief  Send command to display
 * @param  command  One of listed in LCDCommands enum
 * @param  action   LCD_PARAM_SET or LCD_PARAM_UNSET
 * @return          true if success
 */
bool lcdCommand(LCDCommands command, LCDParamsActions action)
{
    uint8_t lcdData = 0x00;

    /* First of all lest store the command */
    switch (action) {
        case LCD_PARAM_SET:
            switch (command) {
                case LCD_DISPLAY:
                    lcdParams.modeBits |=  LCD_BIT_DISPLAY_ON;
                    break;

                case LCD_CURSOR:
                    lcdParams.modeBits |= LCD_BIT_CURSOR_ON;
                    break;

                case LCD_CURSOR_BLINK:
                    lcdParams.modeBits |= LCD_BIT_BLINK_ON;
                    break;

                case LCD_CLEAR:
                    lcdData = LCD_BIT_DISP_CLEAR;

                    if (lcdWriteByte((uint8_t)0x00, &lcdData) == false) {
                        return false;
                    } else {
                        HAL_Delay(2);
                        return true;
                    }

                case LCD_CURSOR_HOME:
                    lcdData = LCD_BIT_CURSOR_HOME;

                    if (lcdWriteByte((uint8_t)0x00, &lcdData) == false) {
                        return false;
                    } else {
                        HAL_Delay(2);
                        return true;
                    }

                case LCD_CURSOR_DIR_RIGHT:
                    lcdParams.entryBits |= LCD_BIT_CURSOR_DIR_RIGHT;
                    break;

                case LCD_CURSOR_DIR_LEFT:
                    lcdParams.entryBits |= LCD_BIT_CURSOR_DIR_LEFT;
                    break;

                case LCD_DISPLAY_SHIFT:
                    lcdParams.entryBits |= LCD_BIT_DISPLAY_SHIFT;
                    break;

                default:
                    return false;
            }

            break;

        case LCD_PARAM_UNSET:
            switch (command) {
                case LCD_DISPLAY:
                    lcdParams.modeBits &= ~LCD_BIT_DISPLAY_ON;
                    break;

                case LCD_CURSOR:
                    lcdParams.modeBits &= ~LCD_BIT_CURSOR_ON;
                    break;

                case LCD_CURSOR_BLINK:
                    lcdParams.modeBits &= ~LCD_BIT_BLINK_ON;
                    break;

                case LCD_CURSOR_DIR_RIGHT:
                    lcdParams.entryBits &= ~LCD_BIT_CURSOR_DIR_RIGHT;
                    break;

                case LCD_CURSOR_DIR_LEFT:
                    lcdParams.entryBits &= ~LCD_BIT_CURSOR_DIR_LEFT;
                    break;

                case LCD_DISPLAY_SHIFT:
                    lcdParams.entryBits &= ~LCD_BIT_DISPLAY_SHIFT;
                    break;

                default:
                    return false;
            }

            break;

        default:
            return false;
    }

    /* Now lets send the command */
    switch (command) {
        case LCD_DISPLAY:
        case LCD_CURSOR:
        case LCD_CURSOR_BLINK:
            lcdData = LCD_BIT_DISPLAY_CONTROL | lcdParams.modeBits;
            break;

        case LCD_CURSOR_DIR_RIGHT:
        case LCD_CURSOR_DIR_LEFT:
        case LCD_DISPLAY_SHIFT:
            lcdData = LCD_BIT_ENTRY_MODE | lcdParams.entryBits;
            break;

        default:
            break;
    }

    return lcdWriteByte((uint8_t)0x00, &lcdData);
}

/**
 * @brief  Turn display's Backlight On or Off
 * @param  command LCD_BIT_BACKIGHT_ON to turn display On
 *                 LCD_BIT_BACKIGHT_OFF (or 0x00) to turn display Off
 * @return         true if success
 */
bool lcdBacklight(uint8_t command) {
    lcdParams.backlight = command;

    if (HAL_I2C_Master_Transmit(lcdParams.hi2c, lcdParams.address, &lcdParams.backlight, 1, 100) != HAL_OK) {
        return false;
    }

    while (HAL_I2C_GetState(lcdParams.hi2c) != HAL_I2C_STATE_READY) {
        HAL_Delay(1);
    }

    return true;
}

/**
 * @brief  Set cursor position on the display
 * @param  column counting from 0
 * @param  line   counting from 0
 * @return        true if success
 */
bool lcdSetCursorPosition(uint8_t column, uint8_t line) {
    // We will setup offsets for 4 lines maximum
    static const uint8_t lineOffsets[4] = { 0x00, 0x40, 0x14, 0x54 };

    if ( line >= lcdParams.lines ) {
        line = lcdParams.lines - 1;
    }

    uint8_t lcdCommand = LCD_BIT_SETDDRAMADDR | (column + lineOffsets[line]);

    return lcdWriteByte(0x00, &lcdCommand);
}

/**
 * @brief  Print string from cursor position
 * @param  data   Pointer to string
 * @param  length Number of symbols to print
 * @return        true if success
 */
bool lcdPrintStr(uint8_t * data, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i) {
        if (lcdWriteByte(LCD_BIT_RS, &data[i]) == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief  Print single char at cursor position
 * @param  data Symbol to print
 * @return      true if success
 */
bool lcdPrintChar(uint8_t data) {
    return lcdWriteByte(LCD_BIT_RS, &data);
}


/**
 * @brief Loading custom Chars to one of the 8 cells in CGRAM
 * @note  You can create your custom chars according to
 *        documentation page 15.
 *        It consists of array of 8 bytes.
 *        Each byte is line of dots. Lower bits are dots.
 * @param  cell     Number of cell from 0 to 7 where to upload
 * @param  charMap  Pointer to Array of dots
 *                  Example: { 0x07, 0x09, 0x09, 0x09, 0x09, 0x1F, 0x11 }
 * @return          true if success
 */
bool lcdLoadCustomChar(uint8_t cell, uint8_t * charMap) {

    // Stop, if trying to load to incorrect cell
    if (cell > 7) {
        return false;
    }

    uint8_t lcdCommand = LCD_BIT_SETCGRAMADDR | (cell << 3);

    if (lcdWriteByte((uint8_t)0x00, &lcdCommand) == false) {
        return false;
    }

    for (uint8_t i = 0; i < 8; ++i) {
        if (lcdWriteByte(LCD_BIT_RS, &charMap[i]) == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief  Local function to send data to display
 * @param  rsRwBits State of RS and R/W bits
 * @param  data     Pointer to byte to send
 * @return          true if success
 */
static bool lcdWriteByte(uint8_t rsRwBits, uint8_t * data) {

    /* Higher 4 bits*/
    lcdCommandBuffer[0] = rsRwBits | LCD_BIT_E | lcdParams.backlight | (*data & 0xF0);  // Send data and set strobe
    lcdCommandBuffer[1] = lcdCommandBuffer[0];                                          // Strobe turned on
    lcdCommandBuffer[2] = rsRwBits | lcdParams.backlight | (*data & 0xF0);              // Turning strobe off

    /* Lower 4 bits*/
    lcdCommandBuffer[3] = rsRwBits | LCD_BIT_E | lcdParams.backlight | ((*data << 4) & 0xF0);  // Send data and set strobe
    lcdCommandBuffer[4] = lcdCommandBuffer[3];                                                 // Strobe turned on
    lcdCommandBuffer[5] = rsRwBits | lcdParams.backlight | ((*data << 4) & 0xF0);              // Turning strobe off


    if (HAL_I2C_Master_Transmit(lcdParams.hi2c, lcdParams.address, (uint8_t*)lcdCommandBuffer, 6,100) != HAL_OK) {
        return false;
    }

    while (HAL_I2C_GetState(lcdParams.hi2c) != HAL_I2C_STATE_READY) {
        HAL_Delay(1);
    }

    return true;
}

/******************************************************************************/
void lcd_send_cmd (char cmd)
{
    RET_STAT fRet = RET_FAIL;
    char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}
void lcd_send_data (char data)
{
    char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}
void lcd_init (void)
{
    // 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
    
    RET_OK;
}
void lcd_clear(void)
{
    lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
}
void lcd_send_string (char *str)
{
    while (*str) lcd_send_data (*str++);
}
RET_STAT I2C_Scan(I2C_HandleTypeDef *iI2cPort, UART_HandleTypeDef *iLogPort)
{
    HAL_StatusTypeDef res;
    char info[] = "Scanning I2C bus...\r\n";
    
    HAL_UART_Transmit(iLogPort, (uint8_t*)info, strlen(info), HAL_MAX_DELAY);

    for(uint16_t i = 0; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(iI2cPort, i << 1, 1, 10);
        if(res == HAL_OK) {
            char msg[64];
            snprintf(msg, sizeof(msg), "0x%02X(0x%02X)", i,i<<1);
            HAL_UART_Transmit(iLogPort, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        } else {
            HAL_UART_Transmit(iLogPort, (uint8_t*)".", 1, HAL_MAX_DELAY);
        }
    }

    HAL_UART_Transmit(iLogPort, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
   return RET_OK;
}
