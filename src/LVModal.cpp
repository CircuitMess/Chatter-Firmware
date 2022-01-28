#include "LVModal.h"
#include "InputLVGL.h"

LVModal::LVModal(LVScreen* parent) : LVObject(parent->getLvObj()), parentScreen(parent){
	inputGroup = lv_group_create();

	container = lv_obj_create(parent->getLvObj());
	lv_obj_set_parent(obj, container);

	lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

	lv_obj_add_flag(container, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_align(container, LV_ALIGN_CENTER);
	lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_style_border_color(container, lv_color_black(), 0);
	lv_obj_set_style_border_width(container, 2, 0);
	lv_obj_set_style_border_opa(container, LV_OPA_20, 0);
}

LVModal::~LVModal(){
	lv_group_del(inputGroup);
}

void LVModal::start(){
	lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), inputGroup);

	active = true;
	lv_obj_invalidate(container);

	onStart();
}

void LVModal::stop(){
	lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), parentScreen->getInputGroup());
	active = false;

	onStop();
}

bool LVModal::isActive() const{
	return active;
}

void LVModal::onStart(){

}

void LVModal::onStop(){

}
