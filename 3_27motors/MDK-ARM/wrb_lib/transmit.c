#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"

#include "stm32h7xx_hal_fdcan.h"
#include "stm32h7xx_it.h"
#include "usart.h"
#include "fdcan.h"
#include "main.h"
#include "transmit.h"
#include "wrb_communication.h"
#include "stdlib.h"

uint8_t dbus_meta_data0[RX_BUFF_LENGH]__attribute__((at(0x24006000)));

Wheel wheel={
{0,0,0,0},//1
{0,0,0,0},//2
{0,0,0,0},//3
{0,0,0,0},//4
//{0,0,0,0},//5
1,//final_sense
0,//delta_speed
0.01,//delta_speed_sense������0.02ʱ�ᶶ������ǳ�������
1,//vx_sense
1,//vy_sense
1//omega_sense
};

 typedef struct 
{
double tergat_speed[4];
double fabs_max_speed;
float dead_grap;
double feedback_speed[4];
double final_output_speed[4];
float pid_p_param;
float pid_i_param;
float pid_d_param;


}Motors_wheels;
//����ӳ��
#define key_W        key[0]
#define key_A        key[1]
#define key_S        key[2]
#define key_D        key[3]
#define key_shift    key[4]
#define key_ctrl     key[5]
#define key_Q        key[6]
#define key_E        key[7]
#define key_V        key[8]
#define key_F        key[9]
#define key_G        key[10]
#define key_C        key[11]
#define key_R        key[12]
#define key_B        key[13]
#define key_Z        key[14]
#define key_X        key[15]

 
 
RC_Ctl_t RC_Ctl;

 
 #define Key_Filter_Num 7//�ʵ�ֵ���Ը��Ʒ�����ʱ
