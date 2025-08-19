/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DIR_M1 GPIO_PA1
#define DIR_M2 GPIO_PA4
#define DIR_M3 GPIO_PB0
#define DIR_M4 GPIO_PB1
#define DIR_M5 GPIO_PA7
#define LED_PIN GPIO_PIN_13
#define UART_BUFFER_SIZE 16


typedef enum{
	M1,
	M2,
	M3,
	M4,
	M5
} Motor;


typedef enum{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H
}Prescription;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Move_Motor(Motor motor, int16_t step);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void First_Pick(Prescription pres);
void Second_Pick(int16_t val1, int16_t val2, int16_t val3);
void Calibrate(Motor motor);
void Delay_ms(uint16_t delay_ms);

uint8_t rx_buffer[UART_BUFFER_SIZE]={0};
uint8_t tx_buffer[UART_BUFFER_SIZE]={0};

uint8_t calibration_done = 0;

volatile int16_t val1=0;
volatile int16_t val2=0;
volatile int16_t val3=0;

volatile uint8_t second_pick_flag = 0;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint16_t pwm_count_M1 = 0; // Tracks steps for Motor 1
volatile uint16_t pwm_target_M1 = 0;

volatile uint16_t pwm_count_M2 = 0;
volatile uint16_t pwm_target_M2 = 0;

volatile uint16_t pwm_count_M3 = 0;
volatile uint16_t pwm_target_M3 = 0;

volatile uint16_t pwm_count_M4 = 0;
volatile uint16_t pwm_target_M4 = 0;

volatile uint16_t pwm_count_M5 = 0;
volatile uint16_t pwm_target_M5 = 0;

volatile uint8_t M1_movedone = 0;
volatile uint8_t M2_movedone = 0;
volatile uint8_t M3_movedone = 0;
volatile uint8_t M4_movedone = 0;
volatile uint8_t M5_movedone = 0;

volatile uint32_t timer_delay = 0;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  Calibrate(M3);
  Calibrate(M1);
  Calibrate(M2);
  Calibrate(M5);
  Calibrate(M4);

  Delay_ms(1000);

  HAL_UART_Receive_IT(&huart1,rx_buffer,6);


