#include <FastLED.h>
#include "expression.h"   // 表情データのヘッダーファイル

/* PINの定義 */
const int sw1_pin = 2;     // ①ボタン用　デジタルピン2に接続
const int sw2_pin = 4;     // ②ボタン用　デジタルピン4に接続
const int sw3_pin = 6;     // ③ボタン用　デジタルピン6に接続
const int sw4_pin = 8;     // ④ボタン用　デジタルピン8に接続
const int data_pin = 10;   // データ用　　デジタルピン10に接続

/* LED用の定義 */
#define LED 152     // LEDの総数
#define COLOR 12   // 色の種類
CRGB led[LED];
CRGB black = CRGB(0, 0, 0);     // 消灯用
CRGB red   = CRGB(255, 0, 0);   // ランダムモード用
CRGB blue  = CRGB(0, 0, 255);   // マニュアルモード用

/* 以下は各キャラクター色 */
CRGB ayumu    = CRGB(255, 100, 120);
CRGB kasumi   = CRGB(255, 255, 40);
CRGB shizuku  = CRGB(120, 120, 255);
CRGB karin    = CRGB(0, 0, 255);
CRGB ai       = CRGB(255, 50, 0);
CRGB kanata   = CRGB(255, 80, 255);
CRGB setsuna  = CRGB(255, 0 ,0);
CRGB ema      = CRGB(120, 255, 40);
CRGB rina     = CRGB(255, 0, 120);
CRGB shioriko = CRGB(40, 255, 80);
CRGB taylor   = CRGB(255, 255, 255);
CRGB lanzhu   = CRGB(255, 90, 90);
CRGB now_color = rina;   // 色変更用の変数　初期色はrina色

/* 操作用フラグの変数 */
int brightness = 30;                // 明るさ調整用の変数
int mode = 1;                       // 0:ランダムモード 1:マニュアルモード
int now_preset = 0;                 // プリセット
int now_preset_size = EXPRESSION;   // プリセット数
int now_expression = 0;             // 現在の表情
int color = 9;                      // 色変更用の変数

/* 起動時に1回だけ呼ばれる関数 */
void setup()
{
  FastLED.addLeds<NEOPIXEL, data_pin>(led, LED);
  FastLED.setBrightness(brightness);
  pinMode(sw1_pin, INPUT_PULLUP);
  pinMode(sw2_pin, INPUT_PULLUP);
  pinMode(sw3_pin, INPUT_PULLUP);
  pinMode(sw4_pin, INPUT_PULLUP);
  randomSeed(analogRead(0));   // 未接続ピンのノイズを利用して、乱数を初期化する
}

/* setup関数の処理後、ずっと繰り返し処理される関数 */
void loop()
{
  switch_judge();
  delay(50);
  expression_flag();
  delay(50);
}

/* スイッチを読み取るフラグ */
void switch_judge()
{
  /* sw1の判定 */
  if(digitalRead(sw1_pin) == LOW)
  {
    while (digitalRead(sw1_pin) == LOW)
    {
      // 何も動作しない
    }
    sw1();
  }

  /* sw2の判定 */
  if(digitalRead(sw2_pin) == LOW)
  {
    while (digitalRead(sw2_pin) == LOW)
    {
      // 何も動作しない
    }
    sw2();
  }

  /* sw3の判定 */
  if(digitalRead(sw3_pin) == LOW)
  {
    int time_counter = 0;   // 短押しか長押しかを判定する用のカウンター
    while(digitalRead(sw3_pin) == LOW)
    {
      time_counter++;
      delay(5);
    }
    if(time_counter < 50)
    {
      /* 0.25秒未満は単押し */
      sw3(false);
    }
    else
    {
      /* 0.25秒以上なら長押し */
      sw3(true);
    }
  }

  /* sw4の判定 */
  if(digitalRead(sw4_pin) == LOW)
  {
    int time_counter = 0;   // 短押しか長押しかを判定する用のカウンター
    while(digitalRead(sw4_pin) == LOW)
    {
      time_counter++;
      delay(5);
    }
    if(time_counter < 50)
    {
      /* 0.25秒未満は短押し */
      sw4(false);
    }
    else
    {
      /* 0.25秒以上なら長押し */
      sw4(true);
    }
  }
}