#define key_W_flag        keyflag[0]
#define key_A_flag        keyflag[1]
#define key_S_flag        keyflag[2]
#define key_D_flag        keyflag[3]
#define key_shift_flag    keyflag[4]
#define key_ctrl_flag     keyflag[5]
#define key_Q_flag        keyflag[6]
#define key_E_flag        keyflag[7]
#define key_V_flag        keyflag[8]
#define key_F_flag        keyflag[9]
#define key_G_flag        keyflag[10]
#define key_C_flag        keyflag[11]
#define key_R_flag        keyflag[12]
#define key_B_flag        keyflag[13]
#define key_Z_flag        keyflag[14]
#define key_X_flag        keyflag[15]
 
 void PC_keybroad_filter()//����
{
		static uint16_t key_W_cnt,key_A_cnt,key_S_cnt,key_D_cnt,key_ctrl_cnt,
		               key_shift_cnt,mouse_press_l_cnt,mouse_press_r_cnt,
		               key_C_cnt,key_F_cnt,key_G_cnt,key_Q_cnt,key_E_cnt,
										key_Z_cnt,key_V_cnt,key_X_cnt,key_B_cnt,key_R_cnt;
		/*   ֧�������� W A S D   */
		//key_W
	if(RC_Ctl.key_W==1) 
	{
		key_W_cnt++;
		if(key_W_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_W_flag=1;	
		}	 
	}   
	else
	{
		RC_Ctl.key_W_flag=0;	
		key_W_cnt=0;	
	}	
			//key_A
	if(RC_Ctl.key_A==1) 
	{
		key_A_cnt++;
		if(key_A_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_A_flag=1;
		}	
	}
	else
	{	
		key_A_cnt=0;	
		RC_Ctl.key_A_flag=0;
	}
	//key_S
	if(RC_Ctl.key_S==1) 
	{
		key_S_cnt++;
		if(key_S_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_S_flag=1;	
		}			
	}	
	else
	{
		 key_S_cnt=0;
		RC_Ctl.key_S_flag=0;
	}		
	//key_D
	if(RC_Ctl.key_D==1) 
	{
		key_D_cnt++;
		if(key_D_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_D_flag=1;			
		}	
	}		
	else
	{
		 key_D_cnt=0;
		RC_Ctl.key_D_flag=0;
	}
	
	 	//key_B
	if(RC_Ctl.key_B==1) 
	{
		key_B_cnt++;
		if(key_B_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_B_flag=1;
		}	
	}	
	else
	{
		key_B_cnt=0;
		RC_Ctl.key_B_flag=0;
	}
	//key_C
	if(RC_Ctl.key_C==1) 
	{
		key_C_cnt++;
		if(key_C_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_C_flag++;
		}	 
	}  
	else
	{
	  key_C_cnt=0;
//		RC_Ctl.key_C_flag=0;
	}	
	 	//key_R
	if(RC_Ctl.key_R==1) 
	{
		key_R_cnt++;
		if(key_R_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_R_flag++;			
		}	 
	}   else
	 {
		key_R_cnt=0;	
//		RC_Ctl.key_R_flag=0;
	 }
	//key_F
	if(RC_Ctl.key_F==1) 
	{
		key_F_cnt++;
		if(key_F_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_F_flag++;
	  if(RC_Ctl.key_F_flag>1) RC_Ctl.key_F_flag=0;
		}	
	}	else
	 {
		key_F_cnt=0;
//		RC_Ctl.key_F_flag=0;
	 }
	 	//key_X
	if(RC_Ctl.key_X==1) 
	{
		key_X_cnt++;
		if(key_X_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_X_flag=1;			
		}	 
		else
		{
		RC_Ctl.key_X_flag=0;
		}
	}	
	else
	{
		key_X_cnt=0;
		RC_Ctl.key_X_flag=0;
  }
	//key_G
	if(RC_Ctl.key_G==1) 
	{
		key_G_cnt++;
		if(key_G_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_G_flag=1;		
		}	
		else
		{
		RC_Ctl.key_G_flag=0;
		}		
	}	
	else
	{
		key_G_cnt=0;
		RC_Ctl.key_G_flag=0;
	}	
		//key_Q
	if(RC_Ctl.key_Q==1) 
	{
		key_Q_cnt++;
		if(key_Q_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_Q_flag=1;			
		}	
    else
   {
    RC_Ctl.key_Q_flag=0;	
   }	
	}	
	else
	{
		key_Q_cnt=0;
		RC_Ctl.key_Q_flag=0;
	}
	//key_E
	if(RC_Ctl.key_E==1) 
	 {
		key_E_cnt++;
		if(key_E_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_E_flag++;	
    if(RC_Ctl.key_E_flag>1)  RC_Ctl.key_E_flag=0;			
		}	 
	 }
	 else
	 {
		key_E_cnt=0;
		//RC_Ctl.key_E_flag=0;
	 }	
	//key_Z
		if(RC_Ctl.key_Z==1) 
	{
		key_Z_cnt++;
		if(key_Z_cnt==Key_Filter_Num)	//RC_Ctl.key_Z_flag��0��λ�ÿ���˼��һ��
		{
    RC_Ctl.key_Z_flag=1;
//		if(RC_Ctl.key_Z_flag>1)	RC_Ctl.key_Z_flag=0;
		}			
	}	else
	 {
		key_Z_cnt=0;
		RC_Ctl.key_Z_flag=0;
	 } 
	//key_V
		if(RC_Ctl.key_V==1) 
	{
		key_V_cnt++;
		if(key_V_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_V_flag=1;		
		}			
	}	else
	 {
		key_V_cnt=0;	
		RC_Ctl.key_V_flag=0;
	 } 	 
	//key_ctrl
	if(RC_Ctl.key_ctrl==1) 
	{
		key_ctrl_cnt++;
		if(key_ctrl_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_ctrl_flag=1;
		key_ctrl_cnt=0;	
		}	
	} 
	else
	{
		RC_Ctl.key_ctrl_flag=0;
	}	 
	//key_shift 
	 if(RC_Ctl.key_shift==1) 
	{
		key_shift_cnt++;
		if(key_shift_cnt==Key_Filter_Num)	
		{
    RC_Ctl.key_shift_flag=1;
		key_shift_cnt=0;	
		}	
   }	
 	else
	{
		RC_Ctl.key_shift_flag=0;
	}
	 //mouse_l
	 if(RC_Ctl.mouse.press_l==1)
	 {
		mouse_press_l_cnt++;
		if(mouse_press_l_cnt==Key_Filter_Num)
		{
			RC_Ctl.mouse.press_l_flag=1;
			mouse_press_l_cnt=0;
		}
	 }
	 else
	 {
		 RC_Ctl.mouse.press_l_flag=0;
	 }
	 	 //mouse_r
	 if(RC_Ctl.mouse.press_r==1)
	 {
		mouse_press_r_cnt++;
		if(mouse_press_r_cnt==Key_Filter_Num)
		{
			RC_Ctl.mouse.press_r_flag=1;
			mouse_press_r_cnt=0;
		}
	 }
	 else
	 {
		 RC_Ctl.mouse.press_r_flag=0;
	 }
}
			
void Keyboard_control_logic()
{


}
/******************************************************************
��������USART1_IDLE_CALLBACK
���ܣ������ڿ����жϵĲ���
��������
����ֵ����
����������Ĵ���
����������ɹرպͿ���DMA�Ĳ�����
					�н����ݡ�
					�����¼����㺯��
�ϼ�������USART1_IRQHandler 
�¼�������
			�������� Remote_deal
							 Rotation_Chassis_Remote_Dispack
******************************************************************/
//DMA_HandleTypeDef hdma_usart1_rx;

