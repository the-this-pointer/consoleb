#ifndef CONSOLEB_CONSOLEB_USER_H
#define CONSOLEB_CONSOLEB_USER_H

#include <consoleb.h>

// Ui Callbacks
enum Pages {
  MainPageIdx, SettingPageIdx, OtherPageIdx,
  MaxPages
};

void UserInterface_InitPages(UiHandle* uih);
void UserInterface_ShowPopup(UiHandle* uih, const char* text, uint8_t secondsToShow, UiPagePtr fallbackPage);
/* Main Page */
uint8_t mainPageUpdateCallback(void* uih, uint32_t since);
void mainPageInputCallback(void* uih, enum ActionType action);
/* Settings */
void settingsPageInputCallback(void* uih, enum ActionType action);
void setTimeMenuCallback();
void setSleepTimeMenuCallback();
void sendTimesMenuCallback();
/* Other page */
void otherPageInputCallback(void* uih, enum ActionType action);

#endif //CONSOLEB_CONSOLEB_USER_H
