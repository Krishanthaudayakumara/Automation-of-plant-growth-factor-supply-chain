void GSM_Begin();
void GSM_Calling(char *);
void GSM_HangCall();
void GSM_Response();
void GSM_Response_Display();
void GSM_Msg_Read(int);
bool GSM_Wait_for_Msg();
void GSM_Msg_Display();
void GSM_Msg_Delete(unsigned int);
void GSM_Send_Msg(char* , char*);
void GSM_Delete_All_Msg();

char buff[160];								/* buffer to store responses and messages */
char status_flag = 0;						/* for checking any new message */
volatile int buffer_pointer;
char Mobile_no[14];							/* store mobile no. of received message */
char message_received[60];					/* save received message */
int position = 0;							/* save location of current message */



void GSM_Begin()
{

	while(1)
	{
		lcdcmd(0xc0);
		USART_SendString("ATE0\r");			/* send ATE0 to check module is ready or not */
		_delay_ms(500);
		if(strstr(buff,"OK"))
		{
			GSM_Response();                 /* get Response */
			memset(buff,0,160);
			break;
		}
		else
		{
			lcd_string("Error",5);
		}
	}
	_delay_ms(1000);

	lcd_clear();
	lcd_string("Text Mode",9);
	lcdcmd(0xc0);
	USART_SendString("AT+CMGF=1\r");/* select message format as text */
	GSM_Response();
	_delay_ms(1000);
}


/* ISR routine to save responses/new message */
ISR(USART0_RX_vect)
{
	buff[buffer_pointer] = UDR0;							/* copy UDR(received value) to buffer */
	buffer_pointer++;
	status_flag = 1;						            /* flag for new message arrival */
}


void GSM_Send_Msg(char *num,char *sms)
{
	char sms_buffer[35];
	buffer_pointer=0;
	sprintf(sms_buffer,"AT+CMGS=\"%s\"\r",num);
	USART_SendString(sms_buffer);                       /*send command AT+CMGS="Mobile No."\r */
	_delay_ms(200);
	while(1)
	{
		if(buff[buffer_pointer]==0x3e)                  /* wait for '>' character*/
		{
			buffer_pointer = 0;
			memset(buff,0,strlen(buff));
			USART_SendString(sms);                      /* send msg to given no. */
			USART_TxChar(0x1a);                         /* send Ctrl+Z then only message will transmit*/
			break;
		}
		buffer_pointer++;
	}
	_delay_ms(300);
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));
	memset(sms_buffer,0,strlen(sms_buffer));
}


void GSM_Response()
{
	unsigned int timeout=0;
	int CRLF_Found=0;
	char CRLF_buff[2];
	int Response_Length=0;
	while(1)
	{
		if(timeout>=60000)								/*if timeout occur then return */
		return;
		Response_Length = strlen(buff);
		if(Response_Length)
		{
			_delay_ms(2);
			timeout++;
			if(Response_Length==strlen(buff))
			{
				for(int i=0;i<Response_Length;i++)
				{
					memmove(CRLF_buff,CRLF_buff+1,1);
					CRLF_buff[1]=buff[i];
					if(strncmp(CRLF_buff,"\r\n",2))
					{
						if(CRLF_Found++==2)				/* search for \r\n in string */
						{
							GSM_Response_Display();		/* display response */
							return;
						}
					}

				}
				CRLF_Found = 0;

			}
			
		}
		_delay_ms(1);
		timeout++;
	}
	status_flag=0;
}

void GSM_Response_Display()
{
	buffer_pointer = 0;
	int lcd_pointer = 0;
	while(1)
	{
		if(buff[buffer_pointer]== '\r' || buff[buffer_pointer]== '\n')  /* search for \r\n in string */
		{
			buffer_pointer++;
		}
		else
		break;
	}
	

	lcdcmd(0xc0);
	while(buff[buffer_pointer]!='\r')								   /* display response till "\r" */
	{
		LCD_Char(buff[buffer_pointer]);
		buffer_pointer++;
		lcd_pointer++;
		if(lcd_pointer==15)								               /* check for end of LCD line */
		lcdcmd(0x80);
	}
	buffer_pointer=0;
	memset(buff,0,strlen(buff));
}