//First_Pick();
//  Second_Pick(val1,val2,val3);
//  Move_Motor(M4,920,CCW); //CW:Pill 1->200, pill 2->380, pill 3->560, pill 4->740, pill 5->920, pill 6->1100, pill 7->1260, pill8->1450


  	  	  	Move_Motor(M1,1300); //MOVE M1
        	    while (!M1_movedone) {
        	    }

        	Move_Motor(M2,400);
        	    while (!M2_movedone) {
       	    }

        	Move_Motor(M4, -590); //MOVE BASKET //CW:Pill 1->-200, pill 2->-420, pill 3->-590, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
        	    while (!M4_movedone) {
        	    }

        	Delay_ms(500);

        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH
        	Delay_ms(100);
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PUMP Open
        	Delay_ms(100);
        	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
        	Delay_ms(100);


        	Move_Motor(M3,-2550);  //M3 DOWN
        		  while (!M3_movedone) {
                 }
        	Delay_ms(1000);

        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PUMP reset
        	Delay_ms(100);
        	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // PUMP STOP
        	Delay_ms(200);

            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            Delay_ms(100);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
            Delay_ms(200);

        	Move_Motor(M3,2100); //M3 UP
        	while (!M3_movedone) {
       	    }

            Delay_ms(2000);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            Delay_ms(100);
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        	Delay_ms(300);
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
        	Delay_ms(300);
        	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP STOP



        	Calibrate(M3);
        	Calibrate(M5);
        	Calibrate(M1);
        	Calibrate(M2);
        	Calibrate(M4);

        	Delay_ms(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1){


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      if(second_pick_flag)
      {
    	  second_pick_flag = 0;
          Second_Pick(val1, val2, val3);
         }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

     val1 = 0; // Initialize to 0
     val2 = 0; // Initialize to 0
     val3 = 0; // Initialize to 0

	//Prescription pres='\0';

    if (huart->Instance == USART1)
    {

        val1 = (int16_t)(rx_buffer[0] | (rx_buffer[1] << 8));
        val2 = (int16_t)(rx_buffer[2] | (rx_buffer[3] << 8));
        val3 = (int16_t)(rx_buffer[4] | (rx_buffer[5] << 8));

    	tx_buffer[0] = (uint8_t)(val1 & 0xFF);
    	tx_buffer[1] = (uint8_t)((val1 >> 8) & 0xFF);

    	tx_buffer[2] = (uint8_t)(val2 & 0xFF);
    	tx_buffer[3] = (uint8_t)((val2 >> 8) & 0xFF);

    	tx_buffer[4] = (uint8_t)(val3 & 0xFF);
    	tx_buffer[5] = (uint8_t)((val3 >> 8) & 0xFF);

       	HAL_UART_Transmit(&huart1,tx_buffer, 6, 100);

       	second_pick_flag = 1;


//      switch(command)
//      {
//        case C: //Calibration
//        	if (strncmp((char*)rx_buffer, "C",1) == 0)
//	          {
//        		Calibrate(M3);
//        		Calibrate(M1);
//		  	    Calibrate(M2);
//		  	    Calibrate(M4);
//		  	    Calibrate(M5);
//		  	  	HAL_UART_Transmit(&huart1,(int16_t*)"Calibration ok\n", 15, 100);
//	          }
//        	else
//	          {
//	        	HAL_UART_Transmit(&huart1,(int16_t*)" Wrong command\n", 15, 100);
//	          }
//	    break;
//
//        case F: //First pick
//        	if (strncmp((char*)rx_buffer, "F",1) == 0)
//        	{
//        		HAL_UART_Transmit(&huart1,(int16_t*)"1st pick start\n", 15, 100);
//        		First_Pick();
//        	}
//        	else
//        	{
//        		HAL_UART_Transmit(&huart1,(int16_t*)" Wrong command\n", 15, 100);
//        	}
//        break;
//
//        case S: //Second pick
//        	if (strncmp((char*)rx_buffer, "S",1) == 0)
//        	  {
//        		HAL_UART_Transmit(&huart1,(int16_t*)"2nd pick start\n", 15, 100);
//        		Second_Pick( );
//        	  }
//        	else
//        	  {
//        		HAL_UART_Transmit(&huart1,(int16_t*)" Wrong command\n", 15, 100);
//              }
//        break;
//
//        case E: //Emergency stop
//                	if (strncmp((char*)rx_buffer, "E",1) == 0)
//        	          {
//        		  	  	HAL_UART_Transmit(&huart1,(int16_t*)"STOP\n", 6, 100);
//        		  	  	HAL_TIM_Base_Stop_IT(&htim2);
//        		  	  	HAL_TIM_Base_Stop_IT(&htim3);
//        	          }
//        	          else
//        	          {
//          	            HAL_UART_Transmit(&huart1,(int16_t*)" Wrong command\n", 15, 100);
//        	          }
//        	            // Transmit the response
//
//
//
//        	            //Clear the memeory
//
//        	    break;
//
//        default:
//            HAL_UART_Transmit(&huart1,(int16_t*)"No command\n", 15, 100);
//            	return;

//      }
      	  memset(rx_buffer, 0, UART_BUFFER_SIZE);
      	  memset(tx_buffer, 0, UART_BUFFER_SIZE);


    }
        // Re-initiate UART receive for the next byte
          HAL_UART_Receive_IT(&huart1,rx_buffer,6);
}

