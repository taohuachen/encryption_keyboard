/**
 *******************************************************************************
 * @file    : 
 * @author  : 
 * @version : 
 * @date    : 
 * @brief   : 
 *******************************************************************************
 * @attention
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_DEF_H
#define __LIB_DEF_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/**
 * @boolean defines
 */
#define DEF_NULL 0u

#define DEF_FALSE 0u
#define DEF_TRUE 1u

#define DEF_NO 0u
#define DEF_YES 1u

#define DEF_DISABLED 0u
#define DEF_ENABLED 1u

#define DEF_INACTIVE 0u
#define DEF_ACTIVE 1u

#define DEF_INVALID 0u
#define DEF_VALID 1u

#define DEF_OFF 0u
#define DEF_ON 1u

#define DEF_CLR 0u
#define DEF_SET 1u

#define DEF_FAIL 0u
#define DEF_OK 1u

/**
 * @bit defines
 */
#define BIT_NONE 0x00u

#define BIT_00 0x01u
#define BIT_01 0x02u
#define BIT_02 0x04u
#define BIT_03 0x08u
#define BIT_04 0x10u
#define BIT_05 0x20u
#define BIT_06 0x40u
#define BIT_07 0x80u

#define BIT_08 0x0100u
#define BIT_09 0x0200u
#define BIT_10 0x0400u
#define BIT_11 0x0800u
#define BIT_12 0x1000u
#define BIT_13 0x2000u
#define BIT_14 0x4000u
#define BIT_15 0x8000u

#define BIT_16 0x00010000u
#define BIT_17 0x00020000u
#define BIT_18 0x00040000u
#define BIT_19 0x00080000u
#define BIT_20 0x00100000u
#define BIT_21 0x00200000u
#define BIT_22 0x00400000u
#define BIT_23 0x00800000u

#define BIT_24 0x01000000u
#define BIT_25 0x02000000u
#define BIT_26 0x04000000u
#define BIT_27 0x08000000u
#define BIT_28 0x10000000u
#define BIT_29 0x20000000u
#define BIT_30 0x40000000u
#define BIT_31 0x80000000u

#define BIT_32 0x0000000100000000u
#define BIT_33 0x0000000200000000u
#define BIT_34 0x0000000400000000u
#define BIT_35 0x0000000800000000u
#define BIT_36 0x0000001000000000u
#define BIT_37 0x0000002000000000u
#define BIT_38 0x0000004000000000u
#define BIT_39 0x0000008000000000u

#define BIT_40 0x0000010000000000u
#define BIT_41 0x0000020000000000u
#define BIT_42 0x0000040000000000u
#define BIT_43 0x0000080000000000u
#define BIT_44 0x0000100000000000u
#define BIT_45 0x0000200000000000u
#define BIT_46 0x0000400000000000u
#define BIT_47 0x0000800000000000u

#define BIT_48 0x0001000000000000u
#define BIT_49 0x0002000000000000u
#define BIT_50 0x0004000000000000u
#define BIT_51 0x0008000000000000u
#define BIT_52 0x0010000000000000u
#define BIT_53 0x0020000000000000u
#define BIT_54 0x0040000000000000u
#define BIT_55 0x0080000000000000u

#define BIT_56 0x0100000000000000u
#define BIT_57 0x0200000000000000u
#define BIT_58 0x0400000000000000u
#define BIT_59 0x0800000000000000u
#define BIT_60 0x1000000000000000u
#define BIT_61 0x2000000000000000u
#define BIT_62 0x4000000000000000u
#define BIT_63 0x8000000000000000u

/* Exported macro ------------------------------------------------------------*/
/**        创建bit位为1的数据
 * @brief  Create bit mask with single, specified bit set.
 * @param  bit：Bit number of bit to set.
 * @retval Bit mask with single, specified bit set.
 */
#define DEF_BIT(bit) (1u << (bit))

/**        数据bit_mask左移bit_shift位
 * @brief  Shift a bit mask.
 * @param  bit_mask：Bit mask to shift.
 * @param  bit_shift：Number of bit positions to left-shift bit mask.
 * @retval Shifted bit mask.
 */
#define DEF_BIT_MASK(bit_mask, bit_shift) ((bit_mask) << (bit_shift))

