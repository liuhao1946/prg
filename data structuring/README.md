# 数据结构化

假设有一个协议，如下表所述：
|数据头(1byte)|命令(1byte)|数据长度(1byte)|数据(n bytes)|校验(1 byte)|
|-|-|-|-|-|
|字节1|字节2|字节3|字节4~字节n|1~n字节的校验|

下面是一个符合上述协议的具体例子。
|数据头(1byte)|命令(1byte)|数据长度(1byte)|数据1|数据2|校验(1 byte)|
|-|-|-|-|-|-|
|0xAA|0x01|2|0x01|0x02|数据头~数据2的校验和|

假如你需要通过串口或者其他的通道向另一个目标发送符合这个协议的数据，有两种方法可以实现。

## 一般方法

根据协议的内容定义一个一维数组。

```c
uint8_t arr[6]; 

arr[0] = 0xaa; //数据头
arr[1] = 0x01; //命令
arr[2] = 2;    //数据长度
arr[3] = 0x01; //数据1
arr[4] = 0x02; //数据2
arr[5] = sum(arr, sizeof(arr)-1); //前5个字节的校验 

//将数据发送到目标
send(arr, sizeof(arr));
```

这种方法实现的协议有两个问题：
* 可读性不好。
* 当协议要扩展时，比如在命令与数据长度之间增加一个字段，后面数组索引都要变化。

下面是一种更好的方法。

## 更好的方法

```c
typedef struct
{
    uint8_t head;
    uint8_t cmd;
    uint8_t len;
    uint8_t data[2];
    uint8_t sum;    
}exa_t;

exa_t exa;

exa.head = 0xaa;
exa.cmd = 0x01;
exa.len = 2;
exa.data[0] = 0x01;
exa.data[1] = 0x02;
exa.sum = sum(&exa, sizeof(exa_t)-1);

//将数据发送到目标
send(&exa.head, sizeof(exa_t));
```

可见，这种实现方法的可读性比第一种方法好，当要在cmd与len间扩展一个字节时也方便实现。

```c
typedef struct
{
    uint8_t head;
    uint8_t cmd;
    uint8_t ctr; //增加的一个字段
    uint8_t len;
    uint8_t data[2];
    uint8_t sum;    
}exa_t;

exa_t exa;

exa.ctr = 0x01; //增加一个字段，其余字段原封不动
exa.head = 0xaa;
exa.cmd = 0x01;
exa.len = 2;
exa.data[0] = 0x01;
exa.data[1] = 0x02;
exa.sum = sum(&exa, sizeof(exa_t)-1);

//将数据发送到目标
send(&exa.head, sizeof(exa_t));
```

**Note:**
1. 使用第二种方法要注意结构体的内存地址对齐问题（地址不对齐会造成结构体所占用内存比实际定义的要多）。不用考虑内存对齐的简单方法是将结构体内所有字段都定义为**uint8_t**类型，这样以后协议扩展后也不用考虑地址对齐问题。

2. 结构体内的字段顺序必须与协议字段顺序一致。