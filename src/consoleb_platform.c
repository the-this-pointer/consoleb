#include <consoleb.h>

#if defined(_WIN32)
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <sys/timeb.h>
#include <pthread.h>

pthread_t g_platform_thread_id;

void* consoleb_ui_thread_func(void* arg)
{
/* Infinite loop */
  struct timeb last_update, now;
  ftime(&now);
  ftime(&last_update);

  uint8_t user_input;
  for(;;)
  {
    user_input = consoleb_platform_handle_inputs();
    if (user_input == KeyPower)
      break;
    else if( user_input > 0 )
    {
      consoleb_handle_input(&uih, user_input);
    }
    ftime(&now);

    int diff = (int) (1000.0 * (now.time - last_update.time)
                             + (now.millitm - last_update.millitm));
    if (consoleb_update(&uih, diff))
      ftime(&last_update);

    consoleb_flush(&uih);
    consoleb_platform_delay(100);
  }
  pthread_exit(NULL);
}

void consoleb_platform_init()
{
  pthread_create(&g_platform_thread_id, NULL, consoleb_ui_thread_func, NULL);
}

void consoleb_platform_uninit()
{
  pthread_join(g_platform_thread_id, NULL);
}

void consoleb_platform_clear()
{
  system("cls");
}

void consoleb_platform_setxy(uint16_t x, uint16_t y)
{
  x = x * 20 / 128 + 2;
  y = y * 5/ 64 + 2;
  printf("%c[%d;%df",0x1B,y,x);
}

void consoleb_platform_write(const char* str)
{
  printf("%s", str);
}

void consoleb_platform_write_char(char chr)
{
  printf("%c", chr);
}

void consoleb_platform_update()
{}

void consoleb_platform_delay(uint16_t ms)
{
  Sleep(ms);
}

void consoleb_platform_lock()
{}

void consoleb_platform_unlock()
{}

void consoleb_platform_update_screen()
{}

char consoleb_platform_handle_inputs()
{
  if(GetKeyState('A') & 0x8000)
    return Key1;
  if(GetKeyState('S') & 0x8000)
    return Key2;
  if(GetKeyState('D') & 0x8000)
    return Key3;
  if(GetKeyState('F') & 0x8000)
    return Key4;
  if(GetKeyState('G') & 0x8000)
    return KeyPower;

  return 0;
}

void consoleb_platform_before_draw_action()
{
  consoleb_platform_write("\r\n");
}

void consoleb_platform_draw_action(char action)
{
  consoleb_platform_write_char(action);
}

#endif