#include "Inbox.h"
#include "UserWithMessage.h"
#include "font.h"
#include "ListItem.h"

extern std::vector<Profile> friends;

Inbox::Inbox() : LVScreen(){
	lv_obj_set_height(obj, LV_SIZE_CONTENT);
	lv_obj_set_layout(obj,LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_pad_all(obj, 3, 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_bg_img_src(obj, "S:/bg.bin", 0);

	auto listItem = new ListItem(obj,"New conversation",0,1);
	lv_obj_set_style_bg_opa(listItem->getLvObj(),0,0);
	lv_group_add_obj(inputGroup, listItem->getLvObj());

	/*lv_obj_add_event_cb(listItem, [](lv_event_t* event){
		static_cast<Inbox*>(event->user_data)->newConvo();
	}, LV_EVENT_CLICKED, this);*/

	for(const Profile& profile : friends){
		auto user = new UserWithMessage(obj, profile, "Lorem ipsum dolor sit amet consequentur");
		lv_group_add_obj(inputGroup, user->getLvObj());
		lv_obj_add_event_cb(user->getLvObj(), [](lv_event_t* event){
			static_cast<Inbox*>(event->user_data)->openConvo(lv_obj_get_index(lv_event_get_target(event)));
		}, LV_EVENT_CLICKED, this);
		userElements.push_back(user);
	}

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* group){
		lv_obj_t* focused = lv_group_get_focused(group);
		lv_obj_scroll_to_view(focused, LV_ANIM_ON);
		lv_obj_invalidate(lv_obj_get_parent(focused));
	});

}

void Inbox::openConvo(uint32_t index){
	Serial.println(index);
	//TODO - staviti otvaranje Convo contexta

}

void Inbox::newConvo(){
	Serial.println("new convo");
	//TODO - staviti otvaranje menija za odabir prijatelja
}