typedef struct
{
	uint16_t ID_0X201;
	uint16_t ID_0X202;
	uint16_t ID_0X203;
	uint16_t ID_0X204;//
	uint16_t ID_0X205;
	uint16_t ID_0X206;
	uint16_t ID_0X207;
	uint16_t ID_0X208;//
	uint16_t ID_0X1FF;
} Motor_ID_Info;
typedef struct
{
	uint16_t 	Remote;
	uint16_t  Referee;
	Motor_ID_Info	Can1;
  Motor_ID_Info Can2;
	uint16_t  Vision;
	uint16_t  Super_cap;
	uint16_t  Slave_Broad;
	uint16_t  Power_Meter;
	uint16_t Uart4;
	uint16_t Uart5;
	uint16_t Uart7;
	uint16_t Uart8;
	 } Debug_Info;//��ɶ�ģ�
Debug_Info Check=
{
50,0,0,0,0,0,0
};//��˵��ɵ�ģ�
Debug_Info Debug=
{
50,0,0,0,0,0,0
};//��˵��ɵ�ģ�
Debug_Info Flag=
{
50,0,0,0,0,0,0
};
 uint16_t this_time_rx_len ;//���ڽ��յ������ݳ���

void USART1_IDLE_CALLBACK()
{ 
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET))	
	{
			Check.Remote=0;    
			Debug.Referee++;
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);
//  		__HAL_UART_CLEAR_FLAG(&huart1,UART_CLEAR_RTOF);
		  __HAL_UART_CLEAR_OREFLAG(&huart1);
//      __HAL_UART_CLEAR_RTOFFLAG(&huart1);		
//		  (void)USART1->ISR;
		  (void)USART1->RDR;
//     __HAL_DMA_DISABLE(&hdma_usart1_rx);
 
     __HAL_DMA_DISABLE(&hdma_usart1_rx);
		 this_time_rx_len=RX_BUFF_LENGH-__HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		__HAL_DMA_SET_COUNTER(&hdma_usart1_rx,RX_BUFF_LENGH);
		__HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,DMA_FLAG_TCIF0_4|DMA_FLAG_HTIF0_4);
		 __HAL_DMA_ENABLE(&hdma_usart1_rx);
     if(this_time_rx_len==18) //��֪��Ϊʲô
		 {
		  	USART1_receive_deal(dbus_meta_data0);
		 }
		 else
		 {
			 HAL_UART_DMAStop(&huart1);
			 HAL_UART_Receive_DMA(&huart1,dbus_meta_data0,RX_BUFF_LENGH);
		 }

	 }
}	
/***************************************************
  
***************************************************/

