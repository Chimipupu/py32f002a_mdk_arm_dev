# 1石12円 ARMマイコン PY32F002A 評価F/W開発

1石12円の世界最安級?なARMマイコン PY32F002Aの評価F/W個人開発リポジトリ

<div align="center">
  <img src="/doc/py32f002a_bread_board.png">
</div>

## 逆スペック詐欺

> [!CAUTION]
> ⚠️この実装では、PY32F002Aに未搭載のはずの機能の多くが実行、動作している(※)⚠️<br>
> ※[PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗はシリコンがROM32KB,RAM4KBの[PY32F030](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗の可能性がある<br>
> ※PY32F002Aにはないはずの下記機能の実行と動作を確認<br>
> ➡️ PLLでクロックを48MHzにできる<br>
> ➡️ DMAも動く<br>
> ➡️ RTCも動く<br>

| 項目 | PUYA公表スペック<br>[PY32F002AF15P6](https://www.py32.org/en/mcu/PY32F002Axx.html)🔗 | 実際のスペック<br>(a.k.a [PY32F030](https://www.py32.org/en/mcu/PY32F030xx.html#introduction)🔗) |
| ---- | ---- | ---- |
| CPU | ARM Cortex-M0+ | ARM Cortex-M0+ |
| ROM | 20KB | **32KB** |
| RAM | 3KB | **4KB** |
| Clock | 24MHz(PLLなし) | **48MHz(PLLで逓倍)** |
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