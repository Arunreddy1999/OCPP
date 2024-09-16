
EXEDIR=exe
ODIR=obj

CC=gcc

INCLUDE_SRC := -I websocket -I cJSON -I ocpp16-manager \
-I websocket \
-I utilities \
-I ocpp16-messages/core/BootNotification \
-I ocpp16-messages/core/Heartbeat \
-I ocpp16-messages/core/StatusNotification \
-I ocpp16-messages/core/Authorize \
-I ocpp16-messages/core/ChangeConfiguration \
-I ocpp16-messages/core/GetConfiguration \
-I ocpp16-messages/core/ChangeAvailability \
-I ocpp16-messages/core/ClearCache \
-I ocpp16-messages/core/DataTransfer \
-I ocpp16-messages/core/MeterValues \
-I ocpp16-messages/core/RemoteStopTransaction \
-I ocpp16-messages/core/RemoteStartTransaction \
-I ocpp16-messages/core/Reset \
-I ocpp16-messages/core/StartTransaction \
-I ocpp16-messages/core/StatusNotification \
-I ocpp16-messages/core/StopTransaction \
-I ocpp16-messages/core/UnlockConnector \
-I ocpp16-messages/firmware/GetDiagnostics \
-I ocpp16-messages/firmware/UpdateFirmware \
-I ocpp16-messages/firmware/DiagnosticsStatusNotification \
-I ocpp16-messages/firmware/FirmwareStatusNotification \
-I ocpp16-messages/local-auth/GetLocalListVersion \
-I ocpp16-messages/local-auth/SendLocalList \
-I ocpp16-messages/remotetrigger/TriggerMessage \
-I ocpp16-messages/reservation/CancelReservation \
-I ocpp16-messages/reservation/ReserveNow \
-I ocpp16-messages/smart-charging/SetChargingProfile \
-I ocpp16-messages/smart-charging/ClearChargingProfile \
-I ocpp16-messages/smart-charging/GetCompositeSchedule \
-I application \


LIBS := -lpthread -lm -lsqlite3 -lssl -lcrypto -lgcov

DEPS = cJSON.h  \
libwebsocket.h \
utils.h \
ocpp_config_manager.h \
ocpp_websocket_manager.h \
ocpp_process.h \
uuidblock.h \
chargerconfigurationdatabase.h \
authorize.h \
bootnotification.h \
heartbeat.h \
changeavailability.h \
changeconfiguration.h \
datatransfer.h \
clearcahce.h \
getconfiguration.h \
metervalues.h \
mytimer.h \
remotestoptransaction.h \
remotestarttransaction.h \
reset.h \
starttransaction.h \
statusnotification.h \
stoptransaction.h \
unlockconnector.h \
diagnosticstatusnotification.h \
firmwarestatusnotification.h \
getdiagnostics.h \
updatefirmware.h \
getlocallistversion.h \
sendlocallist.h \
triggermessage.h \
cancelreservation.h \
reservenow.h \
setchargingprofile.h \
clearchargingprofile.h \
getcompositeschedule.h \
localstorage.h \
process_app.h \
logging.h

_OBJ = \
cJSON.o \
libwebsocket.o \
utils.o \
main.o \
ocpp_process.o \
ocpp_config_manager.o \
ocpp_websocket_manager.o \
uuidblock.o \
chargerconfigurationdatabase.o \
authorize.o \
bootnotification.o \
heartbeat.o \
changeavailability.o \
changeconfiguration.o \
clearcache.o \
datatransfer.o \
getconfiguration.o \
metervalues.o \
mytimer.o \
remotestoptransaction.o \
remotestarttransaction.o \
reset.o \
starttransaction.o \
statusnotification.o \
stoptransaction.o \
unlockconnector.o \
diagnosticstatusnotification.o \
firmwarestatusnotification.o \
getdiagnostics.o \
updatefirmware.o \
getlocallistversion.o \
sendlocallist.o \
triggermessage.o \
cancelreservation.o \
reservenow.o \
setchargingprofile.o \
clearchargingprofile.o \
getcompositeschedule.o \
localstorage.o \
callback_response.o \
process_core.o \
process_firmware.o \
process_localauthlist.o \
process_remotetrigger.o \
process_reserve.o \
process_smartcharging.o \
logging.o


OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Add coverage flags to CFLAGS
CFLAGS = -Wall -Wextra -Werror -O2 -fprofile-arcs -ftest-coverage -g

all: $(EXEDIR)/ocpp1.6_WD

$(EXEDIR)/ocpp1.6_WD: $(OBJ) $(APPOBJ)
	$(CC) -g -o $@ $(OBJ) $(LIBS)


$(ODIR)/%.o: application/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: websocket/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: cJSON/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-manager/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: utilities/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/Authorize/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/BootNotification/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/Heartbeat/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/ChangeAvailability/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/ChangeConfiguration/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/GetConfiguration/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/ClearCache/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/DataTransfer/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)
	
$(ODIR)/%.o: ocpp16-messages/core/MeterValues/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)
	
$(ODIR)/%.o: ocpp16-messages/core/RemoteStopTransaction/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/RemoteStartTransaction/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)	

$(ODIR)/%.o: ocpp16-messages/core/Reset/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/StartTransaction/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/StatusNotification/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/core/StopTransaction/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)	

$(ODIR)/%.o: ocpp16-messages/core/UnlockConnector/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/firmware/DiagnosticsStatusNotification/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)
	
$(ODIR)/%.o: ocpp16-messages/firmware/FirmwareStatusNotification/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/firmware/GetDiagnostics/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/firmware/UpdateFirmware/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)		

$(ODIR)/%.o: ocpp16-messages/local-auth/GetLocalListVersion/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/local-auth/SendLocalList/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/remotetrigger/TriggerMessage/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/reservation/CancelReservation/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)	

$(ODIR)/%.o: ocpp16-messages/reservation/ReserveNow/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/smart-charging/SetChargingProfile/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

$(ODIR)/%.o: ocpp16-messages/smart-charging/ClearChargingProfile/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)	

$(ODIR)/%.o: ocpp16-messages/smart-charging/GetCompositeSchedule/%.c
	$(CC) -c -o $@ $< $(INCLUDE_SRC)

all: $(EXEDIR)/ocpp1.6_WD

$(EXEDIR)/ocpp1.6_WD: $(OBJ) $(APPOBJ)
	$(CC) -g -o $@ $(OBJ) $(LIBS) $(CFLAGS)

# Your existing compilation rules
# ... -fprofile-arcs -ftest-coverage

clean:
	echo "Cleaning..."
	rm -rf $(ODIR)/*.o $(EXEDIR)/ocpp1.6_WD $(ODIR)/.gcda $(ODIR)/.gcno $(ODIR)/.html $(ODIR)/*.css

# Define the rule to generate code coverage report
coverage: all
	timeout 10s ./$(EXEDIR)/ocpp1.6_WD || true
	lcov --capture --directory $(ODIR) --output-file $(ODIR)/coverage.info
	genhtml $(ODIR)/coverage.info --output-directory $(ODIR)/coverage
	
# Define the rule for cppcheck
CPPCHECK = cppcheck
SOURCES = $(shell find . -name "*.c")
CPPCHECKFLAGS = --enable=style,performance --addon=/home/$(USER)/src/cppcheck-main/addons/

check:
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SOURCES)