void USART1_receive_deal(uint8_t * dbus_meta_data)//��remote_deal();����
{
uint8_t correct_num=0;//_���������ı���	����ʼ��Ϊ0����18���ֽڷֳ�6�ݣ�ÿ�Ƚ�һ�����ݾ�++
       		correct_num=0;
				if(/*���ͨ��һ��ֵС��1684�Ҵ���364*/( (dbus_meta_data[0] | (dbus_meta_data[1] << 8)) & 0x07ff) <=1684   &&/*����*/  ((dbus_meta_data[0]        | (dbus_meta_data[1] << 8)/*Ϊ��*/)                            & 0x07ff) >=364)
					correct_num++;//1
				if((((dbus_meta_data[1] >> 3) | (dbus_meta_data[2] << 5)) & 0x07ff)<=1684                            	&&/*����*/  (((dbus_meta_data[1] >> 3) | (dbus_meta_data[2] << 5))                            & 0x07ff) >=364)
					correct_num++;
				if((((dbus_meta_data[2] >> 6) | (dbus_meta_data[3] << 2) |(dbus_meta_data[4] << 10)) & 0x07ff)<=1684 	&&/*����*/  (((dbus_meta_data[2] >> 6) | (dbus_meta_data[3] << 2) |(dbus_meta_data[4] << 10)) & 0x07ff) >=364)
					correct_num++;//3
				if((RC_Ctl.rc.ch3 = ((dbus_meta_data[4] >> 1) | (dbus_meta_data[5] << 7)) & 0x07ff)<=1684 &&/*����*/  (RC_Ctl.rc.ch3 = ((dbus_meta_data[4] >> 1) | (dbus_meta_data[5] << 7)) & 0x07ff)>=364)
					correct_num++;
				if((((dbus_meta_data[5] >> 4)& 0x000C) >> 2)==1 || (((dbus_meta_data[5] >> 4)& 0x000C) >> 2)==2 || (((dbus_meta_data[5] >> 4)& 0x000C) >> 2)==3)
					correct_num++;//5
				if(((dbus_meta_data[5] >> 4)& 0x0003)==1 || ((dbus_meta_data[5] >> 4)& 0x0003)==2 || ((dbus_meta_data[5] >> 4)& 0x0003)==3)
					correct_num++;
				
				if(correct_num==6)	//�����������֤ ����ʼ������ݸ�RC_Ctl�ṹ�壬RC_Ctl�ṹ����ǰ���ch0~3,s1,s2,mouse x,y,z,���left,right,key�Ľṹ��ģ� �����׶������ֲῴ��
				{//�ⲿ�ֿ��ҵĻ�ͼ�ʼ�
				RC_Ctl.rc.ch0 = (dbus_meta_data[0]| (dbus_meta_data[1] << 8)) & 0x07ff; //!< Channel 0   ��8λ���3λ
				RC_Ctl.rc.ch1 = ((dbus_meta_data[1] >> 3) | (dbus_meta_data[2] << 5)) & 0x07ff; //!< Channel 1     ��5λ���6λ
				RC_Ctl.rc.ch2 = ((dbus_meta_data[2] >> 6) | (dbus_meta_data[3] << 2) |(dbus_meta_data[4] << 10)) & 0x07ff; //!< Channel 2
				RC_Ctl.rc.ch3 = ((dbus_meta_data[4] >> 1) | (dbus_meta_data[5] << 7)) & 0x07ff; //!< Channel 3
				RC_Ctl.rc.s1 = ((dbus_meta_data[5] >> 4)& 0x000C) >> 2; //!< Switch left
				RC_Ctl.rc.s2 = ((dbus_meta_data[5] >> 4)& 0x0003); //!< Switch right
        RC_Ctl.rc.sw=(uint16_t)(dbus_meta_data[16]|(dbus_meta_data[17]<<8))&0x7ff; 
					//ch0Ϊ��ҡ������horizontal��ch1Ϊ��ҡ��ǰ��vertical
				RC_Ctl.rc.right_HR=(RC_Ctl.rc.ch0-1024);//1024Ϊ1684��364���м����ֵ����
				RC_Ctl.rc.right_VR=(RC_Ctl.rc.ch1-1024);//Ϊʲôң����4��ͨ����ֵ���ö�Ӧ����ĵ���ֵ��Χ��
				RC_Ctl.rc.left_HR=(RC_Ctl.rc.ch2-1024);//��ר��Ϊ�˷�����ٿض�������Ƶ���
			  RC_Ctl.rc.left_VR=(RC_Ctl.rc.ch3-1024);
					
					/***********����ӳ��*************/
	      RC_Ctl.mouse.x = dbus_meta_data[6]  | (dbus_meta_data[7] << 8);                    //!< Mouse X axis������   
        RC_Ctl.mouse.y = dbus_meta_data[8]  | (dbus_meta_data[9] << 8);                    //!< Mouse Y axis     
        RC_Ctl.mouse.z = dbus_meta_data[10] | (dbus_meta_data[11] << 8);                  //!< Mouse Z axis   
        RC_Ctl.mouse.press_l = dbus_meta_data[12];                                        //!< Mouse Left Is Press ?   
        RC_Ctl.mouse.press_r = dbus_meta_data[13];                                        //!< Mouse Right Is Press ? 
				
			 if(RC_Ctl.mouse.x>25000)   RC_Ctl.mouse.x=25000;//��ʵ��Χ��+-32767�������ǿ��ǵ��ʵ�ʹ������ޣ���һ���޷���
			 if(RC_Ctl.mouse.x<-25000)  RC_Ctl.mouse.x=-25000;	
			 if(RC_Ctl.mouse.y>25000)   RC_Ctl.mouse.y=25000;
			 if(RC_Ctl.mouse.y<-25000)  RC_Ctl.mouse.y=-25000;
		//��֪������ֵ��ɶ�ã�����	
		//����ң����զ����mouse,keyboard?----����Ϊ������usb�ߺ�ң��������һ��ʱ�Ϳ����õ��Կ����ˡ�û��ʱ��Ȼֻ������4��ͨ����s1s2����
			  RC_Ctl.keyboard.v = dbus_meta_data[14]| (dbus_meta_data[15] << 8);  	//!< KeyBoard value   

			
				RC_Ctl.key_W=dbus_meta_data[14]&0x01;	//uint8_t key[18]��Ӧ18������
				RC_Ctl.key_S=(dbus_meta_data[14]>>1)&0x01;					
				RC_Ctl.key_A=(dbus_meta_data[14]>>2)&0x01;
				RC_Ctl.key_D=(dbus_meta_data[14]>>3)&0x01;					
				RC_Ctl.key_B=(dbus_meta_data[15]>>7)&0x01;
				RC_Ctl.key_V=(dbus_meta_data[15]>>6)&0x01;				
				RC_Ctl.key_C=(dbus_meta_data[15]>>5)&0x01;
				RC_Ctl.key_X=(dbus_meta_data[15]>>4)&0x01;					
				RC_Ctl.key_Z=(dbus_meta_data[15]>>3)&0x01;					
				RC_Ctl.key_G=(dbus_meta_data[15]>>2)&0x01;			
				RC_Ctl.key_F=(dbus_meta_data[15]>>1)&0x01;
				RC_Ctl.key_R=(dbus_meta_data[15])&0x01;													
				RC_Ctl.key_E=(dbus_meta_data[14]>>7)&0x01;
				RC_Ctl.key_Q=(dbus_meta_data[14]>>6)&0x01;
				RC_Ctl.key_ctrl=(dbus_meta_data[14]>>5)&0x01;
				RC_Ctl.key_shift=(dbus_meta_data[14]>>4)&0x01;
					
				//����
				PC_keybroad_filter();
				
//				//******����0.0����begin��
//				if(abs(RC_Ctl.rc.ch1-1024)>10)
//				{
//					for(int i=0;i<4;i++) wheel.Delta_speed=(RC_Ctl.rc.ch1-1024)*wheel.final_sense-wheel.target_speed[i];
//				}
//				else 
//				{			
//					for(int i=0;i<4;i++)
//					{ 
//					 wheel.target_speed[i] = 0; 
//					 wheel.Delta_speed=(RC_Ctl.rc.ch1-1024)-wheel.target_speed[i];
//					}
//						
//				}
//				//******����0.0����end

//				//******����1.0����begin��

//abs(RC_Ctl.rc.ch1-1024)>10? (delta_move.vx=(RC_Ctl.rc.ch1-1024)*wheel.final_sense-move.vx) : (move.vx=0;delta_move.vx=(RC_Ctl.rc.ch1-1024)*wheel.final_sense-move.vx);//f0
//abs(RC_Ctl.rc.ch0-1024)>10? (delta_move.vy=(RC_Ctl.rc.ch0-1024)*wheel.final_sense-move.vy) : (move.vy=0;delta_move.vx=(RC_Ct0.rc.ch1-1024)*wheel.final_sense-move.vx);//r0

				if(abs(RC_Ctl.rc.ch1-1024)<10)
					move.vx=0;
				if(abs(RC_Ctl.rc.ch0-1024)<10)
					move.vy=0;
				if(abs(RC_Ctl.rc.ch2-1024)<10)
					move.omega=0;

				delta_move.vx=(RC_Ctl.rc.ch1-1024)*wheel.final_sense-move.vx;
				delta_move.vy=(RC_Ctl.rc.ch0-1024)*wheel.final_sense-move.vy;
				delta_move.omega=(RC_Ctl.rc.ch2-1024)*wheel.final_sense-move.omega;
				
//				//******����0.0����end

	}
}



