## Structures
```c
struct Pack_VTable
{
	uint32_t* fnUn0;
	uint32_t* fnUn1;
	uint32_t* fnUn2;
	uint32_t* fnUn3;
	uint32_t* fnReadFile;
}

struct Read_INFO
{
	char aReadPath[512];
	char aPath1[512];
	char aPath2[512];
	char aPath3[512];
	Pack_VTable* pVtable;
};

struct CMVS_SIZE
{
	int aUn[6];
	int iWidth;
	int iHeigh;
}

struct CMVS_OBJ
{
  int *pVtable;
  HWND hWnd;
  int aUn1[3];
  int usIconID;
  int aUn2[52];
  WNDCLASSA WndClass;
  int aUn3[412];
  CMVS_SIZE WndSize;
  int aUn5[2817];
  int iCodeExecutLen;
  int aUn6[2817];
};

struct PS3_HDR
{
	uint32_t uiSignature;
	uint32_t uiHdrSize;
	uint32_t uiUn0;
	uint32_t uiDecryptKey;
	uint32_t uiLabelCount;
	uint32_t uiCodeSegSize;
	uint32_t uiUnSegSize;
	uint32_t uiTextSegSize;
	uint32_t uiBegPC;
	uint32_t uiCompressSize;
	uint32_t uiDecompressSize;
	uint32_t uiUn1;
};

PS3_File
- HDR
- Label_Table[uiLabelCount*4] // label 里每4个为一项，每一项为code segment的偏移
- Code Segment
- Un Segment
- Text Segment
```



## Pack Version
- case 0: ?
- case 1: CPZ1: Code1024V1
- case 2: CPZ2: Code1024V2
- case 3: CPZ3: Code1024V3
- case 4: CPZ4: Code1024V4 ?
- case 5: CPZ5 : Code1024V5
- case 6: CPZ5 B: Code1024V5B
- case 7: CPZ5 C: Code1024V5C
- case 8: CPZ5 D: Code1024V5D
- case 9: CPZ5 E: Code1024V5E
- case 10: CPZ6: Code1024V6
- case 11: CPZ6 B: Code1024V6B
- case 12: CPZ7: Code1024V7
- case 13: CPZ7 B: Code1024V7B



## OPCode

```C
struct CMVS_OPCode
{
    [sys_command_flag]  4bit
    [opcode]            3*4bit
};

struct CMVS_Param
{
    uint16_t usParameOPCode;
    uint16_t usParameOPCode;
    uint16_t usParameOPCode;
    ...
    Value
    ...
    uint16_t usEnd; // 0x020F
    uint16_t usVMOPCode;
    uint16_t usVMOPCode;
    uint16_t usVMOPCode;
    ....
};

struct CMVS_Code
{
    CMVS_Param Param;  // 可选，不一定有
    uint16_t usSysOPCode;
};
 
if opcode > 0x400 -> stack commnad
if opcode = 0x400 Nop
if opcode = 0x200 ParameTypeA
if opcode = 0x201 ParameTypeB
if opcode = 0x202 ParameTypeC
if opcode >= 0x2000 SysCommand -> really op = opcode - 0x2000
0x2000 = 0010 0000 0000 0000
0x07FF = 0000 0111 1111 1111  -> op mask
    
```

## ParameCommand

```C
OPCode:0x0200:ParameTypeA
OPCode:0x0201:ParameTypeB
OPCode:0x0202:ParameTypeC
```



## VMCommand

```C
OPCode:0x0400: nop
OPCode:0x0401: jmp if regX32 == 0 tp pc+6
OPCode:0x0402: jmp if regX32 !=0 to pc+6
OPCode:0x0403: jmp if reg32 == imm32 to imm32 (0304 01000000 C0960300 (reg0==01000000 -> pc = C0960300))
OPCode:0x0405: jmp to imm16 (imm -> script_ptr - hdr_size)
OPCode:0x0407: jmp to imm32 (jmp to imm,0704 00000000 C0960300, jmp to C0960300)
OPCode:0x0410: call_label imm (imm:lable_index ,1004 0700 -> 0x0410(call label) 0x0007(label table index)
OPCode:0x0411: ret stack[n]
OPCode:0x0412: pop stack[n]
OPCode:0x0413: jmp stack[n]
OPCode:0x0414: switch_slot stack[slot],stack[pc]
OPCode:0x0416: call reg032
OPCode:0x0430: push reg032
OPCode:0x0440: push imm32
OPCode:0x0442: push imm32
```



## SystemCommand

