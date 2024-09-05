# Print Manager

Print Manager is an application to manage print queues and print jobs in Windows. These are its major functions:

* Reports added and changed print jobs in real time.
* Copies and redirects a print job to one or more print queues.
* Pauses, resumes, and purges print queues.
* Reports printer properties.
* Reports print driver properties.

## Screenshot

![UI](images/PrintManager_main.png?raw=true "UI")

# Requirements

* Windows 10+, 64-bit, x64.

# Usage

```
> PrintManager.exe
```

## Report Jobs

To report added and changed jobs in real time, do the following:
* Select a printer to monitor from the "Printer to monitor" combobox. 
* Click Start.
* Use another application to print a document to the selected printer.

![Monitored Jobs](images/PrintManager_job_list.png?raw=true "Monitored Jobs")

For each job in the selected print queue, these properties are reported:

* __Job Id__: Unique identifier of a job in a print server.
* __User__: The username of the user who submitted the print job.
* __From__: The port and machine from which the print job was submitted.
* __Document__: The title of the document printed.
* __Submitted @__: The time when the job was submitted.
* __Pages printed__: The number of pages printed out of the number of pages in the document.
* __Bytes printed__: The number of bytes printed out of the number of bytes in the spool file.
* __Status__: The status of the print job.

### Technical

Capture of print jobs in real time is accomplished with Windows API functions `FindFirstPrinterChangeNotification` and `FindNextPrinterChangeNotification`.

## Duplicate and Redirect Print Jobs

To duplicate and redirect a print job, do the following:

* Select a printer whose jobs should be redirected from the "Printer to redirect" combobox.
* Select one or more printers to receive the duplicate print job from the listbox below.
* Click "Redirect Printer".

![Redirect Printer](images/PrintManager_printer_list.png?raw=true "Redirect Printer")

### Technical

If a user submits a print job to the printer to redirect, that printer will print the job. Print Manager then copies the spool file from `C:\Windows\System32\spool\PRINTERS` to the location specified in the "Intermediate Directory" text box. The Ghostscript driver `mswinpr2` converts the spool file (.SPL) to a bitmap and draws the bitmap to a device context. For each target printer, a new print job is created. During that print job, the bitmap is blitted from the device context to a device context specific to the printer.


## Pause, Resume, and Purge

To control the printer status, select one or more printers from the printers list control. Click one of "Pause Printers", "Resume Printers", and "Purge Printers". 

* __Pause Printers__: Pausing a printer allows the print queue to receive spooled documents but prevents it from sending the document to the printer.
* __Resume Printers__: Resuming a printer allows the print queue to send documents to a printer.
* __Purge Printers__: Purging a print queue removes all documents from it, including printed documents.

### Technical

Controlling the printer status is accomplished with Windows API `SetPrinterStatus`.

## Drivers

To view printer drivers, do the following:

* Click on the "Drivers" tab.

![Redirect Printer](images/PrintManager_drivers.png?raw=true "Redirect Printer")

# Logging

Print Manager cretes two logs in `%USERPROFILE%`: `print_manager_objects.txt` and `print_manager_system.txt`. 

## `print_manager_objects.txt`

The `print_manager_objects.txt` log reports information about print jobs.

```
JobId                          3
PrinterName                    Bullzip PDF Printer
MachineName                    \\MYPC01
PortName                       
UserName                       MyUser
NotifyName                     MyUser
Datatype                       RAW
PrintProcessor                 winprint
Parameters                     
DriverName                     Bullzip PDF Printer
Devmode                        DEVMODE PLACEHOLDER
Status                         128
StatusAsText                   Job has printed.
StatusString                   
SecurityDescriptor             UNSUPPORTED
Document                       Stack Overflow - Where Developers Learn, Share, & Build Careers
Priority                       1
Position                       2
Submitted                      2024-09-02 02:39:56.764
StartTime                      60
UntilTime                      60
Time                           5625
TotalPages                     16
PagesPrinted                   16
TotalBytes                     2382676
BytesPrinted                   0
```

## `print_manager_system.txt`

The `print_manager_system.txt` log reports Print Manager object and thread events. 

```
START
---------------------------------------

CPrintManagerApp::StartLogs                                            Thread ID: 781072 
CPrintManagerApp::CPrintManagerApp                                     Thread ID: 781072 
CPrintManagerApp::InitInstance                                         Thread ID: 781072 
PrintSubscriber::PrintSubscriber                                       Thread ID: 781072 
PrintConverter::PrintConverter                                         Thread ID: 781072 
CTAB1::CTAB1                                                           Thread ID: 781072 
PrintSubscriber::PrintSubscriber                                       Thread ID: 781072 
CPrintManagerDlg::CPrintManagerDlg                                     Thread ID: 781072 
CPrintManagerDlg::OnInitDialog:                                        Thread ID: 781072 
CPrintManagerDlg::OnInitDialog                                         Thread ID: 781072 
CTAB1::OnInitDialog                                                    Thread ID: 781072 
CPrintManagerDlg, UINT ThreadFunc(LPVOID pParam):                      Thread ID: 790420 
PrintSubscriber::Start                                                 Thread ID: 790420 
CPrintManagerDlg::~CPrintManagerDlg                                    Thread ID: 781072 
PrintSubscriber::~PrintSubscriber                                      Thread ID: 781072 
CTAB1::~CTAB1                                                          Thread ID: 781072 
PrintSubscriber::~PrintSubscriber                                      Thread ID: 781072 
PrintConverter::~PrintConverter                                        Thread ID: 781072 
CPrintManagerApp::~CPrintManagerApp                                    Thread ID: 781072 
CPrintManagerApp::CloseLogs                                            Thread ID: 781072 
```


# Credits

Crow, D. (2010, April 20). <i>Ask for, and receive, change notifications from a printer.</i> CodeProject. https://www.codeproject.com/Articles/74359/Ask-for-and-receive-change-notifications-from-a-pr

<blockquote>The article discusses how to retrieve print job status in Windows using FindFirstPrinterChangeNotification. The article includes a Visual C++ 6.0 project. A portion of the print monitoring and MFC user interface code was adapted into this project. The article and code were published under the Code Project Open License (CPOL) 1.02.</blockquote>

Artifex Software, Inc. (2023, November 1). <i>Ghostscript release 10.02.1.</i> Ghostscript. https://www.ghostscript.com/releases/gpdldnld.html. Alternatively, https://github.com/ArtifexSoftware/ghostpdl-downloads/releases.

<blockquote>This repository includes object code built from the GhostPDL source. I provide a complete copy of the source code at https://github.com/jtquisenberry/ghostpdl. The code was published under the AGPL 3.0 license. </blockquote>




 