#define RC_sense 24.8f;
//void Wheel_tergat_original()
//{
//	for(int i=0;i<4;i++)
//	{	
//		wheel.original_speed[i] = RC_Ctl.rc.ch0 -1024;
//		if(wheel.original_speed[i] > max_output)
//		{
//			wheel.original_speed[i] = max_output;
//		}
//		if(wheel.original_speed[i] <  min_output)
//		{
//			wheel.original_speed[i]= min_output;
//		}
//	}
//	

//}
struct Pid_Info
{	
      float Kp;//����ϵ��
      float Ki;//����ϵ��
      float Kd;//΢��ϵ��  
      float E0;//��ֵ
			float Last_E0;
      float Ep_Part;//�������ֿ�����
      float Ei_Part;//���ֲ��ֿ�����
      float Ed_Part;//΢�ֲ��ֿ�����
			float Ei_Part_Limit;
			float Out_Limit;//����޷�
      float Out;//������ֵ
	    float Target; //Ŀ��ֵ
	    float Feedback;//����ֵ
};
struct Pid_Info Pid_Wheel=
{
	9,0,0,0
};
float Wheel_Pid_Control(float Feedback,int16_t target,uint8_t who)//ֻ����P I����
{
		static  float  E_Sum[4];

	Pid_Wheel.Target=target;
	
	Pid_Wheel.Feedback=Feedback;
	if(fabs(Pid_Wheel.Feedback)<10){Pid_Wheel.Feedback=0;}
		
	Pid_Wheel.E0=Pid_Wheel.Target-Pid_Wheel.Feedback;//E0ΪĿ��ֵ�뷴��ֵ֮������Ϊ����
    
	Pid_Wheel.Ep_Part=Pid_Wheel.E0*Pid_Wheel.Kp;	//��������
	if(Pid_Wheel.E0<50||Pid_Wheel.E0>-50)    //�������� 
	{
	E_Sum[who]+=Pid_Wheel.E0*Pid_Wheel.Ki;//��+=�ۼ�
	}
	if(target==0) 
	{
		E_Sum[who]=0;	//who��ʾ�ĸ�wheel???
	}
	Pid_Wheel.Ei_Part=E_Sum[who];
	if(Pid_Wheel.Ei_Part>800) Pid_Wheel.Ei_Part=800;//�����޷�//Ϊɶ��800��
	if(Pid_Wheel.Ei_Part<-800) Pid_Wheel.Ei_Part=-800;
	
	Pid_Wheel.Ed_Part=Pid_Wheel.Kd*(Pid_Wheel.E0-Pid_Wheel.Last_E0);//΢�ֿ��ƣ��Ƚ��������ĸ�������� ��Щ��֣�����
	
	Pid_Wheel.Last_E0=Pid_Wheel.E0;

	Pid_Wheel.Out=Pid_Wheel.Ep_Part+Pid_Wheel.Ei_Part+Pid_Wheel.Ed_Part; //�ۺ�ͳ���������ֵ���
	
//	if(Pid_Wheel.Out>Chassis_Max_out) Pid_Wheel.Out=Chassis_Max_out;
//	if(Pid_Wheel.Out<-Chassis_Max_out) Pid_Wheel.Out=-Chassis_Max_out;  
		if(Pid_Wheel.Out>12000) Pid_Wheel.Out=12000;
	if(Pid_Wheel.Out<-12000) Pid_Wheel.Out=-12000;
	return Pid_Wheel.Out;	
}
/******************************************

*******************************************/
uint32_t Clock_time;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//1,��usart1�����������½���
//	Wheel_tergat_original();
//2,pid+�޷�
//	for(int i=0;i<4;i++)
//	wheel.pid_output_speed[i]=Wheel_Pid_Control(wheel.feedback_speed[i] , wheel.original_speed[i] , i);//ֻ����P I����
//3,�����������
//	Wheel_Control();
	//4.pid:
		
	
	//**********<-���Լ�д�Ļ�����end.
