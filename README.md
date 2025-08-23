# 15円 ARMマイコン PY32F002A 評価F/W開発

1石15円の世界最安級?なARMマイコン PY32F002Aの評価F/W個人開発リポジトリ

> [!CAUTION]
> 【免責】<br> 本実装の利用は本免責に同意したものとみなし、<br>いかなる損害や問題、事故が発生してもすべて自己責任として、<br>私はその責任を一切の負いかねます。知らんがな。

<div align="center">
  <img src="/doc/py32f002a_bread_board.png">
</div>

## TIP

> [!TIP]
> [PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗 a.k.a [PY32F030K16T6](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗というが判明www

- マイコン ... 型番 : [PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗 a.k.a [PY32F030K16T6](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗

| 項目 | PUYA公表スペック<br>[PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗 | 実際のスペック<br>(a.k.a [PY32F030K16T6](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗) |
| ---- | ---- | ---- |
| CPU | ARM Cortex-M0+ | ARM Cortex-M0+ |
| ROM | 20KB | **32KB** |
| RAM | 3KB | **4KB** |
| Clock | 24MHz(PLLなし) | **48MHz(PLLで逓倍)** |
| GPIO | x18本 | x18本 |
| DMA | なし | **x3本** |
| タイマー | 16bit高機能タイマー TIM1<br> 16bit汎用タイマー TIM3<br>低電力タイマー LPTIM | 16bit高機能タイマー TIM1<br> 16bit汎用タイマー TIM3,**TIM14,TIM16,TIM17**<br>低電力タイマー LPTIM |
| I2C | x1本 | x1本 |
| SPI | x1本 | **x2本** |
| UART | x1本 | **x2本** |
| ADC | 12bit x12本 | 12bit x12本 |
| コンパレータ | x2本 | x2本 |

## 開発環境

### H/W

- 評価基板
  - (TBD)
  - ※基板はまだ作ってるからそれまでブレッドボードでwww

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