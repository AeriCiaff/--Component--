#ifndef __MY_MPU6050_H__
#define __MY_MPU6050_H__

#define ERROR_MPU_INIT      -1
#define ERROR_SET_SENSOR    -2
#define ERROR_CONFIG_FIFO   -3
#define ERROR_SET_RATE      -4
#define ERROR_LOAD_MOTION_DRIVER    -5 
#define ERROR_SET_ORIENTATION       -6
#define ERROR_ENABLE_FEATURE        -7
#define ERROR_SET_FIFO_RATE         -8
#define ERROR_SELF_TEST             -9
#define ERROR_DMP_STATE             -10

#define		MPU_I2C_HANDLE		hi2c1
#define	    MPU_ADDR            0x68
#define     MPU_WRITE           0xD0
#define     MPU_READ            0xD1

#define	    AUX_VDDIO           0x01
#define     SMPLRT_DIV          0x19
#define     CONFIG              0x1A     
#define     GYRO_CONFIG         0x1B
#define     ACCEL_CONFIG        0x1C
#define     FF_THR              0x1D
#define     FF_DUR              0x1E
#define     MOT_THR             0x1F
#define     MOT_DUR             0x20
#define     ZRMOT_THR           0x21
#define     ZRMOT_DUR           0x22
#define     FIFO_EN             0x23
#define     I2C_MST_CTRL        0x24
#define     I2C_SLV0_ADDR       0x25
#define     I2C_SLV0_REG        0x26
#define     I2C_SLV0_CTRL       0x27
#define     I2C_SLV1_ADDR       0x28
#define     I2C_SLV1_REG        0x29
#define     I2C_SLV1_CTRL       0x2A
#define     I2C_SLV2_ADDR       0x2B
#define     I2C_SLV2_REG        0x2C
#define     I2C_SLV2_CTRL       0x2D
#define     I2C_SLV3_ADDR       0x2E
#define     I2C_SLV3_REG        0x2F
#define     I2C_SLV3_CTRL       0x30
#define     I2C_SLV4_ADDR       0x31
#define     I2C_SLV4_REG        0x32
#define     I2C_SLV4_DO         0x33
#define     I2C_SLV4_CTRL       0x34
#define     I2C_SLV4_DI         0x35
#define     I2C_MST_STATUS      0x36
#define     INT_PIN_CFG         0x37
#define     INT_ENABLE          0x38
#define     INT_STATUS          0x3A
#define     ACCEL_XOUT_H        0x3B
#define     ACCEL_XOUT_L        0x3C
#define     ACCEL_YOUT_H        0x3D
#define     ACCEL_YOUT_L        0x3E
#define     ACCEL_ZOUT_H        0x3F
#define     ACCEL_ZOUT_L        0x40
#define     TEMP_OUT_H          0x41
#define     TEMP_OUT_L          0x42
#define     GYRO_XOUT_H         0x43
#define     GYRO_XOUT_L         0x44
#define     GYRO_YOUT_H         0x45
#define     GYRO_YOUT_L         0x46
#define     GYRO_ZOUT_H         0x47
#define     GYRO_ZOUT_L         0x48
#define     EXT_SENS_DATA_00    0x49
#define     EXT_SENS_DATA_01    0x4A
#define     EXT_SENS_DATA_02    0x4B
#define     EXT_SENS_DATA_03    0x4C
#define     EXT_SENS_DATA_04    0x4D
#define     EXT_SENS_DATA_05    0x4E
#define     EXT_SENS_DATA_06    0x4F
#define     EXT_SENS_DATA_07    0x50
#define     EXT_SENS_DATA_08    0x51
#define     EXT_SENS_DATA_09    0x52
#define     EXT_SENS_DATA_10    0x53
#define     EXT_SENS_DATA_11    0x54
#define     EXT_SENS_DATA_12    0x55
#define     EXT_SENS_DATA_13    0x56
#define     EXT_SENS_DATA_14    0x57
#define     EXT_SENS_DATA_15    0x58
#define     EXT_SENS_DATA_16    0x59
#define     EXT_SENS_DATA_17    0x5A
#define     EXT_SENS_DATA_18    0x5B
#define     EXT_SENS_DATA_19    0x5C
#define     EXT_SENS_DATA_20    0x5D
#define     EXT_SENS_DATA_21    0x5E
#define     EXT_SENS_DATA_22    0x5F
#define     EXT_SENS_DATA_23    0x60
#define     MOT_DETECT_STATUS   0x61
#define     I2C_SLV0_DO         0x63
#define     I2C_SLV1_DO         0x64
#define     I2C_SLV2_DO         0x65
#define     I2C_SLV3_DO         0x66
#define     I2C_MST_DELAY_CTRL  0x67
#define     SIGNAL_PATH_RESET   0x68
#define     MOT_DETECT_CTRL     0x69
#define     USER_CTRL           0x6A
#define     PWR_MGMT_1          0x6B
#define     PWR_MGMT_2          0x6C
#define     FIFO_COUNTH         0x72
#define     FIFO_COUNTL         0x73
#define     FIFO_R_W            0x74
#define     WHO_AM_I            0x75

#define DEFAULT_MPU_HZ  100
#define Q30  1073741824.0f

int MPU6050_DMP_Init(void);
int MPU6050_DMP_GetData(float *pitch, float *roll, float *yaw);

#endif