//	for(int i=0;i<4;i++)
//	{
//	
//		wheel.original_speed[i] = RC_Ctl.rc.ch0 -1024;
//		if(wheel.pid_output_speed[i]*wheel.final_sense > max_output)
//		{
//			wheel.finalt_speed[i] = max_output;
//		}
//		else if(wheel.pid_output_speed[i]*wheel.final_sense <  min_output)
//		{
//			wheel.finalt_speed[i]= min_output;
//		}
//		else
//		{
//				wheel.finalt_speed[i] = wheel.pid_output_speed[i]*wheel.final_sense;
//		}
//	}
//	//**********���Լ�д�Ļ�����0.0begin->
//	for(int i=0;i<4;i++){
//	 wheel.target_speed[i] += wheel.Delta_speed * wheel.Delta_speed_sense ;//�����ﻺ����  Delta_speed_sense=0.01
//	wheel.final_output_speed[i] = Wheel_Pid_Control(wheel.feedback_speed[i] , wheel.target_speed[i] , i);//pid���й��޷��ˣ����ҵ�ȥ��
//	}
//	//**********<-���Լ�д�Ļ�����0.0end.

		//**********���Լ�д�Ļ�����1.0begin->
		if(move.vx>=0)
				{
					delta_move.vx>0?  (move.vx+=delta_move.vx*wheel.Delta_speed_sense):(move.vx+=delta_move.vx*wheel.Delta_speed_sense*2);//б��ϵ��0.01---wheel.Delta_speed_sense
				}
				else
				{
					delta_move.vx>0?  (move.vx+=delta_move.vx*wheel.Delta_speed_sense*2):(move.vx+=delta_move.vx*wheel.Delta_speed_sense);
				}
		if(move.vy>=0)
				{
					delta_move.vy>0?  (move.vy+=delta_move.vy*wheel.Delta_speed_sense):(move.vy+=delta_move.vy*wheel.Delta_speed_sense*2);//б��ϵ��0.01---wheel.Delta_speed_sense
				}
				else
				{
					delta_move.vy>0?  (move.vy+=delta_move.vy*wheel.Delta_speed_sense*2):(move.vy+=delta_move.vy*wheel.Delta_speed_sense);
				}
		if(move.omega>=0)
				{
					delta_move.omega>0?  (move.omega+=delta_move.omega*wheel.Delta_speed_sense):(move.omega+=delta_move.omega*wheel.Delta_speed_sense*2);//б��ϵ��0.01---wheel.Delta_speed_sense
				}
				else
				{
					delta_move.omega>0?  (move.omega+=delta_move.omega*wheel.Delta_speed_sense*2):(move.omega+=delta_move.omega*wheel.Delta_speed_sense);
				}
		wheel.target_speed[3]=move.vx+ move.omega*wheel.omega_sense+move.vy;
		wheel.target_speed[0]=move.vx+ move.omega*wheel.omega_sense-move.vy;
		wheel.target_speed[2]=-move.vx+move.omega*wheel.omega_sense+move.vy;
		wheel.target_speed[1]=-move.vx+move.omega*wheel.omega_sense-move.vy;
		for(int i=0;i<4;i++)
		wheel.final_output_speed[i] = Wheel_Pid_Control(wheel.feedback_speed[i] , wheel.target_speed[i] , i);//pid���й��޷���

		//**********<-���Լ�д�Ļ�����1.0end.


//	for(int i=0;i<4;i++)
//	{
//		wheel.Delta_speed=wheel.finalt_speed[i]-wheel.final_output_speed[i];
//		if(fabs(wheel.Delta_speed)<100)wheel.Delta_speed=0;
//		wheel.final_output_speed[i]+=wheel.Delta_speed*wheel.Delta_speed_sense;