void Calibrate(Motor motor)
{
    GPIO_PinState sensor_state;



    switch (motor)
    {
    case M1:
        // Unidirectional calibration for Motor 1
    	uint32_t start_time_M1 = HAL_GetTick();
    	calibration_done = 0;

        while (!calibration_done)
        {
            // Read the sensor state for M1 (e.g., PB12)
        	sensor_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
            if (sensor_state == GPIO_PIN_SET) // Sensor triggered (light blocked)
            {
            	HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_3); // Stop motor movement
            	calibration_done = 1;
                pwm_count_M1 = 0; // Reset step counter after reaching calibration point
            }
            else
            {
                // Move backward to the calibration point
            	  Move_Motor(M1,-200);//ARM_X



                // Timeout check

                if ((HAL_GetTick() - start_time_M1) > 10000)
                {
                    HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_3); // Stop the motor
                    HAL_UART_Transmit(&huart1,(uint8_t*)"M1 cali timeout\n",15, 100);

                }
           }
        }
        break;

    case M2:
        // Bidirectional calibration for Motor 2
    	uint32_t start_time_M2 = HAL_GetTick();
    	calibration_done = 0;
        uint8_t direction = 0; // 0 = backward, 1 = forward
        while (!calibration_done)
        {
            // Read the sensor state for M2 (e.g., PB13)
            sensor_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13); // Replace GPIO_PIN_13 with actual sensor pin

            if (sensor_state == GPIO_PIN_SET) // Sensor triggered (light blocked)
            {
            	HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4); // Stop motor movement
            	calibration_done = 1;
                pwm_count_M2 = 0; // Reset step counter after reaching calibration point
                break;
            }
            if (direction == 0) // backward
                       Move_Motor(M2, -200);
                   else // forward
                       Move_Motor(M2, 200);

                   // Wait for move to finish
            while (!M2_movedone) { /* Optionally add timeout here */ }
                   M2_movedone = 0; // reset for next move

                   uint32_t elapsed = HAL_GetTick() - start_time_M2;
                   if (elapsed > 10000) // Total timeout 10s
                   {
                       HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4);
                       HAL_UART_Transmit(&huart1, (uint8_t*)"M2 cali timeout\n", 15, 100);
                       break;
                   }
                   else if (elapsed > 5000 && direction == 0)
                   {
                       // Switch direction after 5s of moving backward with no sensor trigger
                       direction = 1;
                       start_time_M2 = HAL_GetTick(); // reset timer for new direction
                   }
               }
           break;

    case M3:
    	uint32_t start_time_M3 = HAL_GetTick();
        // Unidirectional calibration for Motor 3
    	 calibration_done = 0;
        while (!calibration_done)
        {
            sensor_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14); // Replace GPIO_PIN_14 with actual sensor pin

            if (sensor_state == GPIO_PIN_SET) // Sensor triggered (light blocked)
            {
            	HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1); // Stop motor movement
            	calibration_done = 1;
                pwm_count_M3 = 0; // Reset step counter after reaching calibration point

            }
            else
            {
                // Move backward to the calibration point
          	  Move_Motor(M3,200);//ARM_X


                // Timeout check
                if ((HAL_GetTick() - start_time_M3) > 10000)
                {
                    HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1); // Stop the motor
                    HAL_UART_Transmit(&huart1,(uint8_t*)"M3 cali timeout\n",15, 100);
                }
            }
        }
        break;

    case M4:
        	uint32_t start_time_M4 = HAL_GetTick();
            // Unidirectional calibration for Motor 3
        	 calibration_done = 0;
            while (!calibration_done)
            {
                sensor_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15); // Replace GPIO_PIN_14 with actual sensor pin

                if (sensor_state == GPIO_PIN_SET) // Sensor triggered (light blocked)
                {
                	HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2); // Stop motor movement
                	calibration_done = 1;
                    pwm_count_M4 = 0; // Reset step counter after reaching calibration point

                }
                else
                {
                    // Move backward to the calibration point
              	  Move_Motor(M4,200);//ARM_X

                    // Timeout check
                    if ((HAL_GetTick() - start_time_M4) > 10000)
                    {
                        HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2); // Stop the motor
                        HAL_UART_Transmit(&huart1,(uint8_t*)"M4 cali timeout\n",15, 100);
                    }
                }
            }
            break;
    case M5:
        	uint32_t start_time_M5 = HAL_GetTick();
        	uint8_t calibration_done = 0;
            // Unidirectional calibration for Motor 3
            while (!calibration_done)
            {
                sensor_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11); // Replace GPIO_PIN_14 with actual sensor pin

                if (sensor_state == GPIO_PIN_SET) // Sensor triggered (light blocked)
                {
                	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1); // Stop motor movement
                    calibration_done = 1;
                    pwm_count_M5 = 0; // Reset step counter after reaching calibration point

                }
                else
                {
                    // Move backward to the calibration point
                	Move_Motor(M5,200);


                    // Timeout check
                    if ((HAL_GetTick() - start_time_M5) > 10000)
                    {
                        HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1); // Stop the motor
                        HAL_UART_Transmit(&huart1,(uint8_t*)"M5 cali timeout\n",15, 100);
                    }
                }
            }
            break;
    // Add similar logic for M4, M5, M6 (unidirectional calibration)
    default:
    	HAL_UART_Transmit(&huart1,(uint8_t*)"Wrong cali cmd\n",15, 100);
        Error_Handler(); // If motor is invalid, return failure
    }

}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == htim2.Instance)
    {

        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
        	pwm_count_M1 ++;

        	if(pwm_count_M1 >= pwm_target_M1)
        	{
        		HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_3);
        		pwm_count_M1 = 0;
        		M1_movedone = 1;


        	}
             //Stop PWM generation for Channel 1
        }

        else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
        	pwm_count_M2 ++;

        	if(pwm_count_M2 >= pwm_target_M2)
        	{
            HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4);
            pwm_count_M2 = 0;
            M2_movedone = 1;

        	}
        }

        else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
        	pwm_count_M3 ++;

        	if(pwm_count_M3 >= pwm_target_M3)
        	{
            HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
            pwm_count_M3 = 0;
            M3_movedone = 1;


        	}
        }

        else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
        	pwm_count_M4 ++;

        	if(pwm_count_M4 >= pwm_target_M4)
        	{
            HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2);
            pwm_count_M4 = 0;
            M4_movedone = 1;

        	}
        }
    }

    if (htim->Instance == htim3.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {

            	pwm_count_M5 ++;

            	if(pwm_count_M5 >= pwm_target_M5)
            	{
            		HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
            		pwm_count_M5 = 0;
                    M5_movedone = 1;

            	}
        }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim == &htim4 ||timer_delay > 0) {
        	timer_delay--;
    }
 }

