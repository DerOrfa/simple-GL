# Microsoft Developer Studio Project File - Name="libsimpleGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libsimpleGL - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "libsimpleGL.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "libsimpleGL.mak" CFG="libsimpleGL - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "libsimpleGL - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "libsimpleGL - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libsimpleGL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../emath/" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"simpleGL.lib"

!ELSEIF  "$(CFG)" == "libsimpleGL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libsimpleGL___Win32_Debug"
# PROP BASE Intermediate_Dir "libsimpleGL___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libsimpleGL___Win32_Debug"
# PROP Intermediate_Dir "libsimpleGL___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /w /W0 /Gm /GX /ZI /Od /I "../emath/" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "HAS_SSTREAM" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"simpleGL-Dbg.lib"

!ENDIF 

# Begin Target

# Name "libsimpleGL - Win32 Release"
# Name "libsimpleGL - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\text\backend_glf\glf.c
# End Source File
# Begin Source File

SOURCE=.\util\readtex.c
# End Source File
# Begin Source File

SOURCE=.\text\sgl3dtext.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sglcamera.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sglclipplane.cpp
# End Source File
# Begin Source File

SOURCE=.\text\sglconsole.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sgldisplist.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglflobj.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglgeosphere.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sglgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sglhelper.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglicosahedron.cpp
# End Source File
# Begin Source File

SOURCE=.\util\sglimagefile.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sgllensflare.cpp
# End Source File
# Begin Source File

SOURCE=.\helper\sgllight.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglloft.cpp
# End Source File
# Begin Source File

SOURCE=.\util\sglmaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\sglmetaobj.cpp
# End Source File
# Begin Source File

SOURCE=.\sglmisc.c
# End Source File
# Begin Source File

SOURCE=.\primitives\sglneck.cpp
# End Source File
# Begin Source File

SOURCE=.\sglobj.cpp
# End Source File
# Begin Source File

SOURCE=.\sglobjbase.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglplanet.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglpolygonobj.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglquader.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglquadricobj.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglrekgeb.cpp
# End Source File
# Begin Source File

SOURCE=.\sglspace.cpp
# End Source File
# Begin Source File

SOURCE=.\text\sgltext.cpp
# End Source File
# Begin Source File

SOURCE=.\text\sgltextbackend.cpp
# End Source File
# Begin Source File

SOURCE=.\text\backend_glf\sgltextbackend_glf.cpp
# End Source File
# Begin Source File

SOURCE=.\util\sgltextur.cpp
# End Source File
# Begin Source File

SOURCE=.\sglvektor.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives\sglvieleck.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\text\backend_glf\glf.h
# End Source File
# Begin Source File

SOURCE=.\util\readtex.h
# End Source File
# Begin Source File

SOURCE=.\text\sgl3dtext.h
# End Source File
# Begin Source File

SOURCE=.\helper\sglcamera.h
# End Source File
# Begin Source File

SOURCE=.\helper\sglclipplane.h
# End Source File
# Begin Source File

SOURCE=.\text\sglconsole.h
# End Source File
# Begin Source File

SOURCE=.\helper\sgldisplist.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglflobj.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglgeosphere.h
# End Source File
# Begin Source File

SOURCE=.\helper\sglgrid.h
# End Source File
# Begin Source File

SOURCE=.\helper\sglhelper.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglicosahedron.h
# End Source File
# Begin Source File

SOURCE=.\util\sglimagefile.h
# End Source File
# Begin Source File

SOURCE=.\helper\sgllensflare.h
# End Source File
# Begin Source File

SOURCE=.\helper\sgllight.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglloft.h
# End Source File
# Begin Source File

SOURCE=.\util\sglmaterial.h
# End Source File
# Begin Source File

SOURCE=.\sglmetaobj.h
# End Source File
# Begin Source File

SOURCE=.\sglmisc.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglneck.h
# End Source File
# Begin Source File

SOURCE=.\sglobj.h
# End Source File
# Begin Source File

SOURCE=.\sglobjbase.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglplanet.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglpolygonobj.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglquader.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglquadricobj.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglrekgeb.h
# End Source File
# Begin Source File

SOURCE=.\sglspace.h
# End Source File
# Begin Source File

SOURCE=.\GLf\sgltext.h
# End Source File
# Begin Source File

SOURCE=.\text\sgltext.h
# End Source File
# Begin Source File

SOURCE=.\text\sgltextbackend.h
# End Source File
# Begin Source File

SOURCE=.\text\backend_glf\sgltextbackend_glf.h
# End Source File
# Begin Source File

SOURCE=.\util\sgltextur.h
# End Source File
# Begin Source File

SOURCE=.\sglvektor.h
# End Source File
# Begin Source File

SOURCE=.\primitives\sglvieleck.h
# End Source File
# End Group
# End Target
# End Project