//	}
		//3,send
	if(htim==(&htim14)){
		Clock_time++;
		Flag.Remote=Remote_Online_Check();
		/**�������߼�� led��˸����оƬ״̬****************/
	//	FDCAN2_send_msg_chassis(wheel.final_output_speed);
		/**�������߼�� led��˸����оƬ״̬****************/
		Flag.Remote=Remote_Online_Check();
		//
		if(Flag.Remote==0)
		{
			FDCAN2_send_msg_chassis(wheel.final_output_speed);
			
		}
		else
		{
//			move.vx=0;
//			move.vy=0;
			Remote_Init();
			float zero[4]={0,0,0,0};
//			FDCAN1_send_msg_load(zero);
//			FDCAN2_send_yaw_holder(zero);
//			FDCAN1_send_pitch_holder(zero);
			FDCAN2_send_msg_chassis(zero);
		//	Friction_Start_Cnt=0;	//Ħ�������»����� ��ֹ���������ջ�����

		}
	}

}

//void Wheel_Control()
//{
//	
//	for (int i=0;i<4;i++)
//	{
////		if(wheel.pid_output_speed[i]>max_output)
////			wheel.final_output_speed[i]=10000;
////		if(wheel.original_speed[i]<min_output)
////			wheel.final_output_speed[i]=-10000;
//		wheel.final_output_speed[i]=(wheel.original_speed[i]-0)*wheel.final_sense;
//	}
//		
//}


/******************************************************************
��������FDCANx_send_msg_xxxx()
���ܣ�canͨ�ŷ��ͺ��� �������Ͳ���
������float * number
			�ǳ���Ϊ4��float������׵�ַ ��Ӧcan������һ��ID������֡���˸��ֽڣ�
			����Ŷ�Ӧ���ת�ٵ�������Ϊ�������뼴��
����ֵ��0����1
�������FDCAN_TxHeaderTypeDef CAN_Txstructure 	
					CAN���ͺ������,�洢������һ��can��׼ͨ��֡����������
													����Ҫ�޸ĵ������У�  ID��ʶλ
																							 ֡��ʽΪ��׼/��չ
																							 
��������RC_Ctl_t * RC_Ctl
�ϼ�������USART1_IDLE_CALLBACK �ڴ����ж��б�����
�¼�������
******************************************************************/
uint8_t FDCAN2_send_msg_chassis(float * number)
{
	 uint8_t data[8]={0};
	 FDCAN_TxHeaderTypeDef CAN_Txstructure;
	 
   CAN_Txstructure.IdType=FDCAN_STANDARD_ID;
	 CAN_Txstructure.Identifier=0x200;
	 CAN_Txstructure.TxFrameType=FDCAN_DATA_FRAME;
	 CAN_Txstructure.DataLength=FDCAN_DLC_BYTES_8;
	 CAN_Txstructure.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
	 CAN_Txstructure.BitRateSwitch=FDCAN_BRS_OFF;
	 CAN_Txstructure.FDFormat=FDCAN_CLASSIC_CAN;
	 CAN_Txstructure.TxEventFifoControl=FDCAN_NO_TX_EVENTS;
	 CAN_Txstructure.MessageMarker=0;
	 
	 data[0]=((int16_t)number[0]>>8);			 
   data[1]=((int16_t)number[0]&0xff);   
	 data[2]=((int16_t)number[1]>>8);
	 data[3]=((int16_t)number[1]&0xff);		 	 
   data[4]=((int16_t)number[2]>>8); 
	 data[5]=((int16_t)number[2]&0xff);				 
   data[6]=((int16_t)number[3]>>8);
	 data[7]=((int16_t)number[3]&0xff);			 

//if(htim==(&htim14))
//	{
//		Clock_time++;
		
		//

   if( HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2,&CAN_Txstructure,data)!= HAL_OK)
	 {
	   ;
	 }
 //}
	 	 return 0;
	
}

/******************************************************************
��������Remote_Online_Check
���ܣ�ң�ص���
������
����ֵ��
�������
��������
�ϼ�������
�¼�������
��Ҫ�޸����ݣ�
	
******************************************************************/