/* フラグを読み取り表情の変更操作 */
void expression_flag()
{
  /* ランダムモード時、表情がランダムで変更する */
  if(mode == 0)
  {
    display_expression(now_preset, random(now_preset_size));   // プリセット内からランダムで選択し表示する
    int time_counter = 0;  // タイムカウンター用
    while(true)
    {
      /* 表示中でもボタン操作を受けつける */
      switch_judge();
      delay(10);
      time_counter++;
      if(time_counter > 100)
      {
        break;
      }
    }
  }

  /* マニュアルモード時、手動で表情を変更する */
  else if(mode == 1)
  {
    display_expression(now_preset, now_expression);
  }
}

/* 表情を表示する */
void display_expression(int all_preset, int expression_num)
{
  /* 選択された表情の要素数と表情配列を受け取る */
  int size = presets_sizelist[all_preset][expression_num];
  int *expression = presets[all_preset][expression_num];

  led_off();   // 全てのLEDをoffにする
  
  /* 表情の配列を読み込み設定した色でLEDを点灯する */
  for(int i = 0; i < size; i++)
  {
    led[expression[i]] = now_color;
  }
  FastLED.show();
}

/* 全てのLEDをoffにする */
void led_off()
{
  for(int i = 0; i < LED; i++)
  {
    led[i] = black;
  }
}

/* SW1の挙動 */
void sw1()
{
  /* ランダムモード時、何もしない */
  if(mode == 0)
  {
    // 何も動作しない
  }

  /* マニュアルモード時、表情を一つすすめる */
  else if(mode == 1)
  {
    now_expression = (now_expression + 1) % now_preset_size;
  }
}

/* SW2の挙動 */
void sw2()
{
  /* ランダムモード時、何もしない */
  if(mode == 0)
  {
    // 何も動作しない
  }

  /* マニュアルモード時、表情を1つ前に戻す */
  else if(mode == 1)
  {
    now_expression--;
    if(now_expression < 0)
    {
      now_expression = now_preset_size - 1;
    }
  }
}

/* SW3の挙動 */
void sw3(bool long_judge)
{
  /* ランダムモード時、何もしない */
  if(mode == 0)
  {
    // 何も動作しない
  }
  
  /* マニュアルモード時、短押しと長押しで動作が異なる */
  else if(mode == 1)
  {
    /* 短押し時、色を昇順で変更する */
    if(!long_judge)
    {
       switch(color)
       {
         case 1: now_color = ayumu;
                 break;
         case 2: now_color = kasumi;
                 break; 
         case 3: now_color = shizuku;
                 break;  
         case 4: now_color = karin;
                 break;
         case 5: now_color = ai;
                 break; 
         case 6: now_color = kanata;
                 break; 
         case 7: now_color = setsuna;
                 break;
         case 8: now_color = ema;
                 break; 
         case 9: now_color = rina;
                 break;  
         case 10: now_color = shioriko;
                 break;
         case 11: now_color = taylor;
                 break; 
         case 12: now_color = lanzhu;
                 break;                   
      }
      color++;
    }
    
    /* 長押し時、色を降順で変更する */
    else if(long_judge)
    {
      color--;
      switch(color)
      {
         case 1: now_color = ayumu;
                 break;
         case 2: now_color = kasumi;
                 break; 
         case 3: now_color = shizuku;
                 break;  
         case 4: now_color = karin;
                 break;
         case 5: now_color = ai;
                 break; 
         case 6: now_color = kanata;
                 break; 
         case 7: now_color = setsuna;
                 break;
         case 8: now_color = ema;
                 break; 
         case 9: now_color = rina;
                 break;  
         case 10: now_color = shioriko;
                 break;
         case 11: now_color = taylor;
                 break; 
         case 12: now_color = lanzhu;
                 break;                   
      }
    }
    
    if(color > 12)
    {
      color = 1;
    }
    else if(color < 1)
    {
      color = 12;
    }
  }
}

/* SW4の挙動 */
void sw4(bool long_judge)
{
  /* 短押し時、明るさを+20にする, 明るさ上限は100 */
  if(!long_judge)
  {
    brightness = (brightness + 20) % 100;
    FastLED.setBrightness(brightness);
  }
  
  /* 長押し時、モードを切り替える */
  else if(long_judge)
  {
    /* モード切り替え */
    mode = !mode;
    mode_led();
  }
}

/* モード変更時の目印用 */
void mode_led()
{
  /* ランダムモードに切り替え時、先頭のLEDが赤く点灯する */
  if(mode == 0)
  {
    led[0] = red;
  }
  
  /* マニュアルモードに切り替え時、先頭のLEDが青く点灯する */
  else if(mode == 1)
  {
    led[0] = blue;
  }
  FastLED.show();
  delay(200);
}
