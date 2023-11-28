# 协议说明
校验计算方式: 类型字段和数据字段之和

## 开始或暂停
APP下发:
| 包头  | 方向  | 令牌  | 长度  | 类型  |        数据        | 校验  |
| :---: | :---: | :---: | :---: | :---: | :----------------: | :---: |
| 0xae  | 0x01  | 0xf0  | 0x02  | 0x01  | [0]: 0-暂停;1-开始; | 校验  |

设备回复
| 包头  | 方向  | 令牌  | 长度  | 类型  |        数据        | 校验  |
| :---: | :---: | :---: | :---: | :---: | :----------------: | :---: |
| 0xae  | 0x02  | 0xf0  | 0x02  | 0x01  | [0]: 0-暂停;1-开始;2-电池电量低;3-接入外部电源 | 校验  |

```
开启输出,APP下发内容:
ae 01 f0 02 01 01 02
设备收到后,回复内容:
ae 02 f0 02 01 01 02

如果电池电量低,回复内容:
ae 02 f0 02 01 02 03  
如果接入外部电源,回复内容:
ae 02 f0 02 01 03 04 
```
```
暂停输出,APP下发内容:
ae 01 f0 02 01 00 01
设备收到后,回复内容:
ae 02 f0 02 01 00 01 
```

## 紧急停止
APP下发:
| 包头  | 方向  | 令牌  | 长度  | 类型  | 数据  | 校验  |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0xae  | 0x01  | 0xf0  | 0x01  | 0x03  |  无   | 校验  |

设备回复
| 包头  | 方向  | 令牌  | 长度  | 类型  | 数据  | 校验  |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0xae  | 0x02  | 0xf0  | 0x01  | 0x03  |  无   | 校验  |

```
紧急停止,APP下发内容:
ae 01 f0 01 03 03
设备收到后,回复内容:
ae 02 f0 01 03 03 
```

## 设置输出
APP下发:
| 包头  | 方向  | 令牌  | 长度  | 类型  |                                                                                      数据                                                                                       | 校验  |
| :---: | :---: | :---: | :---: | :---: | :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :---: |
| 0xae  | 0x01  | 0xf0  | 0x09  | 0x04  | [0]:手臂电机强度<br> [1]:手臂电机频率<br> [2]:胸部电机强度<br> [3]:胸部电机频率<br> [4]:背面上电机强度<br> [5]:背面上电机频率<br> [6]:背面下电机强度<br> [7]:背面下电机频率<br> | 校验  |

设备回复
| 包头  | 方向  | 令牌  | 长度  | 类型  |                                                                                      数据                                                                                       | 校验  |
| :---: | :---: | :---: | :---: | :---: | :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :---: |
| 0xae  | 0x02  | 0xf0  | 0x09  | 0x04  | [0]:手臂电机强度<br> [1]:手臂电机频率<br> [2]:胸部电机强度<br> [3]:胸部电机频率<br> [4]:背面上电机强度<br> [5]:背面上电机频率<br> [6]:背面下电机强度<br> [7]:背面下电机频率<br> | 校验  |

```
举例
设置 手臂强度10 频率6; 胸部强度20 频率7; 背面上强度30 频率8; 背面下强度40 频率9; APP下发内容:
ae 01 f0 09 04 0a 06 14 07 1e 08 28 09 86
设备收到后,回复内容:
ae 02 f0 09 04 0a 06 14 07 1e 08 28 09 86 
```

## 电池上报
设备间隔2秒上报电量和电池状态
| 包头  | 方向  | 令牌  | 长度  | 类型  | 数据  | 校验  |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0xae  | 0x02  | 0xf0  | 0x03  | 0x05  |  [0]: 电量百分比<br> [1]:电池状态   | 校验  |

电池状态定义如下:
```
1.电量充足
2.电量低
3.接入外部电源
```

```
举例,电量100,未接入外部电源,设备上报内容如下:
ae 02 f0 03 05 64 01 6a 
```
