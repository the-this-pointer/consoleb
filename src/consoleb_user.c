#include <consoleb_user.h>
#include <malloc.h>
#include <time.h>
#include <stdio.h>

void UserInterface_InitPages(UiHandle* uih)
{
  uih->pages = malloc(sizeof(UiPage) * MaxPages);
  if (!uih->pages)
    return;

  /* Init Main Page */
  uih->pages[MainPageIdx].text = "Main Page";
  uih->pages[MainPageIdx].menu = NULL;
  uih->pages[MainPageIdx].actionIcons = "&  )";
  uih->pages[MainPageIdx].onInit = NULL;
  uih->pages[MainPageIdx].onUpdate = mainPageUpdateCallback;
  uih->pages[MainPageIdx].onLeave = NULL;
  uih->pages[MainPageIdx].onHandleInput = mainPageInputCallback;
  uih->pages[MainPageIdx].data = NULL;

  /* Init Settings Page And Menus */
  enum SettingMenuTypes {
    MNU_SET_TIME = 0x00,
    MNU_SLP_TIME,
    MNU_SND_TIMES,
    MNU_CNT
  };
  UiMenuPtr settingMenus = (UiMenuPtr)malloc(sizeof(UiMenu) * MNU_CNT);
  if (!settingMenus)
    return;
  consoleb_init_menu(&settingMenus[MNU_SET_TIME], "Time", 					NULL, &settingMenus[MNU_SLP_TIME], NULL, &uih->pages[SettingPageIdx], NULL, NULL, setTimeMenuCallback);
  consoleb_init_menu(&settingMenus[MNU_SLP_TIME], "Sleep Time", 		&settingMenus[MNU_SET_TIME], &settingMenus[MNU_SND_TIMES], NULL, &uih->pages[SettingPageIdx], NULL, NULL, setSleepTimeMenuCallback);
  consoleb_init_menu(&settingMenus[MNU_SND_TIMES],"Send Times", 		&settingMenus[MNU_SLP_TIME], NULL, NULL, &uih->pages[SettingPageIdx], NULL, NULL, sendTimesMenuCallback);

  uih->pages[SettingPageIdx].text = "Settings";
  uih->pages[SettingPageIdx].menu = settingMenus;
  uih->pages[SettingPageIdx].actionIcons = NULL;
  uih->pages[SettingPageIdx].onInit = NULL;
  uih->pages[SettingPageIdx].onUpdate = NULL;
  uih->pages[SettingPageIdx].onLeave = NULL;
  uih->pages[SettingPageIdx].onHandleInput = settingsPageInputCallback;
  uih->pages[SettingPageIdx].data = NULL;

  uih->pages[OtherPageIdx].text = "Other";
  uih->pages[OtherPageIdx].menu = NULL;
  uih->pages[OtherPageIdx].actionIcons = NULL;
  uih->pages[OtherPageIdx].onInit = NULL;
  uih->pages[OtherPageIdx].onUpdate = NULL;
  uih->pages[OtherPageIdx].onLeave = NULL;
  uih->pages[OtherPageIdx].onHandleInput = otherPageInputCallback;
  uih->pages[OtherPageIdx].data = NULL;

  consoleb_change_page(uih, &uih->pages[MainPageIdx]);
}


uint8_t mainPageUpdateCallback(void* uih, uint32_t since)
{
  if (since < 400)
    return 0;

  UiHandle* hnd = uih;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  static int counter;
  counter++;
  char timeBuff[16] = {0};
  sprintf(timeBuff, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

  char dateBuff[8] = {0};
  sprintf(dateBuff, "%02d/%02d", tm.tm_mon, tm.tm_mday);

  consoleb_platform_clear();
  consoleb_platform_setxy(16, 5);
  consoleb_platform_write(timeBuff);

  consoleb_platform_setxy(36, 35);
  consoleb_platform_write(dateBuff);

  consoleb_p_draw_actions(hnd->currentPage->actionIcons);

  SCR_DIRTY_ARG(hnd);
  return 1;
}

void mainPageInputCallback(void* uih, enum ActionType action)
{
  switch(action)
  {
    case Key1:
      consoleb_change_page(uih, &((UiHandle*)uih)->pages[SettingPageIdx]);
      break;
    case Key4:
      consoleb_change_page(uih, &((UiHandle*)uih)->pages[OtherPageIdx]);
      break;
    default:
      break;
  }
}

void settingsPageInputCallback(void* uih, enum ActionType action)
{
  switch(action)
  {
    case Key1:
      consoleb_change_page(uih, &((UiHandle*)uih)->pages[MainPageIdx]);
      break;
    default:
      break;
  }
}

void setTimeMenuCallback()
{
  ;
}

void setSleepTimeMenuCallback()
{
  ;
}

void sendTimesMenuCallback()
{
  ;
}

void otherPageInputCallback(void* uih, enum ActionType action)
{
  switch(action)
  {
    case Key1:
      consoleb_change_page(uih, &((UiHandle*)uih)->pages[MainPageIdx]);
      break;
    default:
      break;
  }
}
