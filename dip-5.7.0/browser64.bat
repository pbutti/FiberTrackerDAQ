echo Starting DIP Browser
set PATH=%CD%\lib64;%PATH%
echo Starting DIP Browser 5.7.0
javaw -Djava.library.path=%CD%\lib64 -jar %CD%\tools\dip-browser.jar
