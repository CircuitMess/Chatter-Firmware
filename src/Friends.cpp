#include "Friends.h"
#include "Model/Profile.hpp"
#include "font.h"
#include "ListItem.h"

extern std::vector<Profile> friends;

Friends::Friends() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	auto listItem = new ListItem(obj,"Add friend",0,1);
	lv_obj_set_style_bg_opa(listItem->getLvObj(),0,0);
	lv_group_add_obj(inputGroup, listItem->getLvObj());

	for(const Profile& profile : friends){
		lv_group_add_obj(inputGroup, (new User(obj, profile))->getLvObj());
	}

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		lv_obj_t* focused = lv_group_get_focused(group);
		lv_obj_scroll_to_view(focused, LV_ANIM_ON);
	});

}