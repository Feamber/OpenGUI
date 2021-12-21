set OGUIPath=J:\OpenGUI
set UEOGUIPath=D:\7daysHD\Project\Plugins\UEOpenGUI\Source\ThirdParty\OGUICore

move %UEOGUIPath%\Public\OGUICore.h %UEOGUIPath%\OGUICore.h
move %UEOGUIPath%\Private\OGUICore.cpp %UEOGUIPath%\OGUICore.cpp

rd /q/s %UEOGUIPath%\Public
rd /q/s %UEOGUIPath%\Private

xcopy /y /r /E /H /C /I /f %OGUIPath%\include %UEOGUIPath%\Public
xcopy /y /r /E /H /C /I /f %OGUIPath%\source %UEOGUIPath%\Private
rd /q/s %UEOGUIPath%\Public\External\harfbuzz
rd /q/s %UEOGUIPath%\Public\External\icu
rd /q/s %UEOGUIPath%\Public\External\freetype
rd /q/s %UEOGUIPath%\Public\External\ft2build.h

move %UEOGUIPath%\OGUICore.h %UEOGUIPath%\Public\OGUICore.h
move %UEOGUIPath%\OGUICore.cpp %UEOGUIPath%\Private\OGUICore.cpp