/**        数据val在指定位置1
 * @brief  Set specified bit(s) in a value.
 * @param  val：Value to modify by setting specified bit(s).
 * @param  mask：Mask of bits to set.
 * @retval Modified value with specified bit(s) set.
 */
#define DEF_BIT_SET(val, mask) ((val) |= (mask))

/**        数据val在指定位清0
 * @brief  Clear specified bit(s) in a value.
 * @param  val：Value to modify by clearing specified bit(s).
 * @param  mask：Mask of bits to clear.
 * @retval Modified value with specified bit(s) clear.
 */
#define DEF_BIT_CLR(val, mask) ((val) &= ~(mask))

/**        数据val在指定位取反
 * @brief  Toggles specified bit(s) in a value.
 * @param  val：Value to modify by toggling specified bit(s).
 * @param  mask：Mask of bits to toggle.
 * @retval Modified value with specified bit(s) toggled.
 */
#define DEF_BIT_TOGGLE(val, mask) ((val) ^= (mask))

/**        数据val读取字段field_mask（field_mask中的1必须是连续的）的值，
           并将值右移至bit0位。
           举例：val = bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
                 field_mask = BIT_04 + BIT_03 + BIT_02
                          BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0
                 返回值 =  0    0    0    0    0   bit4 bit3 bit2
 * @brief  Reads a 'val' field, masked and shifted, given by mask 'field_mask'.
 * @param  val：Value to read from.
 * @param  field_mask：Mask of field to read.
 * @retval Field value, masked and right-shifted to bit position 0.
 */
#define DEF_BIT_FIELD_RD(val, field_mask) (((val) & (field_mask)) / ((field_mask) & ~((field_mask) << 1u)))

/**        数据field_val从bit0读取字段field_mask（field_mask中的1必须是连续的）数量的值，
           并将值左移至字段的最高位。
           举例：field_val = bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
                 field_mask = BIT_04 + BIT_03 + BIT_02
                          BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0
                 返回值 =  0    0    0   bit2 bit1 bit0  0    0
 * @brief  Encodes given 'field_val' at position given by mask 'field_mask'.
 * @param  field_val：Value to encode.
 * @param  field_mask：Mask of field to read.
 * @retval Field value, masked and left-shifted to field position.
 */
#define DEF_BIT_FIELD_ENC(field_val, field_mask) (((field_val) * ((field_mask) & ~((field_mask) << 1u))) & (field_mask))

/**        在字段field_mask（field_mask中的1必须是连续的）写数据field_val到变量var
           数据field_val不能超过字段field_mask允许的最大值，否则var不变
           举例：var初始值 = bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
                 field_val = 2（二进制码为10）
                 field_mask = BIT_04 + BIT_03 + BIT_02（3位，允许的最大值为7）
                             BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0
                 var计算值 = bit7 bit6 bit5  0    1    0   bit1 bit0
 * @brief  Writes 'field_val' field at position given by mask 'field_mask' in variable 'var'.
 * @param  var：Variable to write field to.
 * @param  field_val：Desired value for field.
 * @param  field_mask：Mask of field to write to.
 * @retval None.
 */
#define DEF_BIT_FIELD_WR(var, field_val, field_mask) (var) = (((var) & ~(field_mask)) | DEF_BIT_FIELD_ENC((field_val), (field_mask)))

/**        数据val指定位是否全为1
 * @brief  Determine if specified bit(s) in a value are set.
 * @param  val：Value to check for specified bit(s) set.
 * @param  mask：Mask of bits to check if set.
 * @retval DEF_YES, if ALL specified bit(s) are     set in value.
           DEF_NO,  if ALL specified bit(s) are NOT set in value.
 */
#define DEF_BIT_IS_SET(val, mask) (((((val) & (mask)) == (mask)) && ((mask) != 0u)) ? (DEF_YES) : (DEF_NO))

/**        数据val指定位是否全为0
 * @brief  Determine if specified bit(s) in a value are clear.
 * @param  val：Value to check for specified bit(s) clear.
 * @param  mask：Mask of bits to check if clear.
 * @retval DEF_YES, if ALL specified bit(s) are     clear in value.
           DEF_NO,  if ALL specified bit(s) are NOT clear in value.
 */
