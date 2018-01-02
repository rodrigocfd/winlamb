/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "file.h"
#include "com.h"
#include <Shlobj.h>

namespace wl {

// Utilities to work with zipped files.
class zip final {
private:
	zip() = delete;

public:
	static void extract_all(const std::wstring& zipFile, const std::wstring& destFolder) {
		if (!file::util::exists(zipFile)) {
			throw std::invalid_argument("File doesn't exist.");
		}
		if (!file::util::exists(destFolder)) {
			throw std::invalid_argument("Output directory doesn't exist.");
		}

		// http://social.msdn.microsoft.com/Forums/vstudio/en-US/45668d18-2840-4887-87e1-4085201f4103/visual-c-to-unzip-a-zip-file-to-a-specific-directory
		com::lib comLib{com::lib::init::NOW};

		com::ptr<IShellDispatch> shellDispatch;
		shellDispatch.co_create_instance(CLSID_Shell, IID_IShellDispatch);
		
		com::variant variZipFilePath;
		variZipFilePath.set_str(zipFile);

		com::ptr<Folder> zippedFile;
		com::check_hr(
			shellDispatch->NameSpace(variZipFilePath, &zippedFile),
			"IShellDispatch::NameSpace failed on zip file name");
		
		com::variant variOutFolderPath;
		variOutFolderPath.set_str(destFolder);

		com::ptr<Folder> outFolder;
		com::check_hr(
			shellDispatch->NameSpace(variOutFolderPath, &outFolder),
			"IShellDispatch::NameSpace failed on directory name");

		com::ptr<FolderItems> filesInside;
		com::check_hr(
			zippedFile->Items(&filesInside),
			"Folder::Items failed");

		long fileCount = 0;
		com::check_hr(
			filesInside->get_Count(&fileCount),
			"Folder::get_Count failed");

		com::variant variItem;
		variItem.set_idispatch(filesInside);

		com::variant variOptions;
		variOptions.set_int4(1024 | 512 | 16 | 4); // http://msdn.microsoft.com/en-us/library/bb787866(VS.85).aspx

		com::check_hr(
			outFolder->CopyHere(variItem, variOptions),
			"Folder::CopyHere failed");
	}
};

}//namespace wl