# 1石12円 ARMマイコン PY32F002A 評価F/W開発

1石12円の世界最安級?なARMマイコン PY32F002Aの評価F/W個人開発リポジトリ

<div align="center">
  <img src="/doc/py32f002a_bread_board.png" hight = 200>
</div>

## 逆スペック詐欺

> [!CAUTION]
> ⚠️このF/Wは、PY32F002Aには未搭載のはずの機能の多くが動作している(※)⚠️<br>
> ※[PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗はシリコンがROM32KB,RAM4KBの[PY32F030](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗の可能性がある<br>
> ➡️ **Flashは16KBのはずが32KBも使用できる**<br>
> ➡️ **SRAMは3KBのはずが4KBも使用できる**<br>
> ➡️ **未搭載のはずのPLLが存在していて、PLLはx2の2逓倍固定だがHSI 24MHzを48MHzにできる**<br>
> ➡️ **未搭載のはずの内蔵RCレゾネータのLSI(32.768KHz)が存在していて、LSIをクロック源に使える**<br>
> ➡️ **未搭載のはずのDMAが存在していて、DMAの転送もIRQ割り込みもできる**<br>
> ➡️ **未搭載のはずのRTCが存在していて、RTCで1秒おきのカウントとIRQ割り込みもできる**<br>

| 項目 | PUYA公表スペック<br>[PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗 | 実際のスペック<br>(a.k.a [PY32F030](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗) |
| ---- | ---- | ---- |
| CPU | ARM Cortex-M0+ | ARM Cortex-M0+ |
| ROM | 20KB | **32KB** |
| RAM | 3KB | **4KB** |
| Clock | 24MHz(PLLなし) | **48MHz<br>PLLはx2の2逓倍固定<br>内蔵RCレゾネータのLSI(32.768KHz)** |
| GPIO | x18本 | x18本 |
| DMA | なし | **x3本** |
| タイマー | 16bit高機能タイマー TIM1<br> 16bit汎用タイマー TIM3<br>低電力タイマー LPTIM | 16bit高機能タイマー TIM1<br> 16bit汎用タイマー TIM3,**TIM14,TIM16,TIM17**<br>低電力タイマー LPTIM |
| RTC | (N/A) | **x1本** |
| I2C | x1本 | x1本 |
| SPI | x1本 | **x2本** |
| UART | x1本 | **x2本** |
| ADC | 12bit x12本 | 12bit x12本 |
| コンパレータ | x2本 | x2本 |

## 開発環境

### H/W

- 評価基板
  - (TBD)
  - ※基板完成まではブレッドボード

### S/W

- IDE
  - [ARM μVison V5.43.1.0](https://www.mounriver.com/download)🔗
- SDK
  - [PUYA純正のLLドライバ](https://github.com/OpenPuya/PY32F0xx_Firmware)🔗
- コンパイラ
  - ArmClang V6.24
- 最適化
  - デバッグ中 ... `-O0` (最適化なし)
  - 最適化あり ... `-Oz` (サイズ優先の最適化)

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
