#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal.h"
#include "transmit.h"

typedef struct Motors_wheels
{
double tergat_speed[4];
double fabs_max_speed;
float dead_grap;
double feedback_speed[4];
double final_output_speed[4];
float pid_p_param;
float pid_i_param;
float pid_d_param;


};


void USART1_receive_deal()
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

}


//***********************
void PID_wheels()
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//1,��usart1�����������½���

//2,pid
//3,�޷�
//4,send
	

}

