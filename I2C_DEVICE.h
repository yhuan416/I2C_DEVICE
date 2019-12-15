/*
I2C 通信协议(软件I2C)

在外部初始化好I2Cde引脚后再使用

需要提前配置好I2C设备的引脚操作函数
例如:
	SCL电平设置
	
	SDA电平设置/读取电平

	SDA输入/输出设置
	
然后初始化i2c_device_t结构体
*/

#ifndef _I2C_DEVICE_H_
#define _I2C_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef int (*i2c_cb)(int);

typedef struct
{
	//scl out put 
	i2c_cb I2C_SCL_O;
	
	//set/read sda
	i2c_cb I2C_SDA_O;
	i2c_cb I2C_SDA_I;

	//set sda read/write mode (1 : read|0 : write)
	i2c_cb I2C_SDA_RDWR;
} i2c_device_t, *i2c_device_p;

/* delay function */
#define I2C_delay_us    os_delay_us

//读写位
#define I2C_WR_BIT	0
#define I2C_RD_BIT	1

//init struct
int init_i2c_device(i2c_device_p i2c, i2c_cb scl_o, i2c_cb sda_o, i2c_cb sda_i, i2c_cb sda_rdwr);

/* satrt/stop */
void I2C_start(i2c_device_p i2c);
void I2C_stop(i2c_device_p i2c);

/* master wait ack */
int I2C_wait_ack(i2c_device_p i2c);

/* master ack/nack */
void I2C_ack(i2c_device_p i2c);
void I2C_nack(i2c_device_p i2c);

/* write/read a byte */
int I2C_write_byte(i2c_device_p i2c, uint8_t data, int wait_ack);
int I2C_read_byte(i2c_device_p i2c, uint8_t *data, int ack);

#ifdef __cplusplus
}
#endif

#endif

