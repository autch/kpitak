

ZIPNAME=kpitak_$(shell date +%Y%m%d%H%M).zip
TARGET=kpitak

ZIP=Release/$(TARGET).kpi tak_deco_lib.dll $(TARGET).txt TAK_Readme.html LICENSE

$(ZIPNAME): $(ZIP)
	zip -9j $@ $^
