/**
* MODIFIED BY CHARLES ZALOOM - 8/3/18
* IMPLEMENTATION OF EMBEDDEDML IN LEARNING DEVICE ORIENTATION
**/

/**
******************************************************************************
* @file    DataLog/Src/main.c
* @author  Central Labs
* @version V1.1.1
* @date    06-Dec-2016
* @brief   Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include <string.h> /* strlen */
#include <stdio.h>  /* sprintf */
#include <math.h>   /* trunc */
#include "embeddedML.h"
#include "main.h"

#include "datalog_application.h"
#include "usbd_cdc_interface.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Data acquisition period [ms] */
#define DATA_PERIOD_MS (100)
//#define NOT_DEBUGGING

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* SendOverUSB = 0  --> Save sensors data on SDCard (enable with double click) */
/* SendOverUSB = 1  --> Send sensors data via USB */
uint8_t SendOverUSB = 1;

USBD_HandleTypeDef  USBD_Device;
static volatile uint8_t MEMSInterrupt = 0;
static volatile uint8_t acquire_data_enable_request  = 1;
static volatile uint8_t acquire_data_disable_request = 0;
static volatile uint8_t no_H_HTS221 = 0;
static volatile uint8_t no_T_HTS221 = 0;
static volatile uint8_t no_GG = 0;

static RTC_HandleTypeDef RtcHandle;
static void *LSM6DSM_X_0_handle = NULL;
static void *LSM6DSM_G_0_handle = NULL;
static void *LSM303AGR_X_0_handle = NULL;
static void *LSM303AGR_M_0_handle = NULL;
static void *LPS22HB_P_0_handle = NULL;
static void *LPS22HB_T_0_handle = NULL; 
static void *HTS221_H_0_handle = NULL; 
static void *HTS221_T_0_handle = NULL;
static void *GG_handle = NULL;

/* Private function prototypes -----------------------------------------------*/

static void Error_Handler( void );
static void RTC_Config( void );
static void RTC_TimeStampConfig( void );
static void initializeAllSensors( void );


/* Private functions ---------------------------------------------------------*/

static volatile uint8_t hasTrained = 0;
unsigned int training_cycles = 500;

void getAccel(void *handle, int *xyz){
  uint8_t id;
  SensorAxes_t acceleration;
  uint8_t status;

  BSP_ACCELERO_Get_Instance( handle, &id );

  BSP_ACCELERO_IsInitialized( handle, &status );

  if ( status == 1 )
  {
    if ( BSP_ACCELERO_Get_Axes( handle, &acceleration ) == COMPONENT_ERROR )
    {
      acceleration.AXIS_X = 0;
      acceleration.AXIS_Y = 0;
      acceleration.AXIS_Z = 0;
    }

    xyz[0] = (int)acceleration.AXIS_X;
    xyz[1] = (int)acceleration.AXIS_Y;
    xyz[2] = (int)acceleration.AXIS_Z;
  }
}

