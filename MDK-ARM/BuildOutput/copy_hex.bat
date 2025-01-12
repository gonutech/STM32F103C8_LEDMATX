@echo Copying output file to BuildOutput....
@set OutputPath=%cd%\BuildOutput\build
@copy WirelessSensor\*.hex %OutputPath%
@copy WirelessSensor\*.map %OutputPath%
@copy WirelessSensor\*.axf %OutputPath%
@copy WirelessSensor\*.htm %OutputPath%
@echo Done - Files copied!