void Delay_ms(uint16_t ms) {
    timer_delay = ms;
    HAL_TIM_Base_Start_IT(&htim4);
    while(timer_delay > 0);
    HAL_TIM_Base_Stop_IT(&htim4);
}


void First_Pick(Prescription pres)
{
//	Calibrate(M1);
//	Calibrate(M2);
//	Calibrate(M3);
//	Calibrate(M4);
//	Calibrate(M5);
//	Delay_ms(100);

    M1_movedone = 0;
    M2_movedone = 0;
    M3_movedone = 0;
    M4_movedone = 0;
    M5_movedone = 0;

    switch(pres){

    case A:

	  	  	Move_Motor(M1,1400); //MOVE M1
    	    while (!M1_movedone) {
    	    }

    	Move_Motor(M2,400);
    	    while (!M2_movedone) {
   	    }

    	Move_Motor(M4, -240); //MOVE BASKET //CW:Pill 1->-240, pill 2->-420, pill 3->-590, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
    	    while (!M4_movedone) {
    	    }

    	Delay_ms(50);

    	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH
    	//Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PUMP Open
    	Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
    	Delay_ms(100);

//       	Move_Motor(M5, 90); //MOVE BASKET //CW:Pill 1->-240, pill 2->-420, pill 3->-560, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
//        	    while (!M5_movedone) {
//        	    }
//            	Delay_ms(100);
    	Move_Motor(M3,-2400);  //M3 DOWN
    		  while (!M3_movedone) {
             }
    	Delay_ms(1000);

    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PUMP reset
    	Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // PUMP STOP
    	Delay_ms(200);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        Delay_ms(100);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
        Delay_ms(200);
    	Move_Motor(M3,2100); //M3 UP
    	while (!M3_movedone) {
   	    }

        Delay_ms(2000);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP STOP
    	Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    	Delay_ms(100);

    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    	Delay_ms(100);

    	Calibrate(M3);
    	Calibrate(M1);
    	Calibrate(M2);
    	Calibrate(M4);
    	Calibrate(M5);
    	Delay_ms(500);

    break;

    case B:
	  	  	Move_Motor(M1,1300); //MOVE M1
    	    while (!M1_movedone) {
    	    }

    	Move_Motor(M2,400);
    	    while (!M2_movedone) {
   	    }

    	Move_Motor(M4, -420); //MOVE BASKET //CW:Pill 1->-200, pill 2->-420, pill 3->-560, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
    	    while (!M4_movedone) {
    	    }

    	Delay_ms(500);

    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH
    	Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PUMP Open
    	Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
    	Delay_ms(100);


    	Move_Motor(M3,-2550);  //M3 DOWN
    		  while (!M3_movedone) {
             }
    	Delay_ms(1000);

    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PUMP reset
    	Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // PUMP STOP
    	Delay_ms(200);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        Delay_ms(100);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
        Delay_ms(200);

    	Move_Motor(M3,2100); //M3 UP
    	while (!M3_movedone) {
   	    }

        Delay_ms(2000);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        Delay_ms(100);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    	Delay_ms(300);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    	Delay_ms(300);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP STOP



    	Calibrate(M3);
    	Calibrate(M5);
    	Calibrate(M1);
    	Calibrate(M2);
    	Calibrate(M4);

    	Delay_ms(1000);


    break;
    case C:
    			  Move_Motor(M1,1400); //MOVE M1
    	      	    while (!M1_movedone) {
    	      	    }

    	      	  Move_Motor(M2,400);
    	      	    while (!M2_movedone) {
    	     	    }

    	      	  Move_Motor(M4, -560); //MOVE BASKET //CW:Pill 1->-200, pill 2->-380, pill 3->-560, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
    	      	    while (!M4_movedone) {
    	      	    }

    	      	  Delay_ms(50);

    	      	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH
    	      	  //Delay_ms(100);
    	      	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PUMP Open
    	      	  Delay_ms(50);
    	          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
    	    	  Delay_ms(50);
    	          Move_Motor(M3,-2300);  //M3 DOWN
    	      		  while (!M3_movedone) {
    	               }
    	      		  Delay_ms(1000);

    	 		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PUMP reset
    	 		  Delay_ms(100);
    	 		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // PUMP STOP
    	 		  Delay_ms(50);

    	          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    	          Delay_ms(50);
    	          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
    	          Delay_ms(200);
    	          Move_Motor(M3,2100); //M3 UP
    	          while (!M3_movedone) {
    	     	    }

    	          Delay_ms(2200);
    	          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    	          Delay_ms(100);
    	          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP STOP
    	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    	          Delay_ms(50);
    	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);



    break;
    case D:
  	  Move_Motor(M1,1400); //MOVE M1
  	    while (!M1_movedone) {
  	    }

  	  Move_Motor(M2,400);
  	    while (!M2_movedone) {
  	    }

  	  Move_Motor(M4, -740); //MOVE BASKET //CW:Pill 1->-200, pill 2->-380, pill 3->-560, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
  	    while (!M4_movedone) {
  	    }

  	  Delay_ms(50);

  	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH
  	  //Delay_ms(100);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PUMP Open
  	  Delay_ms(50);
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
  	  Delay_ms(50);
  	  Move_Motor(M3,-2300);  //M3 DOWN
  		  while (!M3_movedone) {
         }
  		  Delay_ms(1000);

  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PUMP reset
  	  Delay_ms(100);
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // PUMP STOP
  	  Delay_ms(50);

  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  	  Delay_ms(50);
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
  	  Delay_ms(200);
  	  Move_Motor(M3,2100); //M3 UP
  	  while (!M3_movedone) {
  	    }

  	  Delay_ms(2200);
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  	  Delay_ms(100);
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP STOP
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  	  Delay_ms(50);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);



    break;
    case E:




    break;
    case F:




    break;
    case G:




    break;
    case H:



    break;

    default:


    }

	Move_Motor(M1,1300); //MOVE M1
	    while (!M1_movedone) {
	    }

	Move_Motor(M2,500);
	    while (!M2_movedone) {
	    }

	Move_Motor(M4, -220); //MOVE BASKET //CW:Pill 1->-200, pill 2->-380, pill 3->-560, pill 4->-740, pill 5->-920, pill 6->-1100, pill 7->1260, pill8->1450
	    while (!M4_movedone) {
	    }

	Delay_ms(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH

    Move_Motor(M3,-1500);  //M3 DOWN
		    while (!M3_movedone) {
		    }

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); //PUMP ON
	Delay_ms(750);

	Move_Motor(M3,1500); //M3 UP
	while (!M3_movedone) {
	    }

	Delay_ms(1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //CLOSE HATCH

	Delay_ms(750);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP OFF


}
void Second_Pick(int16_t val1, int16_t val2, int16_t val3)
{
	Calibrate(M1);
	Calibrate(M2);
	Calibrate(M3);
	Calibrate(M4);
	Calibrate(M5);
	Delay_ms(500);


    M1_movedone = 0;
    M2_movedone = 0;
    M3_movedone = 0;
    M4_movedone = 0;
    M5_movedone = 0;

	Move_Motor(M1,1500+val3); //origin 1300, down->+, right->+    M1>CORDINMATE SENCOND DATA  M2> COR FIRST DATA  CCW>-
	    while (!M1_movedone) {
	    }
		Delay_ms(100);

    Move_Motor(M2,val2);
		    while (!M2_movedone) {
		    }

	Delay_ms(100);
    Move_Motor(M5,val1);
		    while (!M5_movedone) {
		    }
			Delay_ms(100);
	Move_Motor(M3,-630);
	    while (!M3_movedone) {
	    }

	Delay_ms(100);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    Delay_ms(100);
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // PUMP START
     Delay_ms(500);

     Move_Motor(M3,300);
     while (!M3_movedone) {
     }
     Delay_ms(1500);
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
     Delay_ms(100);
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //PUMP STOP
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
     Delay_ms(50);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

//
//	Move_Motor(M4, -220); //MOVE BASKET
//	    while (!M4_movedone) {
//	    }
//
//
//    Delay_ms(100);
//    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //OPEN HATCH
//
//	Delay_ms(100);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
//
//    Delay_ms(100);
//    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //CLOSE HATCH
//
//    	Delay_ms(100);

//        	Calibrate(M2);
//        	Calibrate(M1);
//
//        	Calibrate(M3);
//        	Calibrate(M4);
//        	Calibrate(M5);

}

