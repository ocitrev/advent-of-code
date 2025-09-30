cmake -G "Ninja Multi-Config" -S "%~dp0." -B "%~dp0build\cpp" %*
@exit /b %errorlevel%
