//セッティングファイルクラス
#pragma once

#include <Siv3D.hpp>
#include "Resource.h"

class settingFile {
private:
	settingFile() {
		{
			BinaryReader reader(_FILE_PATH);
			if (reader) {
				if (reader.read(_volume)) {
					if (reader.read(_width)) return;
				}
			}
		}
		BinaryWriter writer(_FILE_PATH);
		_volume = soundResource::DEF_VOLUME;
		_width = textureResource::WINDOW_DEF_WIDTH;
		if (writer) {
			writer.write(_volume);
			writer.write(_width);
		}
	}
	static inline std::unique_ptr<settingFile> instance = nullptr;

	static inline const String _FILE_PATH = U"Data/setting.bin";

	double _volume;
	double _width;

public:
	static settingFile& get_instance() {
		return (instance == nullptr) ? *(instance = std::make_unique<settingFile>(settingFile())) : *instance;
	}

	double get_volume() const{ return _volume; }
	double get_window_width() const { return _width; }

	bool set_volume(double vol) {
		_volume = vol;
		BinaryWriter writer(_FILE_PATH);
		if (writer) {
			writer.write(_volume);
			writer.write(_width);
			return true;
		}
		return false;
	}
	bool set_window_width(double width) {
		_width = width;
		BinaryWriter writer(_FILE_PATH);
		if (writer) {
			writer.write(_volume);
			writer.write(_width);
			return true;
		}
		return false;
	}
};