uint8_t Remote_Online_Check()
{
	if(Check.Remote<30)
	{
		Check.Remote++;
		return 0;
	}
	else
	{	
		//Clock_time--;//���¼�ʱ
    return 1;//����
	}
}
/*************************************
ԭ��Ϊ�����壬Ҳ��Ҫ��ͷ�ļ�������
*****************************************/
struct KK kk;

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
//	wheel.original_speed[0] *= 2;
	FDCAN_RxHeaderTypeDef RxMessage1,RxMessage2;

  uint8_t Data1[8],Data2[8];

	if(hfdcan==&hfdcan1)
	{
		if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
		{
			/* Retreive Rx messages from RX FIFO0 */
			if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxMessage1, Data1) != HAL_OK)
			{
			/* Reception Error */
			Error_Handler();
			}
			if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) == HAL_OK)
			{
//      /*pitch��������������㴦����can�����жϴ���*/

//          if(RxMessage1.Identifier==0x205)
//					{  
//						Check.Can1.ID_0X205=0;
//					  Debug.Can1.ID_0X205++;
//						Holder.Pitch.Can_Angle_Raw=Data1[0]<<8|Data1[1];
//						Pitch_Angle_Can_Change();
//					}
//					else if(RxMessage1.Identifier>0x200&&RxMessage1.Identifier<0x204)
//					{
//						Motor_Load.Feedback_Speed[RxMessage1.Identifier-0x201]=Data1[2]<<8|Data1[3];
//						if(RxMessage1.Identifier==0x201)
//						{
//							Check.Can1.ID_0X201=0;
//					    Debug.Can1.ID_0X201++;
//						}
//						else if(RxMessage1.Identifier==0x202)
//						{
//							Check.Can1.ID_0X202=0;
//					    Debug.Can1.ID_0X202++;
//						}
//						else if(RxMessage1.Identifier==0x203)
//						{
//							Check.Can1.ID_0X203=0;
//					    Debug.Can1.ID_0X203++;
//						}
//		//			  if(Motor_Load.Feedback_Speed[2]<10&&Motor_Load.Feedback_Speed[2]>-10)
//						{
//							Motor_Load.Feedback_Speed[2]=0;
//						}
//					}
				}
			}
		}
	if(hfdcan==&hfdcan2)
	{
		if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
		{
//			/* Retreive Rx messages from RX FIFO0 */
			if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxMessage2, Data2) != HAL_OK)
			{
			/* Reception Error */
			Error_Handler();
			}
	kk.a=2;
			if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) == HAL_OK)
			{
//      /*yaw��������������㴦����can�����жϴ���*/

			  if(0x200<RxMessage2.Identifier&&RxMessage2.Identifier<=0x204)
				{
						wheel.feedback_speed[RxMessage2.Identifier-0x201]	=Data2[2]<<8|Data2[3];	
						if(RxMessage2.Identifier==0x201)
						{
							Check.Can2.ID_0X201=0;
					    Debug.Can2.ID_0X201++;
						}
						else if(RxMessage2.Identifier==0x202)
						{
							Check.Can2.ID_0X202=0;
					    Debug.Can2.ID_0X202++;
						}
						else if(RxMessage2.Identifier==0x203)
						{
							Check.Can2.ID_0X203=0;
					    Debug.Can2.ID_0X203++;
						}
						else if(RxMessage2.Identifier==0x204)
						{
							Check.Can2.ID_0X204=0;
					    Debug.Can2.ID_0X204++;
						}
				}
				if(RxMessage2.Identifier==0x205)
				{   
						Check.Can2.ID_0X205=0;
					  Debug.Can2.ID_0X205++;
				//  	Holder.Yaw.Can_Angle_Raw=Data2[0]<<8|Data2[1];
					//	Holder.Yaw.Can_Speed_Feedback = Data2[2]<<8|Data2[3];
				 // 	Yaw_Angle_Can_Change();
//					  Chassis.Feedback_Angle=Holder.Yaw.Can_Angle;
				}
//	      else if(RxMessage2.Identifier==0x601)
//				{
//					Check.Super_cap=0;
//					Debug.Super_cap++;
//					super_cap_voltage=(Data2[0]<<8|Data2[1])/100;
//				}
//				else if(RxMessage2.Identifier==0x5ff)
//				{   Debug.Slave_Broad++;
//					  Check.Slave_Broad=0;
//						Chassis.Yaw.Bmi088_Angle=((int16_t)(Data2[0]<<8|Data2[1]))/100.0;
//						Chassis.Pitch.Bmi088_Angle=((int16_t)(Data2[2]<<8|Data2[3]))/100.0;
//						Chassis.Yaw.Bmi088_Angle_speed=(int16_t)(Data2[4]<<8|Data2[5]);
//						Chassis.Pitch.Bmi088_Angle_speed=(int16_t)(Data2[6]<<8|Data2[7]);	
//					  Chassis.Feedback_Speed=Chassis.Yaw.Bmi088_Angle_speed;
//				}
			}
		}
	}	
	
	
	
	
	
	/*******************************/
//	if(hfdcan==&hfdcan2){
//			FDCAN_RxHeaderTypeDef RxMessage1, RxMessage2;
//		 uint8_t Data1[8],Data2[8];
//		
//			if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
//		{
//			/* Retreive Rx messages from RX FIFO0 */
//			if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxMessage1, Data1) != HAL_OK)
//			{
//			/* Reception Error */
//			Error_Handler();
//			}
//			if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) == HAL_OK)
//			{
//						if(0x200<RxMessage2.Identifier&&RxMessage2.Identifier<=0x204)
//					{
//							wheel.feedback_speed[RxMessage2.Identifier-0x201]	=Data2[2]<<8|Data2[3];	
//					}
//			}
//		}
//	}

//			}
//		}
//	}
}
