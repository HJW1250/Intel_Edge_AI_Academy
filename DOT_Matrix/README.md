# Project name
STM32를 사용한 DotMatrix제어

# Project Purpose
- STM32 구조와 Datasheet 해석 능력 향샹
- STM32CubeIDE 툴 사용능력 향상
- GPIO 및 Dot Matrix 제어방법 학습

# Requirement

## Tool
STM32CubeIDE 1.12.1

## Kit 
HBE-MCU-Multi II M4

## Chipset 
Main Chipset : CortaxM4 - STM32F405RGTx
sub Chipset : LD1071


# What is Dot Matrix?
- 도트매트릭스 LED의 형태에서 5x7형은 주로 글씨를 표시하기 위한 용도로
주로 사용됨
- 8x8, 16x16은 주로 이미지를 표시하기 위해 주로 사용됨
- 다른 Dot Matrix와의 차별점은 3Color이다.

# What is LD1070?
- LD1071은 14비트 PWM을 내장하고 있는 16채널 LED 드라이버
- 3Color Dot Matrix 구동 IC로 널리 사용

# Import Datasheet
### LD1071 IC PIN NO.
![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/f387bc67-631f-4eb2-9d43-b7eb18b43815)

### LD1071 IC Flowchart and command Code
![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/23c4463b-14a5-4bd9-9375-919b6df0a9ed)

![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/f2b34d73-4a6b-46e9-a13c-9006e9fb0b6e)

![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/5bd077e1-713a-4363-b127-f6f81603d98a)

![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/b86f37d7-b584-4e2a-8743-74dbe835dafc)

![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/b7b6cbfc-0cd9-457e-b448-26eda3ec6a54)

# Improvement
모두 개발하고 보니 Dot Matrix가 좌우 반전이 되어서 출력되었다.

비트반전코드 함수를 만들어 보완.

```uint16_t reverseBits(uint16_t num)
{
   int NO_OF_BITS = sizeof(num) * 8;
   uint16_t reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}
```

# Result
![image](https://github.com/HJW1250/Intel_Edge_AI_Academy/assets/114561883/49ef2f57-5bd4-43cb-90f9-a8b81a9a44ce)

(폴더참조)
