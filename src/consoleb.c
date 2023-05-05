#include <consoleb.h>
#include <string.h>

UiHandle uih;

uint8_t	consoleb_g_menu_actions_offset = 0;
const char* consoleb_g_menu_action_icons = "%#$!";

#define ACT_OFST(a, b) \
	if (consoleb_g_menu_actions_offset == 0) \
	{ \
		a; \
	} else { \
		b; \
	}

#define ACT_OFST_3(a, b, c) \
	if (consoleb_g_menu_actions_offset == 0) \
	{ \
		a; \
	} else if (consoleb_g_menu_actions_offset == 4) {\
		b; \
	} else { \
		c; \
	}

void consoleb_init()
{
  consoleb_platform_init();

  uih.currentPage = NULL;
  uih.currentMenu = NULL;
  uih.screenDirty = 0;
  uih.screenStatus = 1;
}

void consoleb_uninit()
{
  consoleb_platform_uninit();
}

void consoleb_change_page(UiHandle* uih, UiPagePtr page)
{
  if (uih->currentPage != NULL && uih->currentPage->onLeave)
    uih->currentPage->onLeave(uih);

  uih->currentPage = page;
  consoleb_g_menu_actions_offset = 0;

  if (uih->currentPage->onInit)
    uih->currentPage->onInit(uih);

  if (uih->currentPage->text && uih->currentPage->menu)
  {
    consoleb_platform_clear();
    consoleb_platform_setxy(5, 5);
    consoleb_platform_write(uih->currentPage->text);

    consoleb_platform_update();
    consoleb_platform_delay(1000);
  }

  if (!uih->currentPage->menu)
  {
    uih->currentMenu = NULL;
  }

  consoleb_platform_clear();

  if (uih->currentPage->text && !uih->currentPage->menu)
  {
    consoleb_platform_setxy(5, 5);
    consoleb_platform_write(uih->currentPage->text);
    consoleb_p_draw_actions(uih->currentPage->actionIcons);
  }

  if (uih->currentPage->menu)
  {
    uih->currentMenu = uih->currentPage->menu;
    consoleb_platform_setxy(5, 5);
    consoleb_platform_write(uih->currentMenu->caption);
    consoleb_p_draw_actions(consoleb_g_menu_action_icons);
  }

  SCR_DIRTY;
}

void consoleb_handle_input(UiHandle* uih, uint16_t input)
{
  if (!uih->currentPage)
    return;

  if (uih->currentPage->menu == NULL)
  {
    uih->currentPage->onHandleInput(uih, input);
  }
  else
  {
    consoleb_p_on_handle_menu_input(uih, input);
  }
}

void consoleb_init_menu(UiMenuPtr menu, const char* caption, void* prev, void* next, void* parent, void* parentPage, void* page, void* children, menuCallback callback)
{
  if (!menu)
    return;

  (menu)->caption = caption;
  (menu)->prev = prev;
  (menu)->next = next;
  (menu)->parent = parent;
  (menu)->parentPage = parentPage;
  (menu)->page = page;
  (menu)->children = children;
  (menu)->callback = callback;
}

uint8_t consoleb_update(UiHandle* uih, uint32_t since)
{
  consoleb_platform_lock();
  if (uih->currentPage && uih->currentPage->onUpdate && uih->screenStatus)
  {
    consoleb_platform_unlock();
    return uih->currentPage->onUpdate(uih, since);
  }
  consoleb_platform_unlock();
  return 0;
  // SCR_DIRTY;
}

void consoleb_flush(UiHandle* uih)
{
  consoleb_platform_lock();
  if (!uih->screenDirty || !uih->screenStatus) {
    consoleb_platform_unlock();
    return;
  }
  consoleb_platform_unlock();

  consoleb_platform_update_screen();
  SCR_DIRTY_CLR;
}

void consoleb_turn_on_screen(UiHandle* uih)
{
  consoleb_platform_lock();
  if (uih->screenStatus == 0)
  {
    uih->screenStatus = 1;
    consoleb_change_page(uih, &uih->pages[0]);
  }
  consoleb_platform_unlock();
}

void consoleb_turn_off_screen(UiHandle* uih)
{
  if (uih->screenStatus == 1)
  {
    uih->screenStatus = 0;

    consoleb_platform_clear();
    consoleb_platform_update_screen();
    SCR_DIRTY_CLR;
  }
}

uint8_t consoleb_screen_is_on(UiHandle* uih)
{
  return uih->screenStatus;
}

void consoleb_p_draw_menu(UiHandle* uih)
{
  if (!uih->currentMenu)
    return;
  consoleb_platform_clear();
  consoleb_platform_setxy(5, 5);
  consoleb_platform_write(uih->currentMenu->caption);
  consoleb_p_draw_actions(consoleb_g_menu_action_icons);
  SCR_DIRTY;
}

void consoleb_p_on_handle_menu_input(UiHandle* uih, enum ActionType action)
{
  switch(action)
  {
    case Key1:
      if (uih->currentMenu->parent != NULL)
        uih->currentMenu = uih->currentMenu->parent;
      else if (uih->currentMenu->parentPage)
        ((UiPagePtr)uih->currentMenu->parentPage)->onHandleInput(uih, Key1);
      break;
    case Key2:
      if (uih->currentMenu->prev != NULL)
        uih->currentMenu = uih->currentMenu->prev;
      break;
    case Key3:
      if (uih->currentMenu->next != NULL)
        uih->currentMenu = uih->currentMenu->next;
      break;
    case Key4:
    {
      if (uih->currentMenu->callback != NULL)
      {
        consoleb_platform_clear();
        consoleb_platform_setxy(5, 5);
        consoleb_platform_write("Executing...");
        consoleb_platform_update_screen();
        consoleb_platform_delay(500);

        uih->currentMenu->callback();

        consoleb_platform_clear();
        consoleb_platform_setxy(5, 5);
        consoleb_platform_write("Done");
        consoleb_platform_update_screen();
        consoleb_platform_delay(1000);
      }
      else if (uih->currentMenu->children != NULL)
      {
        uih->currentMenu = uih->currentMenu->children;
      }
      else if (uih->currentMenu->page != NULL)
      {
        consoleb_change_page(uih, uih->currentMenu->page);
      }
      break;
    }
  }

  consoleb_p_draw_menu(uih);
}

void consoleb_p_draw_actions(const char* actions)
{
  if (actions == NULL || strlen(actions) < 4)
    return;

  consoleb_platform_before_draw_action();

  if (actions[consoleb_g_menu_actions_offset+0] != ' ')
  {
    consoleb_platform_setxy(5, 40);
    consoleb_platform_draw_action(actions[consoleb_g_menu_actions_offset+0]);
  }
  if (actions[consoleb_g_menu_actions_offset+1] != ' ')
  {
    consoleb_platform_setxy(40, 40);
    consoleb_platform_draw_action(actions[consoleb_g_menu_actions_offset+1]);
  }
  if (actions[consoleb_g_menu_actions_offset+2] != ' ')
  {
    consoleb_platform_setxy(75, 40);
    consoleb_platform_draw_action(actions[consoleb_g_menu_actions_offset+2]);
  }
  if (actions[consoleb_g_menu_actions_offset+3] != ' ')
  {
    consoleb_platform_setxy(110, 40);
    consoleb_platform_draw_action(actions[consoleb_g_menu_actions_offset+3]);
  }
}