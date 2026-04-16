/*
 * srm32f446xx_i2c_driver.h
 *
 *  Created on: Mar 6, 2026
 *      Author: Abhishek Patil
 */

#ifndef INC_STM32F446XX_I2C_DRIVER_H_
#define INC_STM32F446XX_I2C_DRIVER_H_

#include "stm32f446xx.h"

/*
 *   Configuration strcuture for I2Cx peripheral
 */
 typedef struct
 {

	 /************************************************************
	  * @var        I2C_SCL_Speed
	  *
	  * @brief      Specifies the speed of the I2C serial clock (SCL).
	  *
	  * @details    This parameter determines the communication speed
	  *             of the I2C bus. The driver uses this value to
	  *             calculate the CCR (Clock Control Register) which
	  *             generates the SCL signal.
	  *
	  * @possible_values
	  *             I2C_SCL_SPEED_SM  - Standard mode (100 kHz)
	  *             I2C_SCL_SPEED_FM  - Fast mode (400 kHz)
	  *
	  * @note       The SCL speed must be configured before enabling
	  *             the I2C peripheral.
	  ************************************************************/
	 uint32_t I2C_SCL_Speed;



	 /************************************************************
	  * @var        I2C_DeviceAddress
	  *
	  * @brief      Specifies the own address of the I2C device.
	  *
	  * @details    This address is used when the microcontroller
	  *             operates as an I2C slave. The value is programmed
	  *             into the OAR1 (Own Address Register 1).
	  *
	  *             When a master device sends this address on the
	  *             I2C bus, the hardware detects an address match
	  *             and communication with this device begins.
	  *
	  * @note       The address is automatically shifted and placed
	  *             in the correct bit position of the OAR1 register
	  *             by the driver during initialization.
	  ************************************************************/


	 uint8_t I2C_DeviceAddress;



	 /************************************************************
	  * @var        I2C_AckControl
	  *
	  * @brief      Controls the acknowledgment feature of the I2C
	  *             peripheral.
	  *
	  * @details    Acknowledgment (ACK) is used to confirm successful
	  *             reception of data during I2C communication.
	  *
	  *             If ACK is enabled, the receiver sends an ACK signal
	  *             after receiving each byte.
	  *
	  * @possible_values
	  *             I2C_ACK_ENABLE   - Enables acknowledgment
	  *             I2C_ACK_DISABLE  - Disables acknowledgment
	  *
	  * @note       ACK must be enabled when receiving multiple bytes
	  *             of data.
	  ************************************************************/


	 uint8_t I2C_AckControl;





	 /************************************************************
	  * @var        I2C_FMDutyCycle
	  *
	  * @brief      Configures the duty cycle of the SCL clock in
	  *             fast mode operation.
	  *
	  * @details    When the I2C peripheral operates in fast mode
	  *             (400 kHz), this parameter determines the ratio
	  *             of the SCL high period to the SCL low period.
	  *
	  * @possible_values
	  *             I2C_FM_DUTY_2        - Duty cycle ratio = 2
	  *             I2C_FM_DUTY_16_9     - Duty cycle ratio = 16/9
	  *
	  * @note       This parameter is used only when the I2C speed
	  *             is configured for fast mode.
	  ************************************************************/

	 uint8_t I2C_FMDutyCycle;



 }I2C_Config_t;

 /*
  * Handle strcuture for i2cx peripheral
  */
  typedef struct
  {
	  I2C_RegDef_t *pI2Cx;
	  I2C_Config_t I2C_Config;
	  uint8_t *pTxBuffer;     // to store the app. Tx buffer address >
	  uint8_t *pRxBuffer;      // to store the app.Rx buffer address
	  uint32_t  TxLen;        // to store Tx Len
	  uint32_t  RxLen;        // to store RxLen
	  uint8_t   TxRRxState;   //TO store the communication state
	  uint8_t   DevAddr;      // to store the slave/device address
	  uint32_t  RxSize;       //to store Rx size
 	  uint8_t    sr;          // to store repeated start value
  }I2C_Handle_t;


  /*
   * I2C_SCLSpeed
   */

#define I2C_SCL_SPEED_SM   100000
#define I2C_SCL_SPEED_FM4K   400000
#define I2C_SCL_SPEED_FM2K   400000

/*
 * I2C_AckControl
 */

#define I2C_ACK_ENABLE      1
#define I2C_ACK_DISABLE     0

/*
 * @I2C_DUTYCYCLE
 */

#define I2C_FM_DUTY_2        0
#define I2C_FM_DUTY_16_9     1


  //I2C RELATED STATUS FLAG

#define I2C_FLAG_TXE         ( 1 << I2C_SR1_TxE)
#define I2C_FLAG_RXNE        ( 1 << I2C_SR1_RxNE)
#define I2C_FLAG_SB          (1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR        (1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF         (1 << I2C_SR1_BTF)
#define I2C_FLAG_STOPF       (1 << I2C_SR1_STOPF)
#define I2C_FLAG_BERR        (1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO        (1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF          (1 << I2C_SR1_AF)
#define I2C_FLAG_OVR         (1 << I2C_SR1_OVR)
#define I2C_FLAG_PECERR      (1 << I2C_SR1_PECRR)
#define I2C_FLAG_TIMEOUT     (1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_SMBALERT    (1 << I2C_SR1_SMBALERT)


#define I2C_DISABLE_SR    RESET
#define I2C_ENABLE_SR      SET

  // I2C application status

#define I2C_READY        0
#define I2C_BUSY_IN_RX   1
#define I2C_BUSY_IN_TX   2

/*
 * I2C Application event macros
 */
#define  I2C_EV_TX_CMPLT   0
#define  I2C_EV_RX_CMPLT   1
#define I2C_EV_STOP        2
#define I2C_ERROR_BERR     3
#define I2C_ERROR_ARLO     4
#define I2C_ERROR_AF       5
#define I2C_ERROR_OVR      6
#define I2C_ERROR_TIMEOUT  7
#define I2C_EV_DATA_REQ    8
#define I2C_EV_DATA_RCV    9



  //@
  //*****************API SUPPORTED BY THIS DRIVER*************
  //        FOR MORE INFORMATION ABOUT THE API CHEAK THE FUNCTION DEFINATION
  //*************************************************************


  //PERIPHERAL CLOCK SETUP

  void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);



  //INIT AND DEINIT


  void I2C_Init(I2C_Handle_t *pI2CHandle);
  void I2C_DeInit(I2C_RegDef_t *pI2Cx);


  //DATA SEND AND RECEIVE

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer,uint8_t Len,uint8_t SlaveAddr,uint8_t sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer,uint8_t Len,uint8_t SlaveAddr,uint8_t sr);


uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer,uint8_t Len,uint8_t SlaveAddr,uint8_t sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer,uint8_t Len,uint8_t SlaveAddr,uint8_t sr);

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);

void I2C_SlaveSendData(I2C_RegDef_t *pI2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);

  //IRQ CONFIGURATION AND ISR HANDLING

  void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
  void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
  void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
  void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);


  void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
  void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);




  //I2C peripheral control api

  void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);
  uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
  void I2C_ManageAcking(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);
  void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
  void I2C_SlaveEnbleDisbleCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);



  //Application callback
  void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);




#endif /* INC_STM32F446XX_I2C_DRIVER_H_ */
