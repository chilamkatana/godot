/*************************************************************************/
/*  text_asset.cpp                                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
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
#include "text_asset.h"
#include "os/file_access.h"

TextAsset::TextAsset() {
}

TextAsset::~TextAsset() {
}

void TextAsset::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("load","path"),&TextAsset::load);
	ObjectTypeDB::bind_method(_MD("get_value"),&TextAsset::get_value);
}

Error TextAsset::load(const String& p_path) {

	Vector<uint8_t> file = FileAccess::get_file_as_array(p_path);
	if(file.size() == 0)
		return ERR_FILE_CANT_READ;
	if(value.parse_utf8((const char*)file.ptr(),file.size()))
		return ERR_FILE_UNRECOGNIZED;
	return OK;
}

const String& TextAsset::get_value() {

	return value;
}