void Move_Motor(Motor motor, int16_t step)
{


	uint16_t abs_step = (step < 0) ? -step : step;
	switch (motor)
	{
	case M1:
		HAL_GPIO_WritePin(GPIOA, DIR_M1_Pin, (step < 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);// Set direction right<0, left>0,
	    pwm_target_M1 = abs_step;                        // Set target steps
	    pwm_count_M1 = 0;                             // Reset step count
	    HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);

	    break;
	case M2:
		HAL_GPIO_WritePin(GPIOA, DIR_M2_Pin, (step < 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);// Set direction  forward>0, backward<0
		pwm_target_M2 = abs_step;                        // Set target steps
		pwm_count_M2 = 0;                             // Reset step count
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4);
		break;
	case M3:
		HAL_GPIO_WritePin(GPIOB, DIR_M3_Pin, (step < 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);// Set direction up>0, down<0
		pwm_target_M3 = abs_step;                        // Set target steps
		pwm_count_M3 = 0;                             // Reset step count
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
		break;
	case M4:
		HAL_GPIO_WritePin(GPIOB, DIR_M4_Pin, (step < 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);// Set direction cw>0, ccw<0
		pwm_target_M4 = abs_step;                        // Set target steps
		pwm_count_M4 = 0;                             // Reset step count
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);
		break;
	case M5:
		HAL_GPIO_WritePin(GPIOA, DIR_M5_Pin, (step < 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);// Set direction CW>0, CCW<0
		pwm_target_M5 = abs_step;                        // Set target steps
		pwm_count_M5 = 0;                             // Reset step count
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
		break;

	default:
		return;
	}

}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