void TrainOrientation(void *handle, ANN *net){

  static char dataOut[256];
  char newLine[] = "\r\n";
  uint8_t who_am_i;
  float odr;
  float fullScale;\
  uint8_t id;
  SensorAxes_t acceleration;
  uint8_t status;
  int32_t d1, d2;
  float training_data[6][3];

  BSP_ACCELERO_Get_Instance( handle, &id );

  BSP_ACCELERO_IsInitialized( handle, &status );

  if ( status == 1 ){
    if ( BSP_ACCELERO_Get_Axes( handle, &acceleration ) == COMPONENT_ERROR )
    {
      acceleration.AXIS_X = 0;
      acceleration.AXIS_Y = 0;
      acceleration.AXIS_Z = 0;
    }

    int i,j;
    for(i = 0; i < 6; i++){
        char msg1[256];
        switch(i){
          case 0:
          sprintf(msg1, "\r\nAlign Sensor: Forward\r\nHold..");
          break;
          case 1:
          sprintf(msg1, "\r\nAlign Sensor: Backward\r\nHold..");
          break;
          case 2:
          sprintf(msg1, "\r\nAlign Sensor: Right\r\nHold..");
          break;
          case 3:
          sprintf(msg1, "\r\nAlign Sensor: Left\r\nHold..");
          break;
          case 4:
          sprintf(msg1, "\r\nAlign Sensor: Up\r\nHold..");
          break;
          case 5:
          sprintf(msg1, "\r\nAlign Sensor: Down\r\nHold..");
          break;
          default:
          break;
        }
        CDC_Fill_Buffer(( uint8_t * )msg1, strlen(msg1));
        HAL_Delay(3000);

        //NEED UPDATE
        int ttt[3];
        getAccel(handle, ttt);

        float XYZ[3];
        XYZ[0] = (float)ttt[0];
        XYZ[1] = (float)ttt[1];
        XYZ[2] = (float)ttt[2];

        float xyz[3];
        weak_softmax(XYZ, xyz);
        //stable_softmax(XYZ, xyz);

        char msgo[64];
        sprintf(msgo, "\r\nOUTPUT: %d/100 %d/100 %d/100", (int)(xyz[0]*100.0),(int)(xyz[1]*100.0),(int)(xyz[2]*100.0));
        CDC_Fill_Buffer(( uint8_t * )msgo, strlen(msgo));
        HAL_Delay(10);

        training_data[i][0] = xyz[0];
        training_data[i][1] = xyz[1];
        training_data[i][2] = xyz[2];


        HAL_Delay(10);
        char msg2[32] = "DONE";
        CDC_Fill_Buffer(( uint8_t * )msg2, 32);
        HAL_Delay(100);
      }

      float _FORWARD[6] = { 1.0 , -1.0 , 0.0 , 0.0 , 0.0 , 0.0 };
      float _BACKWARD[6] = { -1.0 , 1.0 , 0.0 , 0.0 , 0.0 , 0.0 };
      float _RIGHT[6] = { 0.0 , 0.0 , 1.0 , -1.0 , 0.0 , 0.0 };
      float _LEFT[6] = { 0.0 , 0.0 , -1.0 , 1.0 , 0.0 , 0.0 };
      float _UP[6] = { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , -1.0 };
      float _DOWN[6] = { 0.0 , 0.0 , 0.0 , 0.0 , -1.0 , 1.0 };

      i = 0;
      while(i < training_cycles){
       for( j = 0; j < 6; j++){
         if(i % 500 == 0){
           char print_train_time[32];
           sprintf(print_train_time, "%d", i);
           CDC_Fill_Buffer(( uint8_t * )print_train_time, strlen(print_train_time));
           HAL_Delay(50);
         }
         switch(j){
           case 0:
             train_ann(net, training_data[j], _FORWARD);
             break;
           case 1:
             train_ann(net, training_data[j], _BACKWARD);
             break;
           case 2:
             train_ann(net, training_data[j], _RIGHT);
             break;
           case 3:
             train_ann(net, training_data[j], _LEFT);
             break;
           case 4:
             train_ann(net, training_data[j], _UP);
             break;
           case 5:
             train_ann(net, training_data[j], _DOWN);
             break;
           default:
             break;
        }
        i++;
        HAL_Delay(5);
       }
      }

    if(SendOverUSB) /* Write data on the USB */
    {
      sprintf( dataOut, "\n\rAX: %d, AY: %d, AZ: %d", (int)acceleration.AXIS_X, (int)acceleration.AXIS_Y, (int)acceleration.AXIS_Z );
      CDC_Fill_Buffer(( uint8_t * )dataOut, strlen( dataOut ));
    }
  }
}

