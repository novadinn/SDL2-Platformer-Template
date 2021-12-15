#pragma once

// TODO: remove
#include <iostream>

namespace generics {
	namespace {
		const int kScreenWidth = 640;
		const int kScreenHeight = 480;

		const int kTileSize = 16;
		const int kTileOffset = 4;
		int round(float val) {
			return (int)(val > 0.0 ? val + 0.5 : val - 0.5);
		}

		inline int floatToInt(float number) {
			return round(number);
		}

		inline float convertTileToFloat(int tile) {
			return tile * kTileSize * 1.0f;
		}

		inline int convertFloatToTile(float number) {
			return (int)(number / kTileSize);
		}
	}
}