#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#include "gp2x_psp.h"
#include "gp2x_cpu.h"
#include <time.h>

# ifndef CLK_TCK
# define CLK_TCK  CLOCKS_PER_SEC
# endif

#define CPU_CLOCK_IDLE 60
#define CPU_CLOCK_STD 200

# define THOM_RENDER_NORMAL      0
# define THOM_LAST_RENDER        0

# define MAX_PATH            256
# define THOM_MAX_SAVE_STATE    5

# define THOM_LOAD_K7_MODE      0
# define THOM_LOAD_DISK_MODE    1
# define THOM_MAX_LOAD_MODE     1

#include <SDL.h>

  typedef struct THOM_save_t {

    SDL_Surface    *surface;
    char            used;
    char            thumb;
    time_t          date;

  } THOM_save_t;

  typedef struct THOM_t {

    THOM_save_t thom_save_state[THOM_MAX_SAVE_STATE];
    char thom_save_name[MAX_PATH];
    char thom_home_dir[MAX_PATH];
    char thom_disk_dir[MAX_PATH];
    char thom_k7_dir[MAX_PATH];
    char thom_memo7_dir[MAX_PATH];
    int  psp_screenshot_id;
    int  psp_cpu_clock;
    int  psp_joystick_cursor;
    int  psp_display_lr;
    int  thom_view_fps;
    int  thom_current_fps;
    int  thom_current_clock;
    int  thom_snd_enable;
    int  thom_render_mode;
    int  psp_skip_max_frame;
    int  psp_skip_cur_frame;
    int  psp_active_joystick;
    int  thom_auto_fire;
    int  thom_auto_fire_pressed;
    int  thom_auto_fire_period;

    int  thom_load_mode;
    int  thom_basic_128;
    int  thom_sound_mute;
    int  thom_speed_limiter;

  } THOM_t;

  extern THOM_t THOM;

  extern void thom_set_basic_mode(int new_mode);

//END_LUDO:
  extern void thom_default_settings(void);
  extern int thom_save_settings(void);
  extern int thom_load_settings();
  extern int thom_load_file_settings(char *FileName);

  extern void thom_update_save_name(char *Name);
  extern void reset_save_name();
  extern void thom_kbd_load(void);
  extern int thom_kbd_save(void);
  extern void emulator_reset(void);
  extern int thom_load_rom(char *FileName, int zip_format);
  extern int thom_load_state(char *FileName);
  extern int thom_snapshot_save_slot(int save_id);
  extern int thom_snapshot_load_slot(int load_id);
  extern int thom_snapshot_del_slot(int save_id);

# endif
