# 世界最安級 15円 ARMマイコン PY32F002A 評価F/W開発

1石15円の世界最安級ARMマイコン PY32F002Aの評価F/W個人開発リポジトリ

<div align="center">
  <img width="200" src="/doc/IMG_20250823_134712.jpg">
</div>

## 開発環境

### H/W

- 評価基板
  - (TBD)
  - ※基板は作ってるやからそれまでブレッドボードwww

- マイコン ... 型番 : [PY32F002AF15P6](hhttps://www.py32.org/en/mcu/PY32F002Axx.html)🔗
  - CPU ... ARM Cortex-M0+
  - ROM ... 20KB
  - RAM ... 3KB
  - Clock ... 24MHz
  - GPIO ... 18本
  - デバッグI/F ... SWD
  - DMA ... (N/A)
  - タイマー
    - TIM1 ... 16bit高機能タイマー
    - TIM16 ... 16bit汎用タイマー
    - LPTM ... 16bit低電力タイマー
  - WDT ... IWDG x1ch
  - SysTick
  - I2C ... x1ch
  - SPI ... x1ch
  - USART ... x1ch
  - ADC ... 12bit x12ch
  - コンパレータ ... x2本

### S/W

- IDE
  - [ARM μVison V5.43.1.0](https://www.mounriver.com/download)🔗
- SDK
  - [PUYA純正のLLドライバ](https://github.com/OpenPuya/PY32F0xx_Firmware)🔗
- コンパイラ
  - ArmClang V6.24
- 最適化
  - -O0 or -Os

<div align="center">
  <img width="500 " src="/doc/uvison5_py32f002a_yool_version.png">
</div>

### デバッガ

- [WCH-LinkE Ver1.3](https://akizukidenshi.com/catalog/g/g118065)🔗
  - 1). CMSIS-DAPにするためにARMモードで起動！
  - 2). デバッガの青と赤色のLEDが点灯してること！
  - 3).デバッグはJTAGではなくSWDであること！

SWD

- [WCH-LinkE <--> PY32F002A]
  - SWDIO <--> PD13ピン (IC 2番目のピン)
  - SWCLK <--> PA14ピン (IC 3番目のピン)
  - GND <--> GND

 UART

- [WCH-LinkE <--> PY32F002A]
  - RX <--> PA2ピン(TX)　(IC 13番目のピン)
  - TX <--> PA3ピン(RX)　(IC 14番目のピン)
  - GND <--> GND

<div align="center">
  <img width="400" src="/doc/py32f002a_pin_info.png">
</div>