#define DEF_BIT_IS_CLR(val, mask) (((((val) & (mask)) == 0u) && ((mask) != 0u)) ? (DEF_YES) : (DEF_NO))

/**        数据val指定位是否至少有一位为1
 * @brief  Determine if any specified bit(s) in a value are set.
 * @param  val：Value to check for specified bit(s) set.
 * @param  mask：Mask of bits to check if set.
 * @retval DEF_YES, if ANY specified bit(s) are     set in value.
           DEF_NO,  if ALL specified bit(s) are NOT set in value.
 */
#define DEF_BIT_IS_SET_ANY(val, mask) ((((val) & (mask)) == 0u) ? (DEF_NO) : (DEF_YES))

/**        数据val指定位是否至少有一位为0
 * @brief  Determine if any specified bit(s) in a value are clear.
 * @param  val：Value to check for specified bit(s) clear.
 * @param  mask：Mask of bits to check if clear.
 * @retval DEF_YES, if ANY specified bit(s) are     clear in value.
           DEF_NO,  if ALL specified bit(s) are NOT clear in value.
 */
#define DEF_BIT_IS_CLR_ANY(val, mask) ((((val) & (mask)) == (mask)) ? (DEF_NO) : (DEF_YES))

/**        检查数据val是否大于等于数据val_min
 * @brief  Validate a value as greater than or equal to a specified minimum value.
 * @param  val：Value to validate.
 * @param  val_min：Minimum value to test.
 * @retval DEF_OK,   Value is greater than or equal to minimum value.
           DEF_FAIL, otherwise.
 */
#define DEF_CHK_VAL_MIN(val, val_min) (((!(((val) >= 1) && ((val_min) < 1))) && \
                                        ((((val_min) >= 1) && ((val) < 1)) ||   \
                                         ((val) < (val_min))))                  \
                                           ? DEF_FAIL                           \
                                           : DEF_OK)

/**        检查数据val是否小于等于数据val_max
 * @brief  Validate a value as less than or equal to a specified maximum value.
 * @param  val：Value to validate.
 * @param  val_max：Maximum value to test.
 * @retval DEF_OK,   Value is less than or equal to maximum value.
           DEF_FAIL, otherwise.
 */
#define DEF_CHK_VAL_MAX(val, val_max) (((!(((val_max) >= 1) && ((val) < 1))) && \
                                        ((((val) >= 1) && ((val_max) < 1)) ||   \
                                         ((val) > (val_max))))                  \
                                           ? DEF_FAIL                           \
                                           : DEF_OK)

/**        检查数据val是否大于等于数据val_min且小于等于数据val_max
 * @brief  Validate a value as greater than or equal to a specified minimum value & less than or
           equal to a specified maximum value.
 * @param  val：Value to validate.
 * @param  val_min：Minimum value to test.
 * @param  val_max：Maximum value to test.
 * @retval DEF_OK,    Value is greater than or equal to minimum value AND
                                less    than or equal to maximum value.
           DEF_FAIL,  otherwise.
 */
#define DEF_CHK_VAL(val, val_min, val_max) (((DEF_CHK_VAL_MIN((val), (val_min)) == DEF_FAIL) || \
                                             (DEF_CHK_VAL_MAX((val), (val_max)) == DEF_FAIL))   \
                                                ? DEF_FAIL                                      \
                                                : DEF_OK)

/**        数据a和数据b取最小值
 * @brief  Determine the minimum of two values.
 * @param  a：First  value.
 * @param  b：Second value.
 * @retval Minimum of the two values.
 */
#define DEF_MIN(a, b) (((a) < (b)) ? (a) : (b))

/**        数据a和数据b取最大值
 * @brief  Determine the maximum of two values.
 * @param  a：First  value.
 * @param  b：Second value.
 * @retval Maximum of the two values.
 */
#define DEF_MAX(a, b) (((a) > (b)) ? (a) : (b))

/**        数据a取绝对值
 * @brief  Determine the absolute value of a value.
 * @param  a：Value to calculate absolute value.
 * @retval Absolute value of the value.
 */
#define DEF_ABS(a) (((a) < 0) ? (-(a)) : (a))

/* Exported functions --------------------------------------------------------*/

/* End of define module include ----------------------------------------------*/
#endif