void Accel_Sensor_Handler( void *handle , ANN *net)
{
  static char dataOut[256];
  char newLine[] = "\r\n";
  uint8_t who_am_i;
  float odr;
  float fullScale;
  uint8_t id;
  SensorAxes_t acceleration;
  uint8_t status;
  int32_t d1, d2;

  BSP_ACCELERO_Get_Instance( handle, &id );

  BSP_ACCELERO_IsInitialized( handle, &status );

  if ( status == 1 )
  {
    if ( BSP_ACCELERO_Get_Axes( handle, &acceleration ) == COMPONENT_ERROR )
    {
      acceleration.AXIS_X = 0;
      acceleration.AXIS_Y = 0;
      acceleration.AXIS_Z = 0;
    }

    if(SendOverUSB) /* Write data on the USB */
    {
      sprintf( dataOut, "\n\rAX: %d, AY: %d, AZ: %d", (int)acceleration.AXIS_X, (int)acceleration.AXIS_Y, (int)acceleration.AXIS_Z );
      CDC_Fill_Buffer(( uint8_t * )dataOut, strlen( dataOut ));
    }

    float xyz[3];
    xyz[0] = (float)acceleration.AXIS_X;
    xyz[1] = (float)acceleration.AXIS_Y;
    xyz[2] = (float)acceleration.AXIS_Z;

    float XYZ[3];
    weak_softmax(xyz, XYZ);
    //stable_softmax(xyz,XYZ);
    run_ann(net, XYZ);

    char msg3[32];
    float point = 0.0;
    int i;
    int loc = -1;

    char msgo[64];
    sprintf(msgo, "\r\nOUTPUT: %d %d %d %d %d %d", (int)net->output[0],(int)net->output[1],(int)net->output[2],(int)net->output[3],(int)net->output[4],(int)net->output[5]);
    CDC_Fill_Buffer(( uint8_t * )msgo, strlen(msgo));
    HAL_Delay(10);

    for (i = 0; i < net->topology[net->n_layers - 1]; i++) {
        if (net->output[i] > point && net->output[i] > 0.1) {
          point = net->output[i];
          loc = i;
        }
      }
      switch (loc) {
        case 0:
          sprintf(msg3, "\n\rFORWARD");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        case 1:
          sprintf(msg3, "\n\rBACKWARD");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        case 2:
          sprintf(msg3, "\n\rRIGHT");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        case 3:
          sprintf(msg3, "\n\rLEFT");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        case 4:
          sprintf(msg3, "\n\rUP");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        case 5:
          sprintf(msg3, "\n\rDOWN");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        case -1:
          sprintf(msg3, "\n\rERROR");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
        default:
          sprintf(msg3, "\n\rNULL");
          CDC_Fill_Buffer(( uint8_t * )msg3, strlen(msg3));
          break;
      }
  HAL_Delay(10);
  }
}

void stable_softmax(float *x, float *y){
  int size = 3;
  float multiplier = 1.0;

  int i;

  //Had to redesign softmax as the SensorTile has trouble storing very small floating point numbers.
  //Using a power of 2 reduces accuracy but allows for guranteed results.
  y[0] = (x[0]*x[0]*multiplier) / ((x[0]*x[0]*multiplier) + (x[1]*x[1]*multiplier) + (x[2]*x[2]*multiplier));
  y[1] = (x[1]*x[1]*multiplier) / ((x[0]*x[0]*multiplier) + (x[1]*x[1]*multiplier) + (x[2]*x[2]*multiplier));
  y[2] = (x[2]*x[2]*multiplier) / ((x[0]*x[0]*multiplier) + (x[1]*x[1]*multiplier) + (x[2]*x[2]*multiplier));

  for(i = 0; i < size; i++){
    if(x[i] < 0.0) y[i] = y[i] * -1.0;
  }
}

void test_softmax(){
  float x[3] = { 2.0, 4.0, 1000.0 };
  float y[3] = { 0.0, 0.0, 0.0 };

  weak_softmax(x,y);
  //stable_softmax(x,y);

  char msgo[64];
  sprintf(msgo, "\r\nOUTPUT: %d %d %d", (int)(y[0]+0.5),(int)(y[1]+0.5),(int)(y[2]+0.5));
  CDC_Fill_Buffer(( uint8_t * )msgo, strlen(msgo));
  HAL_Delay(10);
}
 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
  uint32_t msTick, msTickPrev = 0;
  uint8_t doubleTap = 0;
  
  /* STM32L4xx HAL library initialization:
  - Configure the Flash prefetch, instruction and Data caches
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  
  if(SendOverUSB)
  {
    /* Initialize LED */
    BSP_LED_Init(LED1);
    BSP_LED_On(LED1);
  }
#ifdef NOT_DEBUGGING     
  else
  {
    /* Initialize LEDSWD: Cannot be used during debug because it overrides SWDCLK pin configuration */
    BSP_LED_Init(LEDSWD);
    BSP_LED_Off(LEDSWD);
  }
