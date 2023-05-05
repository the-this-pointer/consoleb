#ifndef CONSOLEB_CONSOLEB_H
#define CONSOLEB_CONSOLEB_H

#include <stdint.h>

#define SCR_DIRTY_ARG(uih) uih->screenDirty = 1
#define SCR_DIRTY uih->screenDirty = 1
#define SCR_DIRTY_CLR uih->screenDirty = 0

enum ActionType {Key1 = 1, Key2, Key3, Key4, KeyPower};

typedef void(*pageLifeCycleCallback)(void* uih);
typedef void(*pageInputCallback)(void* uih, enum ActionType action);
typedef uint8_t(*pageUpdateCallback)(void* uih, uint32_t since);
typedef void(*menuCallback)();

typedef struct {
  const char* caption;
  void* prev;
  void* next;
  void* parent;
  void* children;

  void* parentPage;
  void* page;

  menuCallback callback;
} UiMenu, *UiMenuPtr;

typedef struct {
  const char* text;
  UiMenuPtr	menu;
  const char* actionIcons;

  pageLifeCycleCallback onInit;
  pageLifeCycleCallback onLeave;
  pageUpdateCallback onUpdate;
  pageInputCallback onHandleInput;

  void* data;
} UiPage, *UiPagePtr;

typedef struct {
  UiPagePtr pages;
  UiPagePtr currentPage;
  UiMenuPtr currentMenu;
  uint8_t screenDirty;
  uint8_t screenStatus;
} UiHandle;

void consoleb_platform_init();
void consoleb_platform_uninit();
void consoleb_platform_clear();
void consoleb_platform_setxy(uint16_t x, uint16_t y);
void consoleb_platform_write(const char* str);
void consoleb_platform_write_char(char chr);
void consoleb_platform_update();
void consoleb_platform_delay(uint16_t ms);
void consoleb_platform_lock();
void consoleb_platform_unlock();
void consoleb_platform_update_screen();
char consoleb_platform_handle_inputs();
void consoleb_platform_before_draw_action();
void consoleb_platform_draw_action(char action);

void consoleb_init();
void consoleb_uninit();
void consoleb_change_page(UiHandle* uih, UiPagePtr page);
void consoleb_handle_input(UiHandle* uih, uint16_t input);
void consoleb_init_menu(UiMenuPtr menu, const char* caption, void* prev, void* next, void* parent, void* parentPage, void* page, void* children, menuCallback callback);
uint8_t consoleb_update(UiHandle* uih, uint32_t since);
void consoleb_flush(UiHandle* uih);

void consoleb_turn_on_screen(UiHandle* uih);
void consoleb_turn_off_screen(UiHandle* uih);
uint8_t consoleb_screen_is_on(UiHandle* uih);

void consoleb_p_draw_menu(UiHandle* uih);
void consoleb_p_on_handle_menu_input(UiHandle* uih, enum ActionType action);
void consoleb_p_draw_actions(const char* actions);

extern UiHandle uih;

#endif //CONSOLEB_CONSOLEB_H
