# Ghostscript Notes


# GhostPDL Command Line Examples

``` shell
gswin64c -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=out.pdf prns/Bullzip.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=prns/Bullzip.pdf prns/Bullzip.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -r300 -sDEVICE=pdfwrite -sOutputFile=prns/HP_Laserjet_1310.pdf prns/HP_Laserjet_1310.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=prns/Oki_C5500.pdf prns/Oki_C5500.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=prns/OneNote.pdf prns/OneNote.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=ps2write -sOutputFile=prns/HP_Laserjet_1310.ps prns/HP_Laserjet_1310.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=prns/HP_Laserjet_1310.pdf prns/HP_Laserjet_1310.ps

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=ps2write -sOutputFile=prns/Oki_C5500.ps prns/Oki_C5500.prn

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=prns/Oki_C5500.pdf prns/Oki_C5500.ps

gpdlwin64 -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=prns/00031.pdf prns/00031.SPL

gswin64c.exe -sDEVICE=mswinpr2 -dBATCH -dNOPAUSE -sOutputFile="%printer%\\printServer\printerNameWith Spaces" testprinter.ps

gpdlwin64 -sDEVICE=mswinpr2 -dBATCH -dNOPAUSE -sOutputFile="%printer%Bullzip PDF Printer" prns/Bullzip_PDF.pdf

gpdlwin64 -sDEVICE=mswinpr2 -dBATCH -dNOPAUSE -sOutputFile="%printer%hp LaserJet 1320 PCL 6" prns/Bullzip_PDF.pdf
```