#endif
  
  /* Initialize RTC */
  RTC_Config();
  RTC_TimeStampConfig();
  
  /* enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();
  
  if(SendOverUSB) /* Configure the USB */
  {
    /*** USB CDC Configuration ***/
    /* Init Device Library */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    /* Add Supported Class */
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    /* Add Interface callbacks for AUDIO and CDC Class */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    /* Start Device Process */
    USBD_Start(&USBD_Device);
  }
  else /* Configure the SDCard */
  {
    DATALOG_SD_Init();
  }
  HAL_Delay(200);
  
  /* Configure and disable all the Chip Select pins */
  Sensor_IO_SPI_CS_Init_All();
  
  /* Initialize and Enable the available sensors */
  initializeAllSensors();
  enableAllSensors();
  
  //---EMBEDDED ANN---
  float weights[81] = {0.680700,0.324900,0.607300,0.365800,0.693000,0.527200,0.754400,0.287800,0.592300,0.570900,0.644000,0.416500,0.249200,0.704200,0.598700,0.250300,0.632700,0.372900,0.684000,0.661200,0.230300,0.516900,0.770900,0.315700,0.756000,0.293300,0.509900,0.627800,0.781600,0.733500,0.509700,0.382600,0.551200,0.326700,0.781000,0.563300,0.297900,0.714900,0.257900,0.682100,0.596700,0.467200,0.339300,0.533600,0.548500,0.374500,0.722800,0.209100,0.619400,0.635700,0.300100,0.715300,0.670800,0.794400,0.766800,0.349000,0.412400,0.619600,0.353000,0.690300,0.772200,0.666600,0.254900,0.402400,0.780100,0.285300,0.697700,0.540800,0.222800,0.693300,0.229800,0.698100,0.463500,0.201300,0.786500,0.581400,0.706300,0.653600,0.542500,0.766900,0.411500};
  float dedw[81];
  float bias[15];
  unsigned int network_topology[3] = {3,9,6};
  float run_error[6];

  ANN net;
  net.weights = weights;
  net.dedw = dedw;
  net.bias = bias;
  net.topology = network_topology;
  net.n_layers = 3;
  net.n_weights = 81;
  net.n_bias = 15;
  net.output = run_error;

  //OPTIONS
  net.eta = 0.13;     //Learning Rate
  net.alpha = 0.25;   //Momentum Coefficient
  net.output_activation_function = &relu2;
  net.hidden_activation_function = &relu2;

  init_ann(&net);
  float x[3];
  float y[6];
  //---------------------

  //test_softmax();

  while (1)
  {
    /* Get sysTick value and check if it's time to execute the task */
    msTick = HAL_GetTick();
    if(msTick % DATA_PERIOD_MS == 0 && msTickPrev != msTick)
    {
      msTickPrev = msTick;

      if(SendOverUSB)
      {
        BSP_LED_On(LED1);
      }

      //RTC_Handler( &RtcHandle );
      
      if(hasTrained) Accel_Sensor_Handler( LSM6DSM_X_0_handle , &net );
      
      if(SendOverUSB)
      {
        BSP_LED_Off(LED1);
      }

    }

    /* Check LSM6DSM Double Tap Event  */
    if(!hasTrained)
    {
      BSP_ACCELERO_Get_Double_Tap_Detection_Status_Ext(LSM6DSM_X_0_handle,&doubleTap);
      if(doubleTap) { /* Double Tap event */
        TrainOrientation( LSM6DSM_X_0_handle, &net);
        hasTrained = 1;
      }
    }
    
    /* Go to Sleep */
    __WFI();
  }
}

