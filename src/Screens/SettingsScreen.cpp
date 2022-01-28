#include "SettingsScreen.h"

SettingsScreen::SettingsScreen() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	//sound ON/OFF
	sound = lv_obj_create(obj);
	lv_obj_set_height(sound, LV_SIZE_CONTENT);
	lv_obj_set_width(sound, lv_pct(100));
	lv_obj_set_layout(sound, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(sound, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(sound, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(sound, 30, 0);
	lv_obj_set_style_pad_all(sound, 3, 0);
	lv_obj_set_style_border_width(sound, 1, 0);
	lv_obj_set_style_border_color(sound, lv_color_white(), 0);
	lv_obj_set_style_bg_opa(sound,0,0);

	lv_obj_t* soundLabel = lv_label_create(sound);
	lv_obj_set_style_text_font(soundLabel, &pixelbasic_7, 0);
	lv_obj_set_style_text_color(soundLabel, lv_color_white(), 0);
	lv_label_set_text(soundLabel, "Sound");

	lv_obj_t* soundSwitch = lv_switch_create(sound);
	lv_obj_add_state(soundSwitch, LV_STATE_DISABLED);

	lv_obj_set_style_border_width(soundSwitch,1,0);
	lv_obj_set_style_border_color(soundSwitch,lv_color_white(),0);
	lv_obj_set_style_bg_opa(soundSwitch,0,0);

	//sleepTime
	sleepTime = lv_obj_create(obj);
	lv_obj_set_height(sleepTime, LV_SIZE_CONTENT);
	lv_obj_set_width(sleepTime, lv_pct(100));
	lv_obj_set_layout(sleepTime, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(sleepTime, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(sleepTime, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(sleepTime, 65, 0);
	lv_obj_set_style_pad_all(sleepTime, 3, 0);
	lv_obj_set_style_border_width(sleepTime, 1, 0);
	lv_obj_set_style_border_color(sleepTime, lv_color_white(), 0);
	lv_obj_set_style_bg_opa(sleepTime,0,0);

	lv_obj_t* sleepLabel = lv_label_create(sleepTime);
	lv_obj_set_style_text_font(sleepLabel, &pixelbasic_7, 0);
	lv_obj_set_style_text_color(sleepLabel, lv_color_white(), 0);
	lv_label_set_text(sleepLabel, "Sleep Timeout");

	/*lv_obj_t* soundSwitch = lv_switch_create(sound);
	lv_obj_add_state(soundSwitch, LV_STATE_DISABLED);

	lv_obj_set_style_border_width(soundSwitch,1,0);
	lv_obj_set_style_border_color(soundSwitch,lv_color_white(),0);
	lv_obj_set_style_bg_opa(soundSwitch,0,0);*/
}