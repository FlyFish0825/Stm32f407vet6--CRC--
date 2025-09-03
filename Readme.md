# 简介
## 1. 测试CRC校验进行IAP接受的数据包的完整性

**测试代码**
~~~c
  uint32_t data[2] = {0x12345678, 0x87654321};
  volatile uint32_t crc = 0;
  //适用于数据量不大，一次性处理完
  crc = HAL_CRC_Calculate(&hcrc, data, 2);
  printf("CRC Result: %08X\n", crc); //0xC15A147D


  //重置CRC的值
  crc = 0;
  
  
  //分两次进行CRC校验，适用于数据量大分批次传输进行校验
  crc = HAL_CRC_Calculate(&hcrc, data, 1);
  printf("CRC Temp  Result: %08X\n")  //0xDF8A8A2B
  crc = HAL_CRC_Accumulate(&hcrc, data + 1, 1);
  printf("CRC   Accumulate  Result: %08X\n", crc); //0xC15A147D
~~~
* 首先对数据data[2],两个32位的数据进行CRC校验，得到crc = 0xC15A147D。  
       关键函数：  
**HAL_CRC_Calculate(&hcrc, data, 2);**
这段代码实现了CRC（循环冗余校验）的累加计算功能。具体分析如下  
1. 函数功能：
- HAL_CRC_Accumulate函数用于计算32位数据缓冲区的CRC值
- 特点是使用之前计算的CRC值作为初始值进行新的计算
- 返回32位的CRC计算结果（对于小于32位的CRC，返回最低效位）
2. 参数说明：
- **hcrc**：CRC句柄指针，包含CRC外设的配置信息
- **pBuffer**：指向输入数据缓冲区的指针
- **BufferLength**：输入数据缓冲区的长度（以32位字为位）

* 然后重置crc的值，再对data[2]进行分两次进行CRC校验，得到crc = 0xC15A147D。

**HAL_CRC_Accumulate(&hcrc, data + 1, 1)**

这段代码实现了CRC（循环冗余校验）的独立计算功能。具体分析如下  

1. 函数功能：
- HAL_CRC_Calculate函数用于计算32位数据缓冲区的CRC值
- 特点是使用预设的初始值（hcrc->Instance->INIT）开计算
- 返回32位的CRC计算结果（对于小于32位的CRC，返回最低效位）

2. 参数说明：
- hcrc：CRC句柄指针，包含CRC外设的配置信息
- pBuffer：指向输入数据缓冲区的指针
- BufferLength：输入数据缓冲区的长度（以32位字为单位）

3. 实现步骤：
- 首先将CRC外设状态设置为BUSY
- 重置CRC计算单元（将INIT值写入DR寄存器）
- 通过循环将数据缓冲区中的每个32位字输入到CRC计算器
- 读取最终的CRC计算结果
- 将CRC外设状态恢复为READY
- 返回计算得到的CRC值

4. 与HAL_CRC_Accumulate的<font color="red">关键区别</font>：
- HAL_CRC_Calculate每次都会重置CRC计算单元，使用预的初始值
- 而HAL_CRC_Accumulate则是在之前计算结果的基础上继续算