```c
OPCode:0x2000:CMVS::ScriptCommand::sys_wait
OPCode:0x2001:CMVS::ScriptCommand::sys_end
OPCode:0x2002:CMVS::ScriptCommand::sys_fps
OPCode:0x2003:CMVS::ScriptCommand::sys_reset
OPCode:0x2004:CMVS::ScriptCommand::sys_frame_set_timer
OPCode:0x2005:CMVS::ScriptCommand::sys_frame_get_timer
OPCode:0x2006:CMVS::ScriptCommand::sys_frame_set_mode
OPCode:0x2007:CMVS::ScriptCommand::sys_play_get_timer
OPCode:0x2008:CMVS::ScriptCommand::sys_get_timer
OPCode:0x2009:CMVS::ScriptCommand::sys_get_next_timer
OPCode:0x200A:CMVS::ScriptCommand::sys_set_timer_wait_cut_flags
OPCode:0x200B:CMVS::ScriptCommand::sys_base_timer_create
OPCode:0x200C:CMVS::ScriptCommand::sys_base_timer_now
OPCode:0x200D:CMVS::ScriptCommand::sys_system_now_date
OPCode:0x200E:CMVS::ScriptCommand::sys_master_timer_create
OPCode:0x200F:CMVS::ScriptCommand::sys_master_timer_now
OPCode:0x2010:CMVS::ScriptCommand::sys_file_script_path
OPCode:0x2011:CMVS::ScriptCommand::sys_file_image_path
OPCode:0x2012:CMVS::ScriptCommand::sys_file_wave_path
OPCode:0x2013:CMVS::ScriptCommand::sys_file_music_path
OPCode:0x2014:CMVS::ScriptCommand::sys_file_voice_path
OPCode:0x2015:CMVS::ScriptCommand::sys_file_video_path
OPCode:0x2016:CMVS::ScriptCommand::sys_file_save_path
OPCode:0x2017:CMVS::ScriptCommand::sys_file_image_ex_path
OPCode:0x2018:CMVS::ScriptCommand::sys_file_polygon_path
OPCode:0x2019:CMVS::ScriptCommand::sys_file_shader_path
OPCode:0x201A:CMVS::ScriptCommand::sys_file_image_video_path
OPCode:0x201C:CMVS::ScriptCommand::sys_shell_execute
OPCode:0x201D:CMVS::ScriptCommand::sys_game_id_create
OPCode:0x201E:CMVS::ScriptCommand::sys_system_save_file_ver
OPCode:0x201F:CMVS::ScriptCommand::sys_system_pack_file_ver
OPCode:0x2020:CMVS::ScriptCommand::sys_object_create
OPCode:0x2021:CMVS::ScriptCommand::sys_object_destroy
OPCode:0x2022:CMVS::ScriptCommand::sys_object_child_create
OPCode:0x2023:CMVS::ScriptCommand::sys_object_child_destroy
OPCode:0x2024:
OPCode:0x2025:
OPCode:0x2026:CMVS::ScriptCommand::sys_object_show
OPCode:0x2027:CMVS::ScriptCommand::sys_object_check
OPCode:0x2028:CMVS::ScriptCommand::sys_object_move
OPCode:0x2029:
OPCode:0x202A:CMVS::ScriptCommand::sys_object_resolving_power_length
OPCode:0x202B:CMVS::ScriptCommand::sys_object_render_texture
OPCode:0x202C:CMVS::ScriptCommand::sys_object_attribute_no_coordinates
OPCode:0x202D:CMVS::ScriptCommand::sys_object_attribute_rank
OPCode:0x202E:CMVS::ScriptCommand::sys_object_attribute_no_shake
OPCode:0x202F:CMVS::ScriptCommand::sys_object_all_destroy
OPCode:0x2030:CMVS::ScriptCommand::sys_object_texture_create
OPCode:0x2031:CMVS::ScriptCommand::sys_object_texture_destroy
OPCode:0x2032:CMVS::ScriptCommand::sys_object_texture_create2
OPCode:0x2033:CMVS::ScriptCommand::sys_object_texture_get_info
OPCode:0x2034:CMVS::ScriptCommand::sys_object_texture_get_color_xy
OPCode:0x2035:CMVS::ScriptCommand::sys_object_texture_alpha_mask_create
OPCode:0x2036:
OPCode:0x2037:
OPCode:0x2038:CMVS::ScriptCommand::sys_object_child_texture_create
OPCode:0x2039:CMVS::ScriptCommand::sys_object_child_texture_destroy
OPCode:0x203A:
OPCode:0x203B:CMVS::ScriptCommand::sys_object_texture_create_ex
OPCode:0x203C:CMVS::ScriptCommand::sys_object_texture_surface_memory
OPCode:0x203D:CMVS::ScriptCommand::sys_object_texture_back_surface
OPCode:0x203E:CMVS::ScriptCommand::sys_object_texture_window_back_surface
OPCode:0x203F:CMVS::ScriptCommand::sys_texture_image_cache_use
OPCode:0x2040:CMVS::ScriptCommand::sys_object_sprite_create
OPCode:0x2041:CMVS::ScriptCommand::sys_object_sprite_destroy
OPCode:0x2042:CMVS::ScriptCommand::sys_object_sprite_mode3D
OPCode:0x2043:CMVS::ScriptCommand::sys_object_sprite_color
OPCode:0x2044:CMVS::ScriptCommand::sys_object_sprite_uv
OPCode:0x2045:CMVS::ScriptCommand::sys_object_sprite_xy
OPCode:0x2046:CMVS::ScriptCommand::sys_object_sprite_oxy
OPCode:0x2047:CMVS::ScriptCommand::sys_object_sprite_zrank
OPCode:0x2048:CMVS::ScriptCommand::sys_object_sprite_trans
OPCode:0x2049:CMVS::ScriptCommand::sys_object_sprite_zoom
OPCode:0x204A:CMVS::ScriptCommand::sys_object_sprite_rotation_z
OPCode:0x204B:CMVS::ScriptCommand::sys_object_sprite_trans_mode
OPCode:0x204C:CMVS::ScriptCommand::sys_object_sprite_mode3DEx
OPCode:0x204D:
OPCode:0x204E:CMVS::ScriptCommand::sys_object_sprite_layer_mode
OPCode:0x204F:CMVS::ScriptCommand::sys_object_sprite_info
OPCode:0x2050:CMVS::ScriptCommand::sys_object_child_sprite_create
OPCode:0x2051:CMVS::ScriptCommand::sys_object_child_sprite_destroy
OPCode:0x2052:
OPCode:0x2053:
OPCode:0x2054:CMVS::ScriptCommand::sys_object_sprite_xy2
OPCode:0x2055:CMVS::ScriptCommand::sys_object_sprite_layer_mode_ex
OPCode:0x2056:CMVS::ScriptCommand::sys_quasi_camera_ex_to_display
OPCode:0x2057:
OPCode:0x2058:CMVS::ScriptCommand::sys_quasi_camera_ex_world_space
OPCode:0x2059:CMVS::ScriptCommand::sys_quasi_camera_ex_world_map
OPCode:0x205A:CMVS::ScriptCommand::sys_quasi_camera_ex_xyz
OPCode:0x205B:CMVS::ScriptCommand::sys_quasi_camera_ex_rotation_z
OPCode:0x205C:CMVS::ScriptCommand::sys_quasi_camera_ex_depth_zy
OPCode:0x205D:CMVS::ScriptCommand::sys_quasi_camera_ex_view_oxy
OPCode:0x205E:CMVS::ScriptCommand::sys_quasi_camera_ex_use
OPCode:0x205F:CMVS::ScriptCommand::sys_quasi_camera_ex_out_mode
OPCode:0x2060:CMVS::ScriptCommand::sys_quasi_camera_world_space
OPCode:0x2061:CMVS::ScriptCommand::sys_quasi_camera_world_map
OPCode:0x2062:CMVS::ScriptCommand::sys_quasi_camera_xyz
OPCode:0x2063:CMVS::ScriptCommand::sys_quasi_camera_rotation_z
OPCode:0x2064:CMVS::ScriptCommand::sys_quasi_camera_depth_zy
OPCode:0x2065:CMVS::ScriptCommand::sys_quasi_camera_view_oxy
OPCode:0x2066:CMVS::ScriptCommand::sys_quasi_camera_ex_task_save_mode
OPCode:0x2067:CMVS::ScriptCommand::sys_quasi_camera_ex_move_revise
OPCode:0x2068:CMVS::ScriptCommand::sys_quasi_camera_task_stop
OPCode:0x2069:CMVS::ScriptCommand::sys_quasi_camera_task_wait
OPCode:0x206A:CMVS::ScriptCommand::sys_quasi_camera_task_status
OPCode:0x206B:CMVS::ScriptCommand::sys_quasi_camera_task_move_xz
OPCode:0x206C:CMVS::ScriptCommand::sys_quasi_camera_task_move_xyz
OPCode:0x206D:CMVS::ScriptCommand::sys_quasi_camera_task_move_y
OPCode:0x206E:CMVS::ScriptCommand::sys_quasi_camera_task_move_x
OPCode:0x206F:CMVS::ScriptCommand::sys_quasi_camera_task_move_z
OPCode:0x2070:CMVS::ScriptCommand::sys_object_sprite_m3d_nz
OPCode:0x2071:CMVS::ScriptCommand::sys_object_sprite_m3d_ny
OPCode:0x2072:CMVS::ScriptCommand::sys_object_sprite_m3d_wx
OPCode:0x2073:CMVS::ScriptCommand::sys_object_sprite_m3d_wy
OPCode:0x2074:CMVS::ScriptCommand::sys_object_sprite_m3d_wz
OPCode:0x2075:CMVS::ScriptCommand::sys_object_sprite_m3d_vwz
OPCode:0x2076:
OPCode:0x2077:
OPCode:0x2078:CMVS::ScriptCommand::sys_object_sprite_m3d_get_map
OPCode:0x2079:
OPCode:0x207A:
OPCode:0x207B:
OPCode:0x207C:
OPCode:0x207D:CMVS::ScriptCommand::sys_quasi_camera_task_move_xyz_rot_z
OPCode:0x207E:
OPCode:0x207F:
OPCode:0x2080:CMVS::ScriptCommand::sys_script_open
OPCode:0x2081:CMVS::ScriptCommand::sys_script_int
OPCode:0x2082:CMVS::ScriptCommand::sys_script_get_name
OPCode:0x2083:CMVS::ScriptCommand::sys_script_set_ver
OPCode:0x2084:
OPCode:0x2085:
OPCode:0x2086:
OPCode:0x2087:CMVS::ScriptCommand::sys_button_old_version
OPCode:0x2088:CMVS::ScriptCommand::sys_int_create
OPCode:0x2089:CMVS::ScriptCommand::sys_int_use
OPCode:0x208A:CMVS::ScriptCommand::sys_int_call
OPCode:0x208B:CMVS::ScriptCommand::sys_int_release
OPCode:0x208C:CMVS::ScriptCommand::sys_int_close_use
OPCode:0x208D:
OPCode:0x208E:
OPCode:0x208F:CMVS::ScriptCommand::sys_icon_window_image_create
OPCode:0x2090:CMVS::ScriptCommand::sys_system_data_error_check
OPCode:0x2091:CMVS::ScriptCommand::sys_system_flags_clear
OPCode:0x2092:CMVS::ScriptCommand::sys_system_int_clear
OPCode:0x2093:CMVS::ScriptCommand::sys_system_float_clear
OPCode:0x2094:CMVS::ScriptCommand::sys_system_str_clear
OPCode:0x2095:
OPCode:0x2096:
OPCode:0x2097:
OPCode:0x2098:CMVS::ScriptCommand::sys_mouse_icon_file_create
OPCode:0x2099:CMVS::ScriptCommand::sys_mouse_icon_index_change
OPCode:0x209A:
OPCode:0x209B:
OPCode:0x209C:CMVS::ScriptCommand::sys_quasi_camera_ex_render_target
OPCode:0x209D:
OPCode:0x209E:
OPCode:0x209F:
OPCode:0x20A0:CMVS::ScriptCommand::sys_bgm_play
OPCode:0x20A1:CMVS::ScriptCommand::sys_bgm_stop
OPCode:0x20A2:CMVS::ScriptCommand::sys_bgm_fadeout
OPCode:0x20A3:CMVS::ScriptCommand::sys_bgm_get_status
OPCode:0x20A4:CMVS::ScriptCommand::sys_bgm_play_two
OPCode:0x20A5:CMVS::ScriptCommand::sys_sound_set_main_volume
OPCode:0x20A6:CMVS::ScriptCommand::sys_voice_open
OPCode:0x20A7:CMVS::ScriptCommand::sys_voice_play
OPCode:0x20A8:CMVS::ScriptCommand::sys_voice_release
OPCode:0x20A9:CMVS::ScriptCommand::sys_voice_fadeout
OPCode:0x20AA:CMVS::ScriptCommand::sys_voice_status
OPCode:0x20AB:CMVS::ScriptCommand::sys_voice_run_byte_length
OPCode:0x20AC:CMVS::ScriptCommand::sys_voice_sub_volume
OPCode:0x20AD:CMVS::ScriptCommand::sys_voice_channel_volume
OPCode:0x20AE:CMVS::ScriptCommand::sys_voice_power_level
OPCode:0x20AF:CMVS::ScriptCommand::sys_voice_open_time_stretch
OPCode:0x20B0:CMVS::ScriptCommand::sys_wave_open
OPCode:0x20B1:CMVS::ScriptCommand::sys_wave_play
OPCode:0x20B2:CMVS::ScriptCommand::sys_wave_stop
OPCode:0x20B3:CMVS::ScriptCommand::sys_wave_release
OPCode:0x20B4:CMVS::ScriptCommand::sys_wave_replay
OPCode:0x20B5:CMVS::ScriptCommand::sys_wave_all_release
OPCode:0x20B6:CMVS::ScriptCommand::sys_wave_fadeout
OPCode:0x20B7:CMVS::ScriptCommand::sys_wave_status
OPCode:0x20B8:CMVS::ScriptCommand::sys_wave_sub_volume
OPCode:0x20B9:CMVS::ScriptCommand::sys_wave_channel_volume
OPCode:0x20BA:CMVS::ScriptCommand::sys_wave_fade_open
OPCode:0x20BB:
OPCode:0x20BC:CMVS::ScriptCommand::sys_voice_release_task
OPCode:0x20BD:CMVS::ScriptCommand::sys_voice_release_wait
OPCode:0x20BE:
OPCode:0x20BF:
OPCode:0x20C0:CMVS::ScriptCommand::sys_video_play
OPCode:0x20C1:CMVS::ScriptCommand::sys_video_stop
OPCode:0x20C2:CMVS::ScriptCommand::sys_video_status
OPCode:0x20C3:
OPCode:0x20C4:CMVS::ScriptCommand::sys_video_mgv_play
OPCode:0x20C5:
OPCode:0x20C6:
OPCode:0x20C7:
OPCode:0x20C8:CMVS::ScriptCommand::sys_mouse_get_xy
OPCode:0x20C9:CMVS::ScriptCommand::sys_mouse_get_move
OPCode:0x20CA:CMVS::ScriptCommand::sys_mouse_get_position
OPCode:0x20CB:CMVS::ScriptCommand::sys_mouse_get_position_ex
OPCode:0x20CC:CMVS::ScriptCommand::sys_mouse_set_xy
OPCode:0x20CD:CMVS::ScriptCommand::sys_mouse_set_screen
OPCode:0x20CE:
OPCode:0x20CF:
OPCode:0x20D0:CMVS::ScriptCommand::sys_abs
OPCode:0x20D1:CMVS::ScriptCommand::sys_limit
OPCode:0x20D2:CMVS::ScriptCommand::sys_max
OPCode:0x20D3:CMVS::ScriptCommand::sys_min
OPCode:0x20D4:CMVS::ScriptCommand::sys_rand
OPCode:0x20D5:CMVS::ScriptCommand::sys_rand2
OPCode:0x20D6:CMVS::ScriptCommand::sys_power
OPCode:0x20D7:CMVS::ScriptCommand::sys_length
OPCode:0x20D8:CMVS::ScriptCommand::sys_rotation_map_point_z
OPCode:0x20D9:CMVS::ScriptCommand::sys_rotation_map_point_z2
OPCode:0x20DA:CMVS::ScriptCommand::sys_xy_to_rotation_length
OPCode:0x20DB:CMVS::ScriptCommand::sys_xy_to_rotation_length2
OPCode:0x20DC:CMVS::ScriptCommand::sys_rotation_map_length
OPCode:0x20DD:
OPCode:0x20DE:
OPCode:0x20DF:
OPCode:0x20E0:CMVS::ScriptCommand::sys_rotation_xyz
OPCode:0x20E1:
OPCode:0x20E2:
OPCode:0x20E3:
OPCode:0x20E4:
OPCode:0x20E5:
OPCode:0x20E6:
OPCode:0x20E7:
OPCode:0x20E8:CMVS::ScriptCommand::sys_system_id_create
OPCode:0x20E9:CMVS::ScriptCommand::sys_system_android_check
OPCode:0x20EA:
OPCode:0x20EB:
OPCode:0x20EC:CMVS::ScriptCommand::sys_message_window_set_zoom
OPCode:0x20ED:
OPCode:0x20EE:
OPCode:0x20EF:
OPCode:0x20F0:CMVS::ScriptCommand::sys_lstrcpy
OPCode:0x20F1:CMVS::ScriptCommand::sys_lstrcmp
OPCode:0x20F2:CMVS::ScriptCommand::sys_lstrcat
OPCode:0x20F3:CMVS::ScriptCommand::sys_lstrncut
OPCode:0x20F4:CMVS::ScriptCommand::sys_lstrtocpy
OPCode:0x20F5:CMVS::ScriptCommand::sys_lstrtopcmp
OPCode:0x20F6:CMVS::ScriptCommand::sys_lstrtoint
OPCode:0x20F7:CMVS::ScriptCommand::sys_lstrcount
OPCode:0x20F8:CMVS::ScriptCommand::sys_lstrlen
OPCode:0x20F9:CMVS::ScriptCommand::sys_lstrncount
OPCode:0x20FA:CMVS::ScriptCommand::sys_linttostr
OPCode:0x20FB:CMVS::ScriptCommand::sys_lstrtodate
OPCode:0x20FC:CMVS::ScriptCommand::sys_sprintf_int
OPCode:0x20FD:CMVS::ScriptCommand::sys_sprintf_str
OPCode:0x20FE:
OPCode:0x20FF:CMVS::ScriptCommand::sys_change_extension
OPCode:0x2100:CMVS::ScriptCommand::sys_text_layer_create
OPCode:0x2101:CMVS::ScriptCommand::sys_text_layer_release
OPCode:0x2102:CMVS::ScriptCommand::sys_text_layer_xy
OPCode:0x2103:CMVS::ScriptCommand::sys_text_layer_draw_map
OPCode:0x2104:CMVS::ScriptCommand::sys_text_layer_draw_clear
OPCode:0x2105:CMVS::ScriptCommand::sys_text_layer_font_size
OPCode:0x2106:CMVS::ScriptCommand::sys_text_layer_font_change
OPCode:0x2107:CMVS::ScriptCommand::sys_text_layer_font_color
OPCode:0x2108:CMVS::ScriptCommand::sys_text_layer_confinement_text
OPCode:0x2109:CMVS::ScriptCommand::sys_text_layer_draw_status
OPCode:0x210A:CMVS::ScriptCommand::sys_text_layer_message
OPCode:0x210B:CMVS::ScriptCommand::sys_text_layer_output
OPCode:0x210C:CMVS::ScriptCommand::sys_text_layer_offset
OPCode:0x210D:CMVS::ScriptCommand::sys_text_layer_indication_variety
OPCode:0x210E:CMVS::ScriptCommand::sys_text_layer_message_velocity
OPCode:0x210F:CMVS::ScriptCommand::sys_text_layer_color_mode
OPCode:0x2110:CMVS::ScriptCommand::sys_text_layer_rank
OPCode:0x2111:CMVS::ScriptCommand::sys_text_layer_effect_skip
OPCode:0x2112:CMVS::ScriptCommand::sys_text_layer_line_count
OPCode:0x2113:CMVS::ScriptCommand::sys_text_layer_shadow_mode
OPCode:0x2114:CMVS::ScriptCommand::sys_text_layer_ruby_use
OPCode:0x2115:CMVS::ScriptCommand::sys_text_layer_ruby_size
OPCode:0x2116:CMVS::ScriptCommand::sys_text_layer_ruby_picth
OPCode:0x2117:CMVS::ScriptCommand::sys_text_layer_ruby_line_lock
OPCode:0x2118:CMVS::ScriptCommand::sys_text_layer_font_bold
OPCode:0x2119:CMVS::ScriptCommand::sys_text_layer_to_text_count
OPCode:0x211A:CMVS::ScriptCommand::sys_text_layer_shadow_color
OPCode:0x211B:
OPCode:0x211C:CMVS::ScriptCommand::sys_text_layer_create_ex
OPCode:0x211D:CMVS::ScriptCommand::sys_text_layer_draw_text
OPCode:0x211E:CMVS::ScriptCommand::sys_text_layer_ret_mode_create
OPCode:0x211F:CMVS::ScriptCommand::sys_text_layer_ret_mode_count
OPCode:0x2120:CMVS::ScriptCommand::sys_system_log_file_check
OPCode:0x2121:CMVS::ScriptCommand::sys_system_log_script_check
OPCode:0x2122:CMVS::ScriptCommand::sys_system_log_file_clear
OPCode:0x2123:CMVS::ScriptCommand::sys_system_log_script_clear
OPCode:0x2124:
OPCode:0x2125:
OPCode:0x2126:CMVS::ScriptCommand::sys_window_standard_size_info
OPCode:0x2127:
OPCode:0x2128:CMVS::ScriptCommand::sys_system_save_open
OPCode:0x2129:CMVS::ScriptCommand::sys_system_save_close
OPCode:0x212A:CMVS::ScriptCommand::sys_system_save_file_log
OPCode:0x212B:CMVS::ScriptCommand::sys_system_save_script_log
OPCode:0x212C:CMVS::ScriptCommand::sys_system_save_pc_info
OPCode:0x212D:CMVS::ScriptCommand::sys_system_save_pc_status
OPCode:0x212E:CMVS::ScriptCommand::sys_log_message_get_max_count
OPCode:0x212F:CMVS::ScriptCommand::sys_log_message_clear
OPCode:0x2130:CMVS::ScriptCommand::sys_log_message_get_count
OPCode:0x2131:CMVS::ScriptCommand::sys_log_message_get_voice
OPCode:0x2132:CMVS::ScriptCommand::sys_log_message_get_text
OPCode:0x2133:CMVS::ScriptCommand::sys_log_message_get_name
OPCode:0x2134:CMVS::ScriptCommand::sys_log_message_get_file
OPCode:0x2135:CMVS::ScriptCommand::sys_log_message_get_data
OPCode:0x2136:CMVS::ScriptCommand::sys_log_message_insert_data
OPCode:0x2137:CMVS::ScriptCommand::sys_log_message_set_max_count
OPCode:0x2138:
OPCode:0x2139:
OPCode:0x213A:CMVS::ScriptCommand::sys_dialog_system_close
OPCode:0x213B:CMVS::ScriptCommand::sys_dialog_message_check
OPCode:0x213C:CMVS::ScriptCommand::sys_dialog_input_string
OPCode:0x213D:CMVS::ScriptCommand::sys_dialog_font_create
OPCode:0x213E:
OPCode:0x213F:
OPCode:0x2140:CMVS::ScriptCommand::sys_message_window_create
OPCode:0x2141:CMVS::ScriptCommand::sys_message_window_release
OPCode:0x2142:CMVS::ScriptCommand::sys_message_window_layer_rank
OPCode:0x2143:CMVS::ScriptCommand::sys_message_window_font_size
OPCode:0x2144:CMVS::ScriptCommand::sys_message_window_font_space
OPCode:0x2145:CMVS::ScriptCommand::sys_message_window_draw_map
OPCode:0x2146:CMVS::ScriptCommand::sys_message_window_default_color
OPCode:0x2147:CMVS::ScriptCommand::sys_message_window_velocity
OPCode:0x2148:CMVS::ScriptCommand::sys_message_window_indication_variety
OPCode:0x2149:CMVS::ScriptCommand::sys_message_window_color_mode
OPCode:0x214A:CMVS::ScriptCommand::sys_message_window_text_clear
OPCode:0x214B:CMVS::ScriptCommand::sys_message_window_check
OPCode:0x214C:CMVS::ScriptCommand::sys_message_window_set_hide
OPCode:0x214D:CMVS::ScriptCommand::sys_message_window_get_hide
OPCode:0x214E:CMVS::ScriptCommand::sys_message_window_disp
OPCode:0x214F:CMVS::ScriptCommand::sys_message_window_draw_pos
OPCode:0x2150:CMVS::ScriptCommand::sys_message_window_set_move
OPCode:0x2151:CMVS::ScriptCommand::sys_message_window_ret_pos
OPCode:0x2152:CMVS::ScriptCommand::sys_message_window_code_wait
OPCode:0x2153:CMVS::ScriptCommand::sys_message_window_key_wait
OPCode:0x2154:CMVS::ScriptCommand::sys_message_window_output
OPCode:0x2155:CMVS::ScriptCommand::sys_message_window_shadow_color
OPCode:0x2156:CMVS::ScriptCommand::sys_message_window_text_layer_effect_skip
OPCode:0x2157:CMVS::ScriptCommand::sys_message_window_code_wait_auto
OPCode:0x2158:CMVS::ScriptCommand::sys_message_window_key_wait_auto
OPCode:0x2159:CMVS::ScriptCommand::sys_message_window_code_proc_auto
OPCode:0x215A:CMVS::ScriptCommand::sys_message_window_full_move_xy
OPCode:0x215B:CMVS::ScriptCommand::sys_message_window_full_move_use
OPCode:0x215C:CMVS::ScriptCommand::sys_message_window_code_proc
OPCode:0x215D:CMVS::ScriptCommand::sys_message_window_create_ex
OPCode:0x215E:CMVS::ScriptCommand::sys_message_window_get_disp_pos
OPCode:0x215F:CMVS::ScriptCommand::sys_message_window_get_mouse_map_pos
OPCode:0x2160:CMVS::ScriptCommand::sys_window_set_title
OPCode:0x2161:CMVS::ScriptCommand::sys_window_set_sub_title
OPCode:0x2162:CMVS::ScriptCommand::sys_window_script_title
OPCode:0x2163:CMVS::ScriptCommand::sys_window_get_display_mode
OPCode:0x2164:CMVS::ScriptCommand::sys_window_set_display_mode_wait
OPCode:0x2165:CMVS::ScriptCommand::sys_window_get_size
OPCode:0x2166:CMVS::ScriptCommand::sys_window_get_xy
OPCode:0x2167:CMVS::ScriptCommand::sys_window_get_desktop_size
OPCode:0x2168:CMVS::ScriptCommand::sys_window_set_xy_wait
OPCode:0x2169:CMVS::ScriptCommand::sys_window_get_status
OPCode:0x216A:
OPCode:0x216B:CMVS::ScriptCommand::sys_window_complement_info
OPCode:0x216C:CMVS::ScriptCommand::sys_window_complement_get_status
OPCode:0x216D:CMVS::ScriptCommand::sys_window_complement_set_status
OPCode:0x216E:CMVS::ScriptCommand::sys_window_resize_init_wait
OPCode:0x216F:CMVS::ScriptCommand::sys_window_resize_info
OPCode:0x2170:CMVS::ScriptCommand::sys_message_object_texture_open
OPCode:0x2171:CMVS::ScriptCommand::sys_message_object_texture_release
OPCode:0x2172:
OPCode:0x2173:
OPCode:0x2174:
OPCode:0x2175:
OPCode:0x2176:CMVS::ScriptCommand::sys_message_object_child_texture_open
OPCode:0x2177:CMVS::ScriptCommand::sys_message_object_child_texture_release
OPCode:0x2178:CMVS::ScriptCommand::sys_message_object_create
OPCode:0x2179:CMVS::ScriptCommand::sys_message_object_release
OPCode:0x217A:CMVS::ScriptCommand::sys_message_object_show
OPCode:0x217B:CMVS::ScriptCommand::sys_message_object_check
OPCode:0x217C:CMVS::ScriptCommand::sys_message_object_resolving_power
OPCode:0x217D:
OPCode:0x217E:CMVS::ScriptCommand::sys_message_object_attribute_no_coordinates
OPCode:0x217F:CMVS::ScriptCommand::sys_message_object_attribute_no_shake
OPCode:0x2180:CMVS::ScriptCommand::sys_message_object_sprite_create
OPCode:0x2181:CMVS::ScriptCommand::sys_message_object_sprite_destroy
OPCode:0x2182:CMVS::ScriptCommand::sys_message_object_sprite_uv
OPCode:0x2183:CMVS::ScriptCommand::sys_message_object_sprite_xy
OPCode:0x2184:CMVS::ScriptCommand::sys_message_object_sprite_zrank
OPCode:0x2185:CMVS::ScriptCommand::sys_message_object_sprite_oxy
OPCode:0x2186:CMVS::ScriptCommand::sys_message_object_sprite_trans
OPCode:0x2187:CMVS::ScriptCommand::sys_message_object_sprite_trans_mode
OPCode:0x2188:
OPCode:0x2189:CMVS::ScriptCommand::sys_message_object_sprite_color
OPCode:0x218A:
OPCode:0x218B:CMVS::ScriptCommand::sys_message_object_sprite_zoom
OPCode:0x218C:
OPCode:0x218D:CMVS::ScriptCommand::sys_message_object_sprite_info
OPCode:0x218E:
OPCode:0x218F:
OPCode:0x2190:CMVS::ScriptCommand::sys_message_window_icon_create
OPCode:0x2191:CMVS::ScriptCommand::sys_message_window_icon_mouse_pos
OPCode:0x2192:CMVS::ScriptCommand::sys_message_window_icon_start
OPCode:0x2193:CMVS::ScriptCommand::sys_message_window_icon_stop
OPCode:0x2194:CMVS::ScriptCommand::sys_message_window_icon_mouse
OPCode:0x2195:CMVS::ScriptCommand::sys_message_window_icon_get_use
OPCode:0x2196:CMVS::ScriptCommand::sys_message_window_icon_change
OPCode:0x2197:CMVS::ScriptCommand::sys_message_window_icon_pos
OPCode:0x2198:
OPCode:0x2199:
OPCode:0x219A:CMVS::ScriptCommand::sys_custom_key_all_clear
OPCode:0x219B:
OPCode:0x219C:CMVS::ScriptCommand::sys_custom_key_get_message_hist_up
OPCode:0x219D:CMVS::ScriptCommand::sys_custom_key_clear_message_hist_up
OPCode:0x219E:CMVS::ScriptCommand::sys_custom_key_get_message_hist_down
OPCode:0x219F:CMVS::ScriptCommand::sys_custom_key_clear_message_hist_down
OPCode:0x21A0:CMVS::ScriptCommand::sys_custom_key_get_decision
OPCode:0x21A1:CMVS::ScriptCommand::sys_custom_key_clear_decision
OPCode:0x21A2:CMVS::ScriptCommand::sys_custom_key_get_cancel
OPCode:0x21A3:CMVS::ScriptCommand::sys_custom_key_clear_cancel
OPCode:0x21A4:CMVS::ScriptCommand::sys_custom_key_get_compulsion_skip
OPCode:0x21A5:
OPCode:0x21A6:CMVS::ScriptCommand::sys_custom_key_get_yet_skip
OPCode:0x21A7:
OPCode:0x21A8:CMVS::ScriptCommand::sys_custom_key_get_window_elimina
OPCode:0x21A9:CMVS::ScriptCommand::sys_custom_key_clear_window_elimina
OPCode:0x21AA:CMVS::ScriptCommand::sys_custom_key_get_message_auto
OPCode:0x21AB:CMVS::ScriptCommand::sys_custom_key_clear_message_auto
OPCode:0x21AC:CMVS::ScriptCommand::sys_custom_key_get_message_history
OPCode:0x21AD:CMVS::ScriptCommand::sys_custom_key_clear_message_history
OPCode:0x21AE:CMVS::ScriptCommand::sys_custom_key_get_voice_play
OPCode:0x21AF:CMVS::ScriptCommand::sys_custom_key_clear_voice_play
OPCode:0x21B0:CMVS::ScriptCommand::sys_custom_key_get_quick_save
OPCode:0x21B1:CMVS::ScriptCommand::sys_custom_key_clear_quick_save
OPCode:0x21B2:CMVS::ScriptCommand::sys_custom_key_get_quick_load
OPCode:0x21B3:CMVS::ScriptCommand::sys_custom_key_clear_quick_load
OPCode:0x21B4:CMVS::ScriptCommand::sys_custom_key_get_popup_menu
OPCode:0x21B5:CMVS::ScriptCommand::sys_custom_key_clear_popup_menu
OPCode:0x21B6:CMVS::ScriptCommand::sys_custom_key_get_cursor_up
OPCode:0x21B7:CMVS::ScriptCommand::sys_custom_key_clear_cursor_up
OPCode:0x21B8:CMVS::ScriptCommand::sys_custom_key_get_cursor_down
OPCode:0x21B9:CMVS::ScriptCommand::sys_custom_key_clear_cursor_down
OPCode:0x21B:
OPCode:0x21BA:CMVS::ScriptCommand::sys_custom_key_get_cursor_left
OPCode:0x21BB:CMVS::ScriptCommand::sys_custom_key_clear_cursor_left
OPCode:0x21BC:CMVS::ScriptCommand::sys_custom_key_get_cursor_right
OPCode:0x21BD:CMVS::ScriptCommand::sys_custom_key_clear_cursor_right
OPCode:0x21BE:
OPCode:0x21BF:
OPCode:0x21C0:CMVS::ScriptCommand::sys_message_object_sprite_anime_magni_create
OPCode:0x21C1:CMVS::ScriptCommand::sys_message_object_sprite_anime_magni_registration
OPCode:0x21C2:CMVS::ScriptCommand::sys_message_object_sprite_anime_magni_start
OPCode:0x21C3:CMVS::ScriptCommand::sys_message_object_sprite_anime_magni_release
OPCode:0x21C4:
OPCode:0x21C5:CMVS::ScriptCommand::sys_message_object_sprite_anime_zrot_create
OPCode:0x21C6:CMVS::ScriptCommand::sys_message_object_sprite_anime_zrot_registration
OPCode:0x21C7:CMVS::ScriptCommand::sys_message_object_sprite_anime_zrot_start
OPCode:0x21C8:CMVS::ScriptCommand::sys_message_object_sprite_anime_zrot_release
OPCode:0x21C9:
OPCode:0x21CA:CMVS::ScriptCommand::sys_message_object_sprite_anime_trans_create
OPCode:0x21CB:CMVS::ScriptCommand::sys_message_object_sprite_anime_trans_registration
OPCode:0x21CC:CMVS::ScriptCommand::sys_message_object_sprite_anime_trans_start
OPCode:0x21CD:CMVS::ScriptCommand::sys_message_object_sprite_anime_trans_release
OPCode:0x21CE:
OPCode:0x21CF:CMVS::ScriptCommand::sys_message_object_sprite_anime_pattern_create
OPCode:0x21D0:CMVS::ScriptCommand::sys_message_object_sprite_anime_pattern_registration
OPCode:0x21D1:CMVS::ScriptCommand::sys_message_object_sprite_anime_pattern_start
OPCode:0x21D2:CMVS::ScriptCommand::sys_message_object_sprite_anime_pattern_release
OPCode:0x21D3:
OPCode:0x21D4:CMVS::ScriptCommand::sys_message_object_sprite_anime_move_create
OPCode:0x21D5:CMVS::ScriptCommand::sys_message_object_sprite_anime_move_registration
OPCode:0x21D6:CMVS::ScriptCommand::sys_message_object_sprite_anime_move_start
OPCode:0x21D7:CMVS::ScriptCommand::sys_message_object_sprite_anime_move_release
OPCode:0x21D8:
OPCode:0x21D9:
OPCode:0x21DA:
OPCode:0x21DB:
OPCode:0x21DC:
OPCode:0x21DD:
OPCode:0x21DE:
OPCode:0x21DF:
OPCode:0x21E0:CMVS::ScriptCommand::sys_object_sprite_anime_magni_create
OPCode:0x21E1:CMVS::ScriptCommand::sys_object_sprite_anime_magni_registration
OPCode:0x21E2:CMVS::ScriptCommand::sys_object_sprite_anime_magni_start
OPCode:0x21E3:CMVS::ScriptCommand::sys_object_sprite_anime_magni_release
OPCode:0x21E4:CMVS::ScriptCommand::sys_object_sprite_anime_magni_get_status
OPCode:0x21E5:CMVS::ScriptCommand::sys_object_sprite_anime_zrot_create
OPCode:0x21E6:CMVS::ScriptCommand::sys_object_sprite_anime_zrot_registration
OPCode:0x21E7:CMVS::ScriptCommand::sys_object_sprite_anime_zrot_start
OPCode:0x21E8:CMVS::ScriptCommand::sys_object_sprite_anime_zrot_release
OPCode:0x21E9:CMVS::ScriptCommand::sys_object_sprite_anime_zrot_get_status
OPCode:0x21EA:CMVS::ScriptCommand::sys_object_sprite_anime_trans_create
OPCode:0x21EB:CMVS::ScriptCommand::sys_object_sprite_anime_trans_registration
OPCode:0x21EC:CMVS::ScriptCommand::sys_object_sprite_anime_trans_start
OPCode:0x21ED:CMVS::ScriptCommand::sys_object_sprite_anime_trans_release
OPCode:0x21EE:CMVS::ScriptCommand::sys_object_sprite_anime_trans_get_status
OPCode:0x21EF:CMVS::ScriptCommand::sys_object_sprite_anime_pattern_create
OPCode:0x21F0:CMVS::ScriptCommand::sys_object_sprite_anime_pattern_registration
OPCode:0x21F1:CMVS::ScriptCommand::sys_object_sprite_anime_pattern_start
OPCode:0x21F2:CMVS::ScriptCommand::sys_object_sprite_anime_pattern_release
OPCode:0x21F3:CMVS::ScriptCommand::sys_object_sprite_anime_pattern_get_status
OPCode:0x21F4:CMVS::ScriptCommand::sys_object_sprite_anime_move_create
OPCode:0x21F5:CMVS::ScriptCommand::sys_object_sprite_anime_move_registration
OPCode:0x21F6:CMVS::ScriptCommand::sys_object_sprite_anime_move_start
OPCode:0x21F7:CMVS::ScriptCommand::sys_object_sprite_anime_move_release
OPCode:0x21F8:CMVS::ScriptCommand::sys_object_sprite_anime_move_get_status
OPCode:0x21F9:CMVS::ScriptCommand::sys_object_sprite_anime_move_3d_create
OPCode:0x21FA:CMVS::ScriptCommand::sys_object_sprite_anime_move_3d_registration
OPCode:0x21FB:CMVS::ScriptCommand::sys_object_sprite_anime_move_3d_start
OPCode:0x21FC:CMVS::ScriptCommand::sys_object_sprite_anime_move_3d_release
OPCode:0x21FD:CMVS::ScriptCommand::sys_object_sprite_anime_move_3d_get_status
OPCode:0x21FE:
OPCode:0x21FF:
OPCode:0x2200:CMVS::MainScriptCtrl::SetMelcodeDecodeTypeA
OPCode:0x2201:CMVS::MainScriptCtrl::SetMelcodeDecodeTypeB
OPCode:0x2202:CMVS::MainScriptCtrl::SetMelcodeDecodeTypeC
OPCode:0x2203:
OPCode:0x2204:
OPCode:0x2205:
OPCode:0x2206:
OPCode:0x2207:
OPCode:0x2208:
OPCode:0x2209:
OPCode:0x220A:
OPCode:0x220B:
OPCode:0x220C:CMVS::ScriptCommand::sys_effect_shake_run_get
OPCode:0x220D:
OPCode:0x220E:
OPCode:0x220F:
OPCode:0x2210:CMVS::ScriptCommand::sys_select_button_create
OPCode:0x2211:CMVS::ScriptCommand::sys_select_button_release
OPCode:0x2212:CMVS::ScriptCommand::sys_select_button_parts_registration
OPCode:0x2213:CMVS::ScriptCommand::sys_select_button_parts_mouse_pos
OPCode:0x2214:CMVS::ScriptCommand::sys_select_button_parts_texture_map
OPCode:0x2215:CMVS::ScriptCommand::sys_select_button_start
OPCode:0x2216:CMVS::ScriptCommand::sys_select_button_select_no
OPCode:0x2217:CMVS::ScriptCommand::sys_select_button_frame_task
OPCode:0x2218:CMVS::ScriptCommand::sys_select_button_left_right_effective
OPCode:0x2219:CMVS::ScriptCommand::sys_select_button_key_make
OPCode:0x221A:CMVS::ScriptCommand::sys_select_button_active_no
OPCode:0x221B:CMVS::ScriptCommand::sys_select_button_create_ex
OPCode:0x221C:
OPCode:0x221D:
OPCode:0x221E:
OPCode:0x221F:
OPCode:0x2220:CMVS::ScriptCommand::sys_effect_fade_create
OPCode:0x2221:CMVS::ScriptCommand::sys_effect_fade_release
OPCode:0x2222:CMVS::ScriptCommand::sys_effect_fade_stop
OPCode:0x2223:CMVS::ScriptCommand::sys_effect_fade_status
OPCode:0x2224:CMVS::ScriptCommand::sys_effect_fade_wait
OPCode:0x2225:CMVS::ScriptCommand::sys_effect_shake_create
OPCode:0x2226:CMVS::ScriptCommand::sys_effect_shake_check
OPCode:0x2227:CMVS::ScriptCommand::sys_effect_shake_run_mode
OPCode:0x2228:CMVS::ScriptCommand::sys_effect_shake_stop_wait
OPCode:0x2229:CMVS::ScriptCommand::sys_effect_shake_stop
OPCode:0x222A:CMVS::ScriptCommand::sys_effect_mask_file_open
OPCode:0x222B:CMVS::ScriptCommand::sys_effect_mask_wait
OPCode:0x222C:CMVS::ScriptCommand::sys_effect_mask_status
OPCode:0x222D:CMVS::ScriptCommand::sys_effect_mask_release
OPCode:0x222E:CMVS::ScriptCommand::sys_effect_mask_file_bg_create
OPCode:0x222F:CMVS::ScriptCommand::sys_effect_shutter_create
OPCode:0x2230:CMVS::ScriptCommand::sys_effect_shutter_stop
OPCode:0x2231:CMVS::ScriptCommand::sys_effect_shutter_release
OPCode:0x2232:CMVS::ScriptCommand::sys_effect_shutter_status
OPCode:0x2233:CMVS::ScriptCommand::sys_effect_shutter_wait
OPCode:0x2234:CMVS::ScriptCommand::sys_effect_mask_alpha_file_open
OPCode:0x2235:CMVS::ScriptCommand::sys_effect_mask_alpha_wait
OPCode:0x2236:CMVS::ScriptCommand::sys_effect_mask_alpha_status
OPCode:0x2237:CMVS::ScriptCommand::sys_effect_mask_alpha_release
OPCode:0x2238:CMVS::ScriptCommand::sys_vp_bgm_fade_set_volume
OPCode:0x2239:CMVS::ScriptCommand::sys_vp_bgm_fade_get_volume
OPCode:0x223A:CMVS::ScriptCommand::sys_vp_bgm_fade_set_fade_time
OPCode:0x223B:CMVS::ScriptCommand::sys_vp_bgm_fade_get_fade_time
OPCode:0x223C:CMVS::ScriptCommand::sys_vp_bgm_fade_set_stop_wait
OPCode:0x223D:CMVS::ScriptCommand::sys_vp_bgm_fade_get_stop_wait
OPCode:0x223E:CMVS::ScriptCommand::sys_vp_bgm_fade_set_fade_use_flag
OPCode:0x223F:CMVS::ScriptCommand::sys_vp_bgm_fade_get_fade_use_flag
OPCode:0x2240:CMVS::ScriptCommand::sys_pcm_fade_open
OPCode:0x2241:CMVS::ScriptCommand::sys_pcm_release
OPCode:0x2242:CMVS::ScriptCommand::sys_pcm_status
OPCode:0x2243:CMVS::ScriptCommand::sys_pcm_power_level
OPCode:0x2244:CMVS::ScriptCommand::sys_pcm_sub_volume
OPCode:0x2245:
OPCode:0x2246:
OPCode:0x2247:
OPCode:0x2248:CMVS::ScriptCommand::sys_wave_ex_open
OPCode:0x2249:CMVS::ScriptCommand::sys_wave_ex_release
OPCode:0x224A:CMVS::ScriptCommand::sys_wave_ex_play
OPCode:0x224B:CMVS::ScriptCommand::sys_wave_ex_stop
OPCode:0x224C:
OPCode:0x224D:
OPCode:0x224E:
OPCode:0x224F:CMVS::ScriptCommand::sys_wave_ex_joint_play
OPCode:0x2250:CMVS::ScriptCommand::sys_real_3d_mode
OPCode:0x2251:
OPCode:0x2252:
OPCode:0x2253:
OPCode:0x2254:
OPCode:0x2255:
OPCode:0x2256:
OPCode:0x2257:
OPCode:0x2258:
OPCode:0x2259:
OPCode:0x225A:
OPCode:0x225B:
OPCode:0x225C:
OPCode:0x225D:
OPCode:0x225E:
OPCode:0x225F:
OPCode:0x2260:CMVS::ScriptCommand::sys_real_camera_xyz
OPCode:0x2261:CMVS::ScriptCommand::sys_real_camera_lookup
OPCode:0x2262:CMVS::ScriptCommand::sys_real_camera_direction
OPCode:0x2263:CMVS::ScriptCommand::sys_real_camera_rotation
OPCode:0x2264:CMVS::ScriptCommand::sys_real_camera_range
OPCode:0x2265:
OPCode:0x2266:
OPCode:0x2267:
OPCode:0x2268:
OPCode:0x2269:
OPCode:0x226A:
OPCode:0x226B:
OPCode:0x226C:
OPCode:0x226D:
OPCode:0x226E:
OPCode:0x226F:
OPCode:0x2270:CMVS::ScriptCommand::sys_real_camera_task_stop
OPCode:0x2271:CMVS::ScriptCommand::sys_real_camera_task_wait
OPCode:0x2272:CMVS::ScriptCommand::sys_real_camera_task_status
OPCode:0x2273:CMVS::ScriptCommand::sys_real_camera_task_move_create
OPCode:0x2274:CMVS::ScriptCommand::sys_real_camera_task_rotation_create
OPCode:0x2275:CMVS::ScriptCommand::sys_real_camera_task_move_rotation_create
OPCode:0x2276:
OPCode:0x2277:
OPCode:0x2278:
OPCode:0x2279:
OPCode:0x227A:
OPCode:0x227B:
OPCode:0x227C:
OPCode:0x227D:
OPCode:0x227E:
OPCode:0x227F:
OPCode:0x2280:CMVS::ScriptCommand::sys_frame_task_init
OPCode:0x2281:CMVS::ScriptCommand::sys_frame_task_get_status
OPCode:0x2282:CMVS::ScriptCommand::sys_frame_task_start
OPCode:0x2283:CMVS::ScriptCommand::sys_frame_task_stop
OPCode:0x2284:CMVS::ScriptCommand::sys_frame_task_compulsion_clear
OPCode:0x2285:CMVS::ScriptCommand::sys_frame_task_get_count
OPCode:0x2286:CMVS::ScriptCommand::sys_frame_task_registration_wait_timer
OPCode:0x2287:CMVS::ScriptCommand::sys_frame_task_registration_quasi_camera
OPCode:0x2288:CMVS::ScriptCommand::sys_frame_task_registration_object_double_trans
OPCode:0x2289:CMVS::ScriptCommand::sys_frame_task_registration_object_3d_move
OPCode:0x228A:CMVS::ScriptCommand::sys_frame_task_registration_object_3d_ctrl
OPCode:0x228B:CMVS::ScriptCommand::sys_frame_task_registration_message_object_double_trans
OPCode:0x228C:CMVS::ScriptCommand::sys_save_file_ctrl_move
OPCode:0x228D:CMVS::ScriptCommand::sys_save_file_ctrl_delete
OPCode:0x228E:CMVS::ScriptCommand::sys_save_file_ctrl_delete2
OPCode:0x228F:
OPCode:0x2290:CMVS::ScriptCommand::sys_object_effect_ex_snow_bg
OPCode:0x2291:CMVS::ScriptCommand::sys_object_effect_ex_snow_camera
OPCode:0x2292:CMVS::ScriptCommand::sys_object_effect_ex_gate
OPCode:0x2293:CMVS::ScriptCommand::sys_object_effect_ex_light_feather
OPCode:0x2294:CMVS::ScriptCommand::sys_object_effect_ex_magic1
OPCode:0x2295:CMVS::ScriptCommand::sys_object_effect_ex_game_play
OPCode:0x2296:CMVS::ScriptCommand::sys_object_effect_ex_magic2
OPCode:0x2297:CMVS::ScriptCommand::sys_object_effect_ex_magic3
OPCode:0x2298:
OPCode:0x2299:
OPCode:0x229A:
OPCode:0x229B:
OPCode:0x229C:CMVS::ScriptCommand::sys_save_suspend_resume_use
OPCode:0x229D:CMVS::ScriptCommand::sys_save_suspend_resume_enable
OPCode:0x229E:
OPCode:0x229F:
OPCode:0x22A0:CMVS::ScriptCommand::sys_object_effect_ex_release
OPCode:0x22A1:CMVS::ScriptCommand::sys_object_effect_ex_rain
OPCode:0x22A2:CMVS::ScriptCommand::sys_object_effect_ex_real_snow
OPCode:0x22A3:CMVS::ScriptCommand::sys_object_effect_ex_snow
OPCode:0x22A4:CMVS::ScriptCommand::sys_object_effect_ex_rain2
OPCode:0x22A5:CMVS::ScriptCommand::sys_object_effect_ex_cherry
OPCode:0x22A6:CMVS::ScriptCommand::sys_object_effect_ex_snow2
OPCode:0x22A7:CMVS::ScriptCommand::sys_object_effect_ex_snow3
OPCode:0x22A8:CMVS::ScriptCommand::sys_object_effect_ex_collapse
OPCode:0x22A9:
OPCode:0x22AA:
OPCode:0x22AB:
OPCode:0x22AC:
OPCode:0x22AD:
OPCode:0x22AE:CMVS::ScriptCommand::sys_object_effect_ex_status
OPCode:0x22AF:CMVS::ScriptCommand::sys_object_effect_ex_command
OPCode:0x22B0:CMVS::ScriptCommand::sys_save_file_sam_image_size
OPCode:0x22B1:CMVS::ScriptCommand::sys_save_file_message_log_count
OPCode:0x22B2:CMVS::ScriptCommand::sys_save_image_create
OPCode:0x22B3:CMVS::ScriptCommand::sys_save_menu_create
OPCode:0x22B4:CMVS::ScriptCommand::sys_save_menu_use_flags
OPCode:0x22B5:CMVS::ScriptCommand::sys_save_file_open
OPCode:0x22B6:CMVS::ScriptCommand::sys_save_file_write
OPCode:0x22B7:CMVS::ScriptCommand::sys_save_file_new_data_no
OPCode:0x22B8:CMVS::ScriptCommand::sys_save_sam_image_create
OPCode:0x22B9:CMVS::ScriptCommand::sys_save_file_set_user_data
OPCode:0x22BA:CMVS::ScriptCommand::sys_save_file_set_user_data_ex
OPCode:0x22BB:CMVS::ScriptCommand::sys_save_file_check
OPCode:0x22BC:CMVS::ScriptCommand::sys_save_file_set_comment_text
OPCode:0x22BD:CMVS::ScriptCommand::sys_save_file_get_image_data
OPCode:0x22BE:CMVS::ScriptCommand::sys_save_file_write_user_data_ex
OPCode:0x22BF:
OPCode:0x22C0:CMVS::ScriptCommand::sys_alpha_protect_check
OPCode:0x22C1:CMVS::ScriptCommand::sys_alpha_protect_check2
OPCode:0x22C2:
OPCode:0x22C3:
OPCode:0x22C4:CMVS::ScriptCommand::sys_double_trans_effect_create
OPCode:0x22C5:CMVS::ScriptCommand::sys_double_trans_effect_release
OPCode:0x22C6:CMVS::ScriptCommand::sys_double_trans_effect_status
OPCode:0x22C7:CMVS::ScriptCommand::sys_double_trans_effect_key_wait
OPCode:0x22C8:CMVS::ScriptCommand::sys_aspect_ratio_full_screen_get_use
OPCode:0x22C9:CMVS::ScriptCommand::sys_aspect_ratio_full_screen_set_run_mode_wait
OPCode:0x22CA:CMVS::ScriptCommand::sys_aspect_ratio_full_screen_get_run_mode
OPCode:0x22CB:CMVS::ScriptCommand::sys_display_back_color
OPCode:0x22CC:
OPCode:0x22CD:CMVS::ScriptCommand::sys_popup_memu_set_use
OPCode:0x22CE:CMVS::ScriptCommand::sys_popup_memu_get_use
OPCode:0x22CF:CMVS::ScriptCommand::sys_popup_memu_set_one_clear
OPCode:0x22D0:CMVS::ScriptCommand::sys_cmvs_voice_cut_status
OPCode:0x22D1:CMVS::ScriptCommand::sys_cmvs_display_effect_status
OPCode:0x22D2:CMVS::ScriptCommand::sys_cmvs_wait_timing_status
OPCode:0x22D3:CMVS::ScriptCommand::sys_cmvs_dlg_load_status
OPCode:0x22D4:CMVS::ScriptCommand::sys_cmvs_dlg_save_status
OPCode:0x22D5:CMVS::ScriptCommand::sys_cmvs_active_draw_status
OPCode:0x22D6:CMVS::ScriptCommand::sys_cmvs_bgm_use_status
OPCode:0x22D7:CMVS::ScriptCommand::sys_cmvs_se_use_status
OPCode:0x22D8:CMVS::ScriptCommand::sys_cmvs_voice_use_status
OPCode:0x22D9:CMVS::ScriptCommand::sys_cmvs_master_volume_status
OPCode:0x22DA:CMVS::ScriptCommand::sys_cmvs_bgm_volume_status
OPCode:0x22DB:CMVS::ScriptCommand::sys_cmvs_se_volume_status
OPCode:0x22DC:CMVS::ScriptCommand::sys_cmvs_voice_volume_status
OPCode:0x22DD:CMVS::ScriptCommand::sys_cmvs_video_volume_status
OPCode:0x22DE:CMVS::ScriptCommand::sys_cmvs_system_font_status
OPCode:0x22DF:CMVS::ScriptCommand::sys_cmvs_text_gang_status
OPCode:0x22E0:CMVS::ScriptCommand::sys_cmvs_text_bold_status
OPCode:0x22E1:CMVS::ScriptCommand::sys_cmvs_text_wait_status
OPCode:0x22E2:CMVS::ScriptCommand::sys_cmvs_text_effect_type_status
OPCode:0x22E3:CMVS::ScriptCommand::sys_cmvs_text_effect_timer_status
OPCode:0x22E4:CMVS::ScriptCommand::sys_cmvs_message_read_status
OPCode:0x22E5:CMVS::ScriptCommand::sys_cmvs_message_auto_use_status
OPCode:0x22E6:CMVS::ScriptCommand::sys_cmvs_message_auto_voice_status
OPCode:0x22E7:CMVS::ScriptCommand::sys_cmvs_message_auto_base_timer_status
OPCode:0x22E8:CMVS::ScriptCommand::sys_cmvs_message_auto_one_timer_status
OPCode:0x22E9:CMVS::ScriptCommand::sys_cmvs_mouse_auto_clear_status
OPCode:0x22EA:CMVS::ScriptCommand::sys_cmvs_video_frame_ctrl_status
OPCode:0x22EB:CMVS::ScriptCommand::sys_cmvs_message_skip_use_status
OPCode:0x22EC:CMVS::ScriptCommand::sys_cmvs_window_full_move_status
OPCode:0x22ED:
OPCode:0x22EE:CMVS::ScriptCommand::sys_cmvs_save_use_status
OPCode:0x22EF:CMVS::ScriptCommand::sys_cmvs_message_skip_total_status
OPCode:0x22F0:CMVS::ScriptCommand::sys_custom_key_get_option_mode
OPCode:0x22F1:CMVS::ScriptCommand::sys_custom_key_clear_option_mode
OPCode:0x22F2:CMVS::ScriptCommand::sys_custom_key_get_save_mode
OPCode:0x22F3:CMVS::ScriptCommand::sys_custom_key_clear_save_mode
OPCode:0x22F4:CMVS::ScriptCommand::sys_custom_key_get_load_mode
OPCode:0x22F5:CMVS::ScriptCommand::sys_custom_key_clear_load_mode
OPCode:0x22F6:
OPCode:0x22F7:
OPCode:0x22F8:CMVS::ScriptCommand::sys_cmvs_message_auto_wait_timer_status
OPCode:0x22F9:
OPCode:0x22FA:
OPCode:0x22FB:
OPCode:0x22FC:CMVS::ScriptCommand::sys_aspect_ratio_get_mode
OPCode:0x22FD:CMVS::ScriptCommand::sys_aspect_ratio_set_mode
OPCode:0x22FE:
OPCode:0x22FF:CMVS::ScriptCommand::sys_aspect_ratio_others_display_get_use
OPCode:0x2300:CMVS::ScriptCommand::sys_render_target_create
OPCode:0x2301:CMVS::ScriptCommand::sys_render_target_release
OPCode:0x2302:
OPCode:0x2303:
OPCode:0x2304:CMVS::ScriptCommand::sys_screen_image_capture_registration
OPCode:0x2305:CMVS::ScriptCommand::sys_screen_image_capture_release
OPCode:0x2306:CMVS::ScriptCommand::sys_screen_image_capture_flag
OPCode:0x2307:
OPCode:0x2308:CMVS::ScriptCommand::sys_image_video_object_create
OPCode:0x2309:CMVS::ScriptCommand::sys_image_video_list_play
OPCode:0x230A:CMVS::ScriptCommand::sys_image_video_object_release
OPCode:0x230B:CMVS::ScriptCommand::sys_image_video_restart
OPCode:0x230C:CMVS::ScriptCommand::sys_image_video_stop
OPCode:0x230D:CMVS::ScriptCommand::sys_image_video_play_mode
OPCode:0x230E:CMVS::ScriptCommand::sys_image_video_object_create_ex
OPCode:0x230F:CMVS::ScriptCommand::sys_image_video_get_play_no
OPCode:0x2310:CMVS::ScriptCommand::sys_object_texture_layer_create
OPCode:0x2311:CMVS::ScriptCommand::sys_object_texture_layer_release
OPCode:0x2312:CMVS::ScriptCommand::sys_object_texture_layer_update
OPCode:0x2313:CMVS::ScriptCommand::sys_object_texture_layer_mono_color
OPCode:0x2314:CMVS::ScriptCommand::sys_object_texture_layer_power_color
OPCode:0x2315:CMVS::ScriptCommand::sys_object_texture_layer_attribute_create
OPCode:0x2316:
OPCode:0x2317:
OPCode:0x2318:
OPCode:0x2319:
OPCode:0x231A:CMVS::ScriptCommand::sys_image_video_get_status
OPCode:0x231B:CMVS::ScriptCommand::sys_image_video_frame_stop
OPCode:0x231C:CMVS::ScriptCommand::sys_image_video_play_next_no
OPCode:0x231D:CMVS::ScriptCommand::sys_image_video_put_frame_pos
OPCode:0x231E:CMVS::ScriptCommand::sys_image_video_get_frame
OPCode:0x231F:
OPCode:0x2320:CMVS::ScriptCommand::sys_object_sprite_mode_ex
OPCode:0x2321:CMVS::ScriptCommand::sys_object_sprite_texture_ex_mode
OPCode:0x2322:CMVS::ScriptCommand::sys_object_sprite_reverse
OPCode:0x2323:
OPCode:0x2324:
OPCode:0x2325:
OPCode:0x2326:
OPCode:0x2327:
OPCode:0x2328:CMVS::ScriptCommand::sys_object_special_create
OPCode:0x2329:CMVS::ScriptCommand::sys_object_special_release
OPCode:0x232A:
OPCode:0x232B:
OPCode:0x232C:
OPCode:0x232D:
OPCode:0x232E:
OPCode:0x232F:
OPCode:0x2330:CMVS::ScriptCommand::sys_object_map_init
OPCode:0x2331:CMVS::ScriptCommand::sys_object_map_release
OPCode:0x2332:CMVS::ScriptCommand::sys_object_map_create
OPCode:0x2333:
OPCode:0x2334:
OPCode:0x2335:
OPCode:0x2336:
OPCode:0x2337:
OPCode:0x2338:CMVS::ScriptCommand::sys_task_image_create
OPCode:0x2339:CMVS::ScriptCommand::sys_task_image_wait
OPCode:0x233A:CMVS::ScriptCommand::sys_task_image_release
OPCode:0x233B:
OPCode:0x233C:
OPCode:0x233D:
OPCode:0x233E:
OPCode:0x233F:
OPCode:0x2340:CMVS::ScriptCommand::sys_special_high_skip_use
OPCode:0x2341:
OPCode:0x2342:
OPCode:0x2343:
OPCode:0x2344:CMVS::ScriptCommand::sys_custom_key_get_menu_up
OPCode:0x2345:CMVS::ScriptCommand::sys_custom_key_clear_menu_up
OPCode:0x2346:CMVS::ScriptCommand::sys_custom_key_get_menu_down
OPCode:0x2347:CMVS::ScriptCommand::sys_custom_key_clear_menu_down
OPCode:0x2348:CMVS::ScriptCommand::sys_custom_key_get_outer_move
OPCode:0x2349:CMVS::ScriptCommand::sys_custom_key_clear_outer_move
OPCode:0x234A:CMVS::ScriptCommand::sys_custom_key_get_message_ret_key
OPCode:0x234B:CMVS::ScriptCommand::sys_custom_key_clear_message_ret_key
OPCode:0x234C:
OPCode:0x234D:CMVS::ScriptCommand::sys_custom_key_use_function
OPCode:0x234E:CMVS::ScriptCommand::sys_custom_key_get_function
OPCode:0x234F:CMVS::ScriptCommand::sys_custom_key_clear_function
OPCode:0x2350:CMVS::ScriptCommand::sys_mouse_gesture_use
OPCode:0x2351:CMVS::ScriptCommand::sys_mouse_gesture_enable_button
OPCode:0x2352:CMVS::ScriptCommand::sys_mouse_gesture_enable
OPCode:0x2353:CMVS::ScriptCommand::sys_mouse_gesture_get_index
OPCode:0x2354:CMVS::ScriptCommand::sys_mouse_gesture_clear
OPCode:0x2355:CMVS::ScriptCommand::sys_mouse_gesture_check
OPCode:0x2356:CMVS::ScriptCommand::sys_mouse_gesture_enable_sub
OPCode:0x2357:CMVS::ScriptCommand::sys_mouse_gesture_enable_sub2
OPCode:0x2358:CMVS::ScriptCommand::sys_mouse_gesture_enable_sub3
OPCode:0x2359:CMVS::ScriptCommand::sys_mouse_gesture_get_wheel
OPCode:0x235A:CMVS::ScriptCommand::sys_mouse_gesture_set_angle
OPCode:0x235B:CMVS::ScriptCommand::sys_mouse_gesture_set_length
OPCode:0x235C:CMVS::ScriptCommand::sys_custom_key_get_decision_down
OPCode:0x235D:CMVS::ScriptCommand::sys_custom_key_get_cancel_down
OPCode:0x235E:
OPCode:0x235F:
OPCode:0x2360:CMVS::ScriptCommand::sys_mouse_gesture_get_preview
OPCode:0x2361:CMVS::ScriptCommand::sys_mouse_gesture_set_preview_timer
OPCode:0x2362:CMVS::ScriptCommand::sys_mouse_gesture_get_status
OPCode:0x2363:
OPCode:0x2364:CMVS::ScriptCommand::sys_mouse_gesture_set_mode
OPCode:0x2365:CMVS::ScriptCommand::sys_mouse_gesture_get_slide_map_xy
OPCode:0x2366:CMVS::ScriptCommand::sys_mouse_gesture_set_mouse_map
OPCode:0x2367:CMVS::ScriptCommand::sys_mouse_gesture_flick_clear
OPCode:0x2368:CMVS::ScriptCommand::sys_mouse_gesture_get_flick_info
OPCode:0x2369:CMVS::ScriptCommand::sys_mouse_gesture_get_key_down
OPCode:0x236A:CMVS::ScriptCommand::sys_mouse_gesture_get_flick_info2
OPCode:0x236B:CMVS::ScriptCommand::sys_mouse_gesture_set_move
OPCode:0x236C:
OPCode:0x236D:
OPCode:0x236E:
OPCode:0x236F:
OPCode:0x2370:CMVS::ScriptCommand::sys_select_button_ex_create
OPCode:0x2371:CMVS::ScriptCommand::sys_select_button_ex_release
OPCode:0x2372:CMVS::ScriptCommand::sys_select_button_ex_parts_registration
OPCode:0x2373:CMVS::ScriptCommand::sys_select_button_ex_parts_mouse_pos
OPCode:0x2374:CMVS::ScriptCommand::sys_select_button_ex_parts_texture_map
OPCode:0x2375:CMVS::ScriptCommand::sys_select_button_ex_start
OPCode:0x2376:CMVS::ScriptCommand::sys_select_button_ex_select_no
OPCode:0x2377:CMVS::ScriptCommand::sys_select_button_ex_frame_task
OPCode:0x2378:CMVS::ScriptCommand::sys_select_button_ex_left_right_effective
OPCode:0x2379:CMVS::ScriptCommand::sys_select_button_ex_key_make
OPCode:0x237A:CMVS::ScriptCommand::sys_select_button_ex_active_no
OPCode:0x237B:CMVS::ScriptCommand::sys_select_button_ex_create_ex
OPCode:0x237C:CMVS::ScriptCommand::sys_select_button_ex_duplication
OPCode:0x237D:CMVS::ScriptCommand::sys_select_button_ex_mouse_map_xy
OPCode:0x237E:CMVS::ScriptCommand::sys_select_button_ex_key_cursor_invalid
OPCode:0x237F:CMVS::ScriptCommand::sys_select_button_ex_mouse_return_xy
OPCode:0x2380:CMVS::ScriptCommand::sys_text_layer_ex_create
OPCode:0x2381:CMVS::ScriptCommand::sys_text_layer_ex_create_ex
OPCode:0x2382:CMVS::ScriptCommand::sys_text_layer_ex_release
OPCode:0x2383:CMVS::ScriptCommand::sys_text_layer_ex_xy
OPCode:0x2384:CMVS::ScriptCommand::sys_text_layer_ex_fxy
OPCode:0x2385:CMVS::ScriptCommand::sys_text_layer_ex_draw_map
OPCode:0x2386:CMVS::ScriptCommand::sys_text_layer_ex_draw_clear
OPCode:0x2387:CMVS::ScriptCommand::sys_text_layer_ex_font_size
OPCode:0x2388:CMVS::ScriptCommand::sys_text_layer_ex_font_change
OPCode:0x2389:CMVS::ScriptCommand::sys_text_layer_ex_font_color
OPCode:0x238A:CMVS::ScriptCommand::sys_text_layer_ex_confinement_text
OPCode:0x238B:CMVS::ScriptCommand::sys_text_layer_ex_output
OPCode:0x238C:CMVS::ScriptCommand::sys_text_layer_ex_offset
OPCode:0x238D:CMVS::ScriptCommand::sys_text_layer_ex_float_offset
OPCode:0x238E:CMVS::ScriptCommand::sys_text_layer_ex_color_mode
OPCode:0x238F:CMVS::ScriptCommand::sys_text_layer_ex_rank
OPCode:0x2390:CMVS::ScriptCommand::sys_text_layer_ex_line_count
OPCode:0x2391:CMVS::ScriptCommand::sys_text_layer_ex_to_text_count
OPCode:0x2392:CMVS::ScriptCommand::sys_text_layer_ex_shadow_mode
OPCode:0x2393:CMVS::ScriptCommand::sys_text_layer_ex_shadow_color
OPCode:0x2394:CMVS::ScriptCommand::sys_text_layer_ex_font_bold
OPCode:0x2395:
OPCode:0x2396:CMVS::ScriptCommand::sys_text_layer_ex_disp_range
OPCode:0x2397:CMVS::ScriptCommand::sys_text_layer_ex_alpha_mask
OPCode:0x2398:CMVS::ScriptCommand::sys_text_layer_ex_get_xy
OPCode:0x2399:
OPCode:0x239A:CMVS::ScriptCommand::sys_text_layer_ex_scroll_xy
OPCode:0x239B:CMVS::ScriptCommand::sys_text_layer_ex_scroll_check
OPCode:0x239C:CMVS::ScriptCommand::sys_text_layer_ex_scroll_create
OPCode:0x239D:CMVS::ScriptCommand::sys_text_layer_ex_scroll_set_object
OPCode:0x239E:CMVS::ScriptCommand::sys_text_layer_ex_scroll_map_xy
OPCode:0x239F:CMVS::ScriptCommand::sys_text_layer_ex_scroll_create_ex
OPCode:0x23A0:CMVS::ScriptCommand::sys_text_layer_ex_scroll_release
OPCode:0x23A1:
OPCode:0x23A2:
OPCode:0x23A3:
OPCode:0x23A4:CMVS::ScriptCommand::sys_text_debug_file_open
OPCode:0x23A5:CMVS::ScriptCommand::sys_text_debug_now_text
OPCode:0x23A6:CMVS::ScriptCommand::sys_dialog_text_debug
OPCode:0x23A7:CMVS::ScriptCommand::sys_text_debug_get_no_text
OPCode:0x23A8:CMVS::ScriptCommand::sys_text_debug_file_no_make
OPCode:0x23A9:
OPCode:0x23AA:
OPCode:0x23AB:
OPCode:0x23AC:
OPCode:0x23AD:
OPCode:0x23AE:CMVS::ScriptCommand::sys_jni_java_get_base_folder
OPCode:0x23AF:CMVS::ScriptCommand::sys_jni_java_get_select_folder_dialog
OPCode:0x23B0:CMVS::ScriptCommand::sys_jni_java_create
OPCode:0x23B1:CMVS::ScriptCommand::sys_jni_java_release
OPCode:0x23B2:CMVS::ScriptCommand::sys_jni_java_get_status
OPCode:0x23B3:CMVS::ScriptCommand::sys_jni_java_dmm_authentication
OPCode:0x23B4:CMVS::ScriptCommand::sys_jni_java_dmm_download
OPCode:0x23B5:CMVS::ScriptCommand::sys_jni_java_file_download_check
OPCode:0x23B6:CMVS::ScriptCommand::sys_jni_java_set_base_folder
OPCode:0x23B7:CMVS::ScriptCommand::sys_jni_java_open_alert_dialog
OPCode:0x23B8:
OPCode:0x23B9:
OPCode:0x23BA:
OPCode:0x23BB:
OPCode:0x23BC:
OPCode:0x23BD:
OPCode:0x23BE:CMVS::ScriptCommand::sys_jni_java_create2
OPCode:0x23BF:CMVS::ScriptCommand::sys_jni_java_open_license_text_dialog
OPCode:0x23C0:
OPCode:0x23C1:
OPCode:0x23C2:
OPCode:0x23C3:
OPCode:0x23C4:
OPCode:0x23C5:
OPCode:0x23C6:
OPCode:0x23C7:
OPCode:0x23C8:
OPCode:0x23C9:
OPCode:0x23CA:
OPCode:0x23CB:
OPCode:0x23CC:
OPCode:0x23CD:
OPCode:0x23CE:
OPCode:0x23CF:CMVS::ScriptCommand::sys_null
```

