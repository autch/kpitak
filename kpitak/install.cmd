set DEST="c:\usr\local\kbmedia player\plugins\autch\kpitak"
if not exist %DEST% mkdir %DEST%
copy ..\%1\kpitak.kpi %DEST% /y
copy ..\kpitak.txt %DEST% /y
