#pragma once
#include <CoreFoundation/CoreFoundation.h>
#include <string>


inline std::string GetResourcePath()
{
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
    char path[PATH_MAX];
    CFURLGetFileSystemRepresentation(resourcesURL, true, (UInt8*)path, PATH_MAX);
    CFRelease(resourcesURL);
    return std::string(path);
}