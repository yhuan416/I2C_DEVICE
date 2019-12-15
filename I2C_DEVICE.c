#include "I2C_DEVICE.h"

//init struct
int init_i2c_device(i2c_device_p i2c, i2c_cb scl_o, i2c_cb sda_o, i2c_cb sda_i, i2c_cb sda_rdwr)
{
	if (i2c == NULL)
	{
		return -1;
	}
		
	i2c->I2C_SCL_O = scl_o;
	
	i2c->I2C_SDA_O = sda_o;
	i2c->I2C_SDA_I = sda_i;
	
	i2c->I2C_SDA_RDWR = sda_rdwr;
}

//I2C设备发送开始信号
void I2C_start(i2c_device_p i2c)
{
	//sda输出模式
	i2c->I2C_SDA_RDWR(0);

	i2c->I2C_SCL_O(1);
	i2c->IIC_SDA_O(1);I2C_delay_us(2);
	
	i2c->IIC_SDA_O(0);I2C_delay_us(5);
	
	i2c->IIC_SCL_O(0);
}


void I2C_stop(i2c_device_p i2c)
{
	//sda输出模式
	i2c->I2C_SDA_RDWR(0);
	
	i2c->IIC_SCL_O(0);
	i2c->IIC_SDA_O(0);I2C_delay_us(2);
	
	i2c->IIC_SCL_O(1);I2C_delay_us(5);
	
	i2c->IIC_SDA_O(1);
}


int I2C_wait_ack(i2c_device_p i2c)
{
	uint8_t err_time = 0;
	
	i2c->I2C_SDA_RDWR(1);//sda in put mode
	
	i2c->IIC_SCL_O(1);I2C_delay_us(2);//wait sda low with scl high
	
	while(i2c->IIC_SDA_I(0) == 1)
	{
		err_time++;
		if(err_time >=250)
		{
			iic_stop();
			return 1;
		}
	}
	
	i2c->IIC_SCL_O(0);
	return 0;
}

void I2C_ack(i2c_device_p i2c)//sda output 0 while scl 0
{
	//sda输出模式
	i2c->I2C_SDA_RDWR(0);//sda out put mode
	
	i2c->IIC_SCL_O(0);
	i2c->IIC_SDA_O(0);I2C_delay_us(2);
	
	i2c->IIC_SCL_O(1);I2C_delay_us(5);
	
	i2c->IIC_SCL_O(0);I2C_delay_us(2);
}

void I2C_nack(i2c_device_p i2c)//sda output 1 while scl 0
{
	//sda输出模式
	i2c->I2C_SDA_RDWR(0);
	
	i2c->IIC_SCL_O(0);
	i2c->IIC_SDA_O(1);I2C_delay_us(2);
	
	i2c->IIC_SCL_O(1);I2C_delay_us(5);
	
	i2c->IIC_SCL_O(0);I2C_delay_us(2);
}

int I2C_write_byte(i2c_device_p i2c, uint8_t data, int wait_ack)
{
	int i;

	i2c->I2C_SDA_RDWR(0);//sda out put mode
	
	i2c->IIC_SCL_O(0);I2C_delay_us(2);
	
	for(i = 0; i < 8; i++)
	{
		if((data & 0x80) >> 7)
		{i2c->IIC_SDA_O(1);}
		else
		{i2c->IIC_SDA_O(0);}
		
		I2C_delay_us(2);
		
		i2c->IIC_SCL_O(1);I2C_delay_us(5);
		
		i2c->IIC_SCL_O(0);I2C_delay_us(2);
		
		data = data << 1;
	}
	
	/* ack */
	if(wait_ack && iic_wait_ack())
	{
		return 1;
	}
	
	return 0;
}

int I2C_read_byte(i2c_device_p i2c, uint8_t *data, int ack)
{
	int i;

	i2c->I2C_SDA_RDWR(1);I2C_delay_us(2);//sda in put mode
	
	for(i = 0; i < 8; i++)
	{
		i2c->IIC_SCL_O(1);I2C_delay_us(3);
		
		*data = *data << 1;
		
		if(i2c->IIC_SDA_I(0))
		{*data += 1;}
		
		i2c->IIC_SCL_O(0);I2C_delay_us(3);
	}
	
	/* ack */
	if(ack)
	{iic_ack();}
	else
	{iic_nack();}
	
	return 0;
}
    
