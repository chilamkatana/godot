/*************************************************************************/
/*  texture_button.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "texture_button.h"


Size2 TextureButton::get_minimum_size() const {

	Size2 rscale;
	if (normal.is_null()) {
		if (pressed.is_null()) {
			if (hover.is_null())
				if (click_mask.is_null())
					rscale= Size2();
				else
					rscale= click_mask->get_size();
			else
				rscale= hover->get_size();
		} else
			rscale=pressed->get_size();

	} else
		rscale= normal->get_size();

	return rscale*scale.abs();
}


bool TextureButton::has_point(const Point2& p_point) const {

	if (scale[0] == 0 || scale[1] == 0) {
		return false;
	}

	Point2 ppos = p_point/scale.abs();

	if (click_mask.is_valid()) {

		Point2i p =ppos;
		if (p.x<0 || p.x>=click_mask->get_size().width || p.y<0 || p.y>=click_mask->get_size().height)
			return false;

		return click_mask->get_bit(p);
	}

	return Control::has_point(p_point);
}

void TextureButton::_notification(int p_what) {

	switch( p_what ) {

		case NOTIFICATION_DRAW: {
			RID canvas_item = get_canvas_item();
			DrawMode draw_mode = get_draw_mode();
//			if (normal.is_null())
//				break;

			Ref<Texture> texdraw;

			switch (draw_mode) {
				case DRAW_NORMAL: {

					if (normal.is_valid())
						texdraw=normal;
				} break;
				case DRAW_PRESSED: {

					if (pressed.is_null()) {
						if (hover.is_null()) {
							if (normal.is_valid())
								texdraw=normal;
						} else
							texdraw=hover;

					} else
						texdraw=pressed;
				} break;
				case DRAW_HOVER: {

					if (hover.is_null()) {
						if (pressed.is_valid() && is_pressed())
							texdraw=pressed;
						else if (normal.is_valid())
							texdraw=normal;
					} else
						texdraw=hover;
				} break;
				case DRAW_DISABLED: {

					if (disabled.is_null()) {
						if (normal.is_valid())
							texdraw=normal;
					} else
						texdraw=disabled;
				} break;
			}

			// scale and translate sprite & all it's children
			{
				Vector2 scale = Vector2(1, 1);

				Matrix32 mat = get_transform();
				if(draw_mode == DRAW_PRESSED) {
					scale *= pressed_scale;
					Size2 delta = ((pressed_scale - Size2(1, 1)) / 2) * get_item_rect().size;
					mat.translate(-delta);
				}
				mat.scale_basis(scale);

				VisualServer::get_singleton()->canvas_item_set_transform(canvas_item, mat);
			}

			if (texdraw.is_valid()) {
				Rect2 drect(Point2(),texdraw->get_size());
				draw_texture_rect(texdraw,drect,false,modulate);

			}
			if (has_focus() && focused.is_valid()) {

				Rect2 drect(Point2(),focused->get_size());
				draw_texture_rect(focused,drect,false,modulate);

			};

		} break;
	}
}

void TextureButton::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_normal_texture","texture:Texture"),&TextureButton::set_normal_texture);
	ObjectTypeDB::bind_method(_MD("set_pressed_texture","texture:Texture"),&TextureButton::set_pressed_texture);
	ObjectTypeDB::bind_method(_MD("set_hover_texture","texture:Texture"),&TextureButton::set_hover_texture);
	ObjectTypeDB::bind_method(_MD("set_disabled_texture","texture:Texture"),&TextureButton::set_disabled_texture);
	ObjectTypeDB::bind_method(_MD("set_focused_texture","texture:Texture"),&TextureButton::set_focused_texture);
	ObjectTypeDB::bind_method(_MD("set_click_mask","mask:BitMap"),&TextureButton::set_click_mask);
	ObjectTypeDB::bind_method(_MD("set_texture_scale","scale"),&TextureButton::set_texture_scale);
	ObjectTypeDB::bind_method(_MD("set_pressed_scale","scale"),&TextureButton::set_pressed_scale);
	ObjectTypeDB::bind_method(_MD("set_modulate","color"),&TextureButton::set_modulate);

	ObjectTypeDB::bind_method(_MD("get_normal_texture:Texture"),&TextureButton::get_normal_texture);
	ObjectTypeDB::bind_method(_MD("get_pressed_texture:Texture"),&TextureButton::get_pressed_texture);
	ObjectTypeDB::bind_method(_MD("get_hover_texture:Texture"),&TextureButton::get_hover_texture);
	ObjectTypeDB::bind_method(_MD("get_disabled_texture:Texture"),&TextureButton::get_disabled_texture);
	ObjectTypeDB::bind_method(_MD("get_focused_texture:Texture"),&TextureButton::get_focused_texture);
	ObjectTypeDB::bind_method(_MD("get_click_mask:BitMap"),&TextureButton::get_click_mask);
	ObjectTypeDB::bind_method(_MD("get_texture_scale"),&TextureButton::get_texture_scale);
	ObjectTypeDB::bind_method(_MD("get_pressed_scale"),&TextureButton::get_pressed_scale);
	ObjectTypeDB::bind_method(_MD("get_modulate"),&TextureButton::get_modulate);

	ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT,"textures/normal",PROPERTY_HINT_RESOURCE_TYPE,"Texture"), _SCS("set_normal_texture"), _SCS("get_normal_texture"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT,"textures/pressed",PROPERTY_HINT_RESOURCE_TYPE,"Texture"), _SCS("set_pressed_texture"), _SCS("get_pressed_texture"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT,"textures/hover",PROPERTY_HINT_RESOURCE_TYPE,"Texture"), _SCS("set_hover_texture"), _SCS("get_hover_texture"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT,"textures/disabled",PROPERTY_HINT_RESOURCE_TYPE,"Texture"), _SCS("set_disabled_texture"), _SCS("get_disabled_texture"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT,"textures/focused",PROPERTY_HINT_RESOURCE_TYPE,"Texture"), _SCS("set_focused_texture"), _SCS("get_focused_texture"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT,"textures/click_mask",PROPERTY_HINT_RESOURCE_TYPE,"BitMap"), _SCS("set_click_mask"), _SCS("get_click_mask")) ;
	ADD_PROPERTYNO(PropertyInfo(Variant::VECTOR2,"params/scale",PROPERTY_HINT_RANGE,"0.01,1024,0.01"), _SCS("set_texture_scale"), _SCS("get_texture_scale"));
	ADD_PROPERTYNO(PropertyInfo(Variant::COLOR,"params/modulate"), _SCS("set_modulate"), _SCS("get_modulate"));
	ADD_PROPERTYNO(PropertyInfo(Variant::VECTOR2,"params/pressed_scale",PROPERTY_HINT_RANGE,"0.01,1024,0.01"), _SCS("set_pressed_scale"), _SCS("get_pressed_scale"));

}


void TextureButton::set_normal_texture(const Ref<Texture>& p_normal) {

	if (normal.is_valid() && normal->is_connected("changed", this, "update"))
		normal->disconnect("changed",this,"update");
	normal=p_normal;
	if(normal.is_valid() && !normal->is_connected("changed", this, "update"))
		normal->connect("changed",this,"update");

	update();
	minimum_size_changed();
}

void TextureButton::set_pressed_texture(const Ref<Texture>& p_pressed) {

	if(pressed.is_valid() && pressed->is_connected("changed", this, "update"))
		pressed->disconnect("changed",this,"update");
	pressed=p_pressed;
	if(pressed.is_valid() && !pressed->is_connected("changed", this, "update"))
		pressed->connect("changed",this,"update");

	update();
}
void TextureButton::set_hover_texture(const Ref<Texture>& p_hover) {

	if(hover.is_valid() && hover->is_connected("changed", this, "update"))
		hover->disconnect("changed",this,"update");
	hover=p_hover;
	if(hover.is_valid() && !hover->is_connected("changed", this, "update"))
		hover->connect("changed",this,"update");

	update();
}
void TextureButton::set_disabled_texture(const Ref<Texture>& p_disabled) {

	if(disabled.is_valid() && disabled->is_connected("changed", this, "update"))
		disabled->disconnect("changed",this,"update");
	disabled=p_disabled;
	if(disabled.is_valid() && !disabled->is_connected("changed", this, "update"))
		disabled->connect("changed",this,"update");

	update();
}
void TextureButton::set_click_mask(const Ref<BitMap>& p_click_mask) {

	click_mask=p_click_mask;
	update();
}

Ref<Texture> TextureButton::get_normal_texture() const {

	return normal;
}
Ref<Texture> TextureButton::get_pressed_texture() const {

	return pressed;
}
Ref<Texture> TextureButton::get_hover_texture() const {

	return hover;
}
Ref<Texture> TextureButton::get_disabled_texture() const {

	return disabled;
}
Ref<BitMap> TextureButton::get_click_mask() const {

	return click_mask;
}

Ref<Texture> TextureButton::get_focused_texture() const {

	return focused;
};

void TextureButton::set_focused_texture(const Ref<Texture>& p_focused) {

	if (focused.is_valid() && focused->is_connected("changed", this, "update"))
		focused->disconnect("changed",this,"update");
	focused = p_focused;
	if(focused.is_valid() && !focused->is_connected("changed", this, "update"))
		focused->connect("changed",this,"update");

	update();

};

void TextureButton::set_texture_scale(Size2 p_scale) {

	scale=p_scale;
	minimum_size_changed();
	update();
}

Size2 TextureButton::get_texture_scale() const{

	return scale;
}

void TextureButton::set_pressed_scale(Size2 p_scale) {

	pressed_scale=p_scale;
	update();
}

Size2 TextureButton::get_pressed_scale() const{

	return pressed_scale;
}

void TextureButton::set_modulate(const Color& p_modulate) {
	modulate=p_modulate;
	update();
}

Color TextureButton::get_modulate() const {
	return modulate;
}


TextureButton::TextureButton() {
	scale=Size2(1.0, 1.0);
	pressed_scale=Size2(1.0, 1.0);
	modulate=Color(1,1,1);
}
