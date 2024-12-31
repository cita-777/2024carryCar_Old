#include "action.h"
#include "jetson.h" // 添加头文件引用以访问coord_array

#define distance_with_sehuan 130

static uint8_t put_to_map_state = 0;
static uint8_t is_send_ok = 0;

void Action_Catch_And_Put_Obj(uint8_t position)
{
  Crawl_Open();
  HuaGui_Get_From_Map(1000, 230);
  Delay_ms(2000);

  Crawl_Close();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);

  // 根据position选择相应的定位函数
  switch (position)
  {
  case 1:
    Location_First();
    break;
  case 2:
    Location_Second();
    break;
  case 3:
    Location_Third();
    break;
  default:
    break;
  }
  Delay_ms(2000);

  HuaGui_UP(1000, 230);
  Backward();
  Delay_ms(3000);

  HuaGui_ZhuanPan(1000, 230);
  Delay_ms(1000);

  Crawl_Open();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);

  Forward();
  Delay_ms(3000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);
  running_flag++;
}

uint8_t Action_Put_Obj_To_Map(uint8_t position)
{
  Backward();
  // 根据position选择相应的定位函数
  switch (position)
  {
  case 1:
    Location_First();
    break;
  case 2:
    Location_Second();
    break;
  case 3:
    Location_Third();
    break;
  default:
    break;
  }
  Delay_ms(2000);

  HuaGui_ZhuanPan(1000, 230);
  Delay_ms(2000);
  HuaGui_Fang_To_Map(1000, 230);
  Delay_ms(2000);
}
uint8_t Action_Put_Obj_To_Map_321(void)
{
  // 只有当Action_Counter_Enable为0时才执行状态切换
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 2:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 3: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 5: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 6:
      // Jetson_Send(0x05);
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // 返回0表示动作尚未完成
  return 0;
}

uint8_t Action_Put_Obj_To_Map_312(void)
{
  // 只有当Action_Counter_Enable为0时才执行状态切换
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      printf("t8.txt=\"sd1\"\xff\xff\xff");
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 2:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 3: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 5: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 6:
      // Jetson_Send(0x05);
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // 返回0表示动作尚未完成
  return 0;
}
uint8_t Action_Put_Obj_To_Map_123(void)
{
  // 只有当Action_Counter_Enable为0时才执行状态切换
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      printf("t8.txt=\"sd1\"\xff\xff\xff");
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 2:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 3: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 5: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 6:
      // Jetson_Send(0x05);
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // 返回0表示动作尚未完成
  return 0;
}
uint8_t Action_Put_Obj_To_Map_132(void)
{
  // 只有当Action_Counter_Enable为0时才执行状态切换
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      printf("t8.txt=\"sd1\"\xff\xff\xff");
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 2:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 3: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 5: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 6:
      // Jetson_Send(0x05);
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // 返回0表示动作尚未完成
  return 0;
}
uint8_t Action_Put_Obj_To_Map_213(void)
{
  // 只有当Action_Counter_Enable为0时才执行状态切换
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {

    case 0: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 1:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 2: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

    case 3:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 4: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 5:
      // Jetson_Send(0x05);
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // 返回0表示动作尚未完成
  return 0;
}

uint8_t Action_Put_Obj_To_Map_231(void)
{
  // 只有当Action_Counter_Enable为0时才执行状态切换
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {

    case 0: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 1:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 2: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

    case 3:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 4: // 微调
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // 根据coord_array的值进行微调
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X方向微调
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        put_to_map_state++;
      }

      break;

    case 5:
      // Jetson_Send(0x05);
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // 返回0表示动作尚未完成
  return 0;
}