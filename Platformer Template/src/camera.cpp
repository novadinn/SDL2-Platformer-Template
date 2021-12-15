#include "camera.h"

#include "generics.h"
#include "graphics.h"

Camera::Camera(float x, float y, float width, float height) {
	rectangle_.x = generics::round(x);
	rectangle_.y = generics::round(y);
	rectangle_.w = generics::round(width);
	rectangle_.h = generics::round(height);
}

void Camera::update(float player_center_x, float player_center_y) {
	if ((player_center_x - rectangle_.x) / generics::kScreenWidth > 1) {
		rectangle_.x += generics::kScreenWidth;
	} else if ((rectangle_.x - player_center_x) / generics::kScreenWidth > 0) {
		rectangle_.x -= generics::kScreenWidth;
	}
	
	if ((player_center_y - rectangle_.y) / generics::kScreenHeight > 1) {
		rectangle_.y += generics::kScreenHeight;
	} else if ((rectangle_.y - player_center_y) / generics::kScreenHeight > 0) {
		rectangle_.y -= generics::kScreenHeight;
	}
}

void Camera::draw(Graphics& graphics) const {
	graphics.set_offset(-rectangle_.x * 1.0f, -rectangle_.y * 1.0f);
}