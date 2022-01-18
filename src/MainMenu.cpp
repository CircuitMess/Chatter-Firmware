#include "MainMenu.h"
#include <Input/Input.h>
#include <Pins.hpp>

const MainMenu::Item MainMenu::Items[] = {
		{ "Friends", "Friends" },
		{ "Profile", "Profile" },
		{ "Inbox", "Inbox" },
		{ "Settings", "Settings" },
};

MainMenu::MainMenu() : LVScreen(){
	lv_obj_set_height(obj, 60);
	lv_obj_set_width(obj, 60);

	left = lv_obj_create(obj);
	mid = lv_obj_create(obj);
	right = lv_obj_create(obj);

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);

	lv_obj_set_width(left, 18);
	lv_obj_set_width(right, 18);
	lv_obj_set_height(left, lv_pct(100));
	lv_obj_set_height(right, lv_pct(100));
	lv_obj_set_height(mid, lv_pct(100));
	lv_obj_set_flex_grow(mid, 1);
	lv_obj_set_scrollbar_mode(mid, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(right, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(left, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_flex_flow(left, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_flow(mid, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_flow(right, LV_FLEX_FLOW_COLUMN);

	lv_obj_set_flex_align(left, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_flex_align(right, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_flex_align(mid, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	for(const auto& item : Items){
		lv_obj_t* bigContainer = lv_obj_create(mid);
		lv_obj_t* bigLabel = lv_img_create(bigContainer);
		lv_obj_t* big = lv_gif_create(bigContainer);
		lv_obj_t* small = lv_img_create(right);

		bigs.push_back(big);
		smalls.push_back(small);

		lv_gif_set_src(big, (String("S:/Menu/Big/") + item.icon + ".gif").c_str());
		lv_gif_set_loop(big, false);

		lv_img_set_src(bigLabel, (String("S:/Menu/Label/") + item.icon + ".bin").c_str());
		lv_img_set_src(small, (String("S:/Menu/Small/") + item.icon + ".bin").c_str());

		lv_obj_set_flex_flow(bigContainer, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(bigContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_set_width(bigContainer, lv_pct(100));
		lv_obj_set_height(bigContainer, lv_pct(100));

		lv_obj_set_style_translate_y(big, -5, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_translate_y(bigLabel, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_translate_y(bigContainer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

		smallAnims.emplace_back();
		lv_anim_t& anim = smallAnims.back();
		lv_anim_init(&anim);
		lv_anim_set_exec_cb(&anim, ease);
		lv_anim_set_var(&anim, small);
		lv_anim_set_time(&anim, 800);
	}

	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", LV_STATE_DEFAULT);
}

void MainMenu::ease(void* var, int32_t value){
	lv_obj_t* obj = (lv_obj_t*) var;

	const float amount = 1.0f;
	float v = (float) value / 100.0f;

	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	const float x =  v < 0.5
			   ? (pow(2.0f * v, 2.0f) * ((c2 + 1.0f) * 2.0f * v - c2)) / 2.0f
			   : (pow(2.0f * v - 2.0f, 2.0f) * ((c2 + 1.0f) * (v * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;

	lv_obj_set_style_translate_x(obj, lv_pct(round(x * amount * 100.0f)), LV_STATE_DEFAULT | LV_PART_MAIN);
}

void MainMenu::startAnim(uint8_t index, bool reverse){
	if(index >= smallAnims.size()) return;

	lv_anim_t& anim = smallAnims[index];
	lv_anim_set_values(&anim, reverse * 100, !reverse * 100);
	lv_anim_start(&anim);

	if(!reverse){
		lv_obj_t* gif = bigs[index];
		lv_gif_restart(gif);
		lv_gif_start(gif);
	}
}

void MainMenu::onStart(){
	Input::getInstance()->addListener(this);

	for(lv_obj_t* small : smalls){
		lv_obj_set_style_translate_x(small, 0, LV_STATE_DEFAULT | LV_PART_MAIN);
	}

	startAnim(0);
}

void MainMenu::onStop(){
	Input::getInstance()->removeListener(this);
}

void MainMenu::buttonPressed(uint i){
	if(i == BTN_LEFT){
		selectPrev();
	}else if(i == BTN_RIGHT){
		selectNext();
	}
}

void MainMenu::selectNext(){
	if(selected + 1 >= sizeof(Items) / sizeof(Items[0])) return;

	startAnim(selected, true);
	selected++;
	startAnim(selected);
	lv_obj_scroll_to(mid, 0, selected * lv_obj_get_height(mid), LV_ANIM_ON);
}

void MainMenu::selectPrev(){
	if(selected == 0) return;

	startAnim(selected, true);
	selected--;
	startAnim(selected);
	lv_obj_scroll_to(mid, 0, selected * lv_obj_get_height(mid), LV_ANIM_ON);
}
