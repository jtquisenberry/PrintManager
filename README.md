# PrintManager




# Building Projects

## GhostHarness

### Filesystem

* Copy `\ghostpdl-10.02.1\pcl\pl\plchar.h` to project.
* Copy `\ghostpdl-10.02.1\debugbin\gpdldll64.dll` to project.
* Copy files `gpdldll64.dll`, `gpdldll64.exp`, `gpdldll64.ilk`, `gpdldll64.lib`, `gpdldll64.pdb` from `\ghostpdl-10.02.1\debugbin\` to `..\GhostPDL`.

### Visual Studio Project Properties

* Set C/C++ -> General -> Additional Include Directories = ..\GhostPDL
* Linker -> All Options -> Additional Dependencies prepend gpdldll64.lib;
* Set Linker -> All Options -> Additional Library Directories = ..\GhostPDL

### Project Files
* Rename GhostHarness.c to GhostHarness.cpp

### Code
* Add `#define GHOSTPDL` near the top of GhostHarness.cpp
* Replace
``` c++
/* This can work either with the Ghostscript or GhostPDL
 * library. The code is the same for both. */
#if GHOSTPDL
#include "pcl/pl/plapi.h"   /* GSAPI - gpdf version */
#else
#include "psi/iapi.h"       /* GSAPI - ghostscript version */
#endif
```
with
``` c++
#ifdef __cplusplus
extern "C" {
    #include "plapi.h"  /* GSAPI - gpdf version */
}
#endif
```
* Replace
``` c++
char* argv[10];
```
with
``` c++
const char* argv[10];
```
* Replace
``` c++
code = gsapi_init_with_args(instance, argc, argv);
```
with
``` c++
code = gsapi_init_with_args(instance, argc, (char**)argv);
```
* Replace
``` c++
char* in_files[] =
```
with
``` c++
const char* in_files[] =
```
* Replace
``` c++
td[i].in_file = in_files[i % NUM_INPUTS];
```
with
``` c++
td[i].in_file = (char*)in_files[i % NUM_INPUTS];
```

# Credits and References

Crow, D. (2010, April 20). <i>Ask for, and receive, change notifications from a printer.</i> CodeProject. https://www.codeproject.com/Articles/74359/Ask-for-and-receive-change-notifications-from-a-pr

<blockquote>The article discusses how to retrieve print job status in Windows using FindFirstPrinterChangeNotification. The article includes a Visual C++ 6.0 project. A portion of the print monitoring and MFC user interface code was adapted into this project. The article and code were published under the Code Project Open License (CPOL) 1.02.</blockquotes>

Artifex Software, Inc. (2023, November 1). <i>Ghostscript release 10.02.1.</i> Ghostscript. https://www.ghostscript.com/releases/gpdldnld.html. Alternatively, https://github.com/ArtifexSoftware/ghostpdl-downloads/releases.

<blockquote>This repository includes object code built from the GhostPDL source. I provide a complete copy of the source code at https://github.com/jtquisenberry/ghostpdl. The code was published under the AGPL 3.0 license. </blockquote>




 