/**
* @brief  Initialize all sensors
* @param  None
* @retval None
*/
static void initializeAllSensors( void )
{
  if (BSP_ACCELERO_Init( LSM6DSM_X_0, &LSM6DSM_X_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_GYRO_Init( LSM6DSM_G_0, &LSM6DSM_G_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_ACCELERO_Init( LSM303AGR_X_0, &LSM303AGR_X_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_MAGNETO_Init( LSM303AGR_M_0, &LSM303AGR_M_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_PRESSURE_Init( LPS22HB_P_0, &LPS22HB_P_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_TEMPERATURE_Init( LPS22HB_T_0, &LPS22HB_T_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if(BSP_TEMPERATURE_Init( HTS221_T_0, &HTS221_T_0_handle ) == COMPONENT_ERROR)
  {
    no_T_HTS221 = 1;
  }
  
  if(BSP_HUMIDITY_Init( HTS221_H_0, &HTS221_H_0_handle ) == COMPONENT_ERROR)
  {
    no_H_HTS221 = 1;
  }
  
  /* Inialize the Gas Gauge if the battery is present */
  if(BSP_GG_Init(&GG_handle) == COMPONENT_ERROR)
  {
    no_GG=1;
  }
  
  //if(!SendOverUSB)
  //{
    /* Enable HW Double Tap detection */
    BSP_ACCELERO_Enable_Double_Tap_Detection_Ext(LSM6DSM_X_0_handle);
    BSP_ACCELERO_Set_Tap_Threshold_Ext(LSM6DSM_X_0_handle, LSM6DSM_TAP_THRESHOLD_MID);
  //}
  
  
}

/**
* @brief  Enable all sensors
* @param  None
* @retval None
*/
void enableAllSensors( void )
{
  BSP_ACCELERO_Sensor_Enable( LSM6DSM_X_0_handle );
  BSP_GYRO_Sensor_Enable( LSM6DSM_G_0_handle );
  BSP_ACCELERO_Sensor_Enable( LSM303AGR_X_0_handle );
  BSP_MAGNETO_Sensor_Enable( LSM303AGR_M_0_handle );
  BSP_PRESSURE_Sensor_Enable( LPS22HB_P_0_handle );
  BSP_TEMPERATURE_Sensor_Enable( LPS22HB_T_0_handle );
  if(!no_T_HTS221)
  {
    BSP_TEMPERATURE_Sensor_Enable( HTS221_T_0_handle );
    BSP_HUMIDITY_Sensor_Enable( HTS221_H_0_handle );
  }
  
}



/**
* @brief  Disable all sensors
* @param  None
* @retval None
*/
void disableAllSensors( void )
{
  BSP_ACCELERO_Sensor_Disable( LSM6DSM_X_0_handle );
  BSP_ACCELERO_Sensor_Disable( LSM303AGR_X_0_handle );
  BSP_GYRO_Sensor_Disable( LSM6DSM_G_0_handle );
  BSP_MAGNETO_Sensor_Disable( LSM303AGR_M_0_handle );
  BSP_HUMIDITY_Sensor_Disable( HTS221_H_0_handle );
  BSP_TEMPERATURE_Sensor_Disable( HTS221_T_0_handle );
  BSP_TEMPERATURE_Sensor_Disable( LPS22HB_T_0_handle );
  BSP_PRESSURE_Sensor_Disable( LPS22HB_P_0_handle );
}



/**
* @brief  Configures the RTC
* @param  None
* @retval None
*/
static void RTC_Config( void )
{
  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;
  
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 12
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if ( HAL_RTC_Init( &RtcHandle ) != HAL_OK )
  {
    
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  Configures the current time and date
* @param  None
* @retval None
*/
static void RTC_TimeStampConfig( void )
{
  
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  
  /*##-3- Configure the Date using BCD format ################################*/
  /* Set Date: Monday January 1st 2000 */
  sdatestructure.Year    = 0x00;
  sdatestructure.Month   = RTC_MONTH_JANUARY;
  sdatestructure.Date    = 0x01;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if ( HAL_RTC_SetDate( &RtcHandle, &sdatestructure, FORMAT_BCD ) != HAL_OK )
  {
    
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-4- Configure the Time using BCD format#################################*/
  /* Set Time: 00:00:00 */
  stimestructure.Hours          = 0x00;
  stimestructure.Minutes        = 0x00;
  stimestructure.Seconds        = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if ( HAL_RTC_SetTime( &RtcHandle, &stimestructure, FORMAT_BCD ) != HAL_OK )
  {   
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  Configures the current time and date
* @param  hh the hour value to be set
* @param  mm the minute value to be set
* @param  ss the second value to be set
* @retval None
*/
void RTC_TimeRegulate( uint8_t hh, uint8_t mm, uint8_t ss )
{
  
  RTC_TimeTypeDef stimestructure;
  
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.Hours          = hh;
  stimestructure.Minutes        = mm;
  stimestructure.Seconds        = ss;
  stimestructure.SubSeconds     = 0;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if ( HAL_RTC_SetTime( &RtcHandle, &stimestructure, FORMAT_BIN ) != HAL_OK )
  {
    /* Initialization Error */
    Error_Handler();
  }
}



/**
* @brief  EXTI line detection callbacks
* @param  GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  MEMSInterrupt=1;
}



/**
* @brief  This function is executed in case of error occurrence
* @param  None
* @retval None
*/
static void Error_Handler( void )
{
  
  while (1)
  {}
}



#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed( uint8_t *file, uint32_t line )
{
  
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  while (1)
  {}
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
