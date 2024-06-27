/*******************************************************************************
  main.c application to test OCPP 1.6

  File Name:
    main.c



  Description:
    None
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 * Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "stdio.h"

#include "ocpp_process.h"
#include "utilities/mytimer.h"

#include "starttransaction.h"

void OCPP_CallBackFn(uint16_t frametype, uint16_t responcecode)
{
  printf("OCPP_CallBackFn ------->\n\r");
  printf("main: Frame Type: %d Frame code: %d\n\r", frametype, responcecode);

  switch (frametype)
  {
  case Authorize:
  {
    if (responcecode == AUTHORIZE_MISSING_PACKET_FIELD)
    {
      printf("Authorize packet issue\n\r");
    }

    AuthorizeResponseVal_t AuthorizeParams;
    GetAuthorizeParms(&AuthorizeParams);
    printf("Authorize parms for cache expiryDate: %s, parentIdTag: %s, status enum: %d, status: %s\n\r", AuthorizeParams.idTagInfo.expiryDate, AuthorizeParams.idTagInfo.parentIdTag, AuthorizeParams.statusENUM, AuthorizeParams.idTagInfo.status);

    // check if Authorization Cache implemented and add idTagInfo
    int cacheEnabled = chargerconfiguration_check_enabled_key("AuthorizationCacheEnabled");
    if (cacheEnabled == 1)
    {
      printf("Authorization Cache Enabled\n");
      // Add to Authroization Cache
    }
    else
    {
      printf("Authorization Cache Not Enabled\n");
    }
  }
  break;
  case BootNotification:
  {
    BootNotificationResponceVal_t BootNotificationParams;

    if (responcecode == BOOTNOTIFICATION_RESPONCE_ACCEPT_FAILED)
    {
      GetBootNotificationParms(&BootNotificationParams);
      printf("BootNotification Failed Retry interval will be: %u\n\r", BootNotificationParams.interval);
    }
  }
  break;
  case ChangeAvailability:
  {

    if (responcecode == CHANGEAVAILABILITY_RECEIVED)
    {
      uint8_t connectorid;
      char type[40];

      GetChangeAvailabilityParms(&connectorid, type);
      printf("OCPP_CallBackFn ChangeAvailability: %d %s\n\r", connectorid, type);
      // CHECK for ongoing transaction and set status to CHANGEAVAILABILITY_SCHEDULED
      if (!strcmp(type, "Inoperative") || !strcmp(type, "Operative"))
      {
        SetChangeAvailabilityStatus(CHANGEAVAILABILITY_ACCEPTED);
      }
      else
      {
        SetChangeAvailabilityStatus(CHANGEAVAILABILITY_REJECTED);
      }
    }
    else if (responcecode == CHANGEAVAILABILITY_MISSING_TYPE)
    {
      SetChangeAvailabilityStatus(CHANGEAVAILABILITY_REJECTED);
    }
    else if (responcecode == CHANGEAVAILABILITY_MISSING_CONNECTORID)
    {
      SetChangeAvailabilityStatus(CHANGEAVAILABILITY_REJECTED);
    }
    printf("Done\n\r");
  }
  break;
  case ClearCache:
  {
    // Clear Authorize Cache
    printf("Clear Cache \n\r");
    int result = 0;
    int cacheEnabled = chargerconfiguration_check_enabled_key("AuthorizationCacheEnabled");
    if (cacheEnabled == 1)
    {
      result = 0; // TODO: Attempt to clear the cache and change result
      if (result == 1)
      {
        SetClearCacheStatus(CLEAR_CACHE_ACCEPTED);
      }
      else
      {
        SetClearCacheStatus(CLEAR_CACHE_REJECTED);
      }
    }
    else
    {
      SetClearCacheStatus(CLEAR_CACHE_REJECTED);
    }
  }
  break;
  case DataTransfer:
  {
    if (responcecode == DATA_TRANSFER_INITIATOR_CSMS)
    {
      RX_DataTransfer_Request_t DataTransferParms;
      GetDataTransferRequestParmsRX(&DataTransferParms);
      printf("DataTransfer Params Request: vendorId: %s messageId: %s hasData: %d data: %s\n\r", DataTransferParms.vendorId, DataTransferParms.messageId, DataTransferParms.hasData, DataTransferParms.data);
      // TODO: Check vendorID, messageID, Data etc.
      SetDataTransferResponseStatus(DATA_TRANSFER_ACCEPTED);
    }
    else if (responcecode == DATA_TRANSFER_INITIATOR_CHARGE_STATION)
    {
      RX_DataTransfer_Response_t DataTransferParms;
      GetDataTransferRequestParmsTX(&DataTransferParms);
      printf("DataTransfer Response Params: statusENUM: %d status: %s hasData: %d data: %s\n\r", DataTransferParms.statusENUM, DataTransferParms.status, DataTransferParms.hasData, DataTransferParms.data);
    }
    else if (responcecode == DATA_TRANSFER_INITIATOR_CHARGE_STATION_RESPONSE_ERROR)
    {
      printf("DataTransfer CSMS Response packet issue\n\r");
    }
  }
  break;
  case Heartbeat:
    break;
  case MeterValues:
    break;
  case Reset:
  {
    if (responcecode == RESET_HARD)
    {
      printf("Perform a Hard Reset\n\r");
      SetResetStatus(RESET_ACCEPTED);
    }
    else if (responcecode == RESET_SOFT)
    {
      printf("Perform a Soft Reset\n\r");
      SetResetStatus(RESET_ACCEPTED);
    }
  }
  break;
  case RemoteStartTransaction:
  {
    char idTag[20];
    int connectorId;
    GetRemoteStartidTagParms(idTag);
    GetRemoteStartconnectorIdParms(&connectorId);
    printf("Remote Start Transaction idTag: %s\n\r", idTag);
    printf("Remote Start Transaction connectorId: %d\n\r", connectorId);
    SetRemoteStartTransactionStatus(REMOTE_START_STOP_ACCEPTED);
  }
  break;
  case RemoteStopTransaction:
  {
    uint8_t transactionId;
    GetRemoteStopTransactionParms(&transactionId);
    printf("Remote Stop Transaction: %d \n\r", transactionId);
    // TODO: check if transactionId is ongoing
    SetRemoteStopTransactionStatus(REMOTE_START_STOP_ACCEPTED);
  }
  break;
  case StartTransaction:
  {
    char status[50];
    GetStartTransactionParmsStatus(status);
    printf("Start TX statis============%s=======", status);
  }
  break;
  case StopTransaction:
  {
    char status[50];
    GetStopTransactionParmsStatus(status);
    printf("Stop TX statis============%s=======", status);
  }
  break;
  case UnlockConnector:
  {
    uint8_t connectorid;
    GetUnlockConnectorParms(&connectorid);
    printf("UnlockConnector connectorId: %d \n\r", connectorid);
    // TODO: check if connectorId is known and whether connector was been unlocked or not
    SetUnlockConnectorStatus(UNLOCK_CONNECTOR_UNLOCKED);
  }
  break;
  case GetDiagnostics:
  {
    if (responcecode == GETDIAGNOSTICS_LOCATION_RECEIVED)
    {
      RX_GetDiagnostics_t params;
      GetGetDiagnosticsParms(&params);
      printf("GetDiagnostics Params location: %s startTime: %s stopTime: %s retryInterval: %d retries: %d\n", params.location, params.startTime, params.stopTime, params.retryInterval, params.retries);

      // Check for Diagnostics Info
      uint8_t hasInfo = 1;
      if (hasInfo == 0)
      {
        SetGetDiagnosticsHasInfo(0);
      }
      else
      {
        SetGetDiagnosticsHasInfo(1);
        SetGetDiagnosticsFileName("GetDiagnostic_TestFileName.txt");
      }
    }
    else if (responcecode == GETDIAGNOSTICS_LOCATION_MISSING)
    {
      printf("GetDiagnostics Missing Location Parameter");
    }
  }
  break;
  case UpdateFirmware:
  {
    if (responcecode == UPDATEFIRMWARE_LOCATION_RECEIVED)
    {
      RX_UpdateFirmware_t params;
      GetUpdateFirmwareParms(&params);
      printf("UpdateFirmware Params location: %s retrieveDate: %s retryInterval: %d retries: %d\n", params.location, params.retrieveDate, params.retryInterval, params.retries);
    }
    else if (responcecode == UPDATEFIRMWARE_LOCATION_MISSING)
    {
      printf("UpdateFirmware Missing Location Parameter");
    }
  }
  break;
  case GetLocalListVersion:
  {
    uint16_t versionnumber = 1;
    int authListEnabled = chargerconfiguration_check_enabled_key("LocalAuthListEnabled");
    if (authListEnabled == 1)
    {
      versionnumber = GetLocalListVersionNumber();
      if (versionnumber == -1)
      {
        SetGetLocalListVersionListVersion(0);
      }
      else
      {

        SetGetLocalListVersionListVersion(versionnumber);
      }
    }
    else
    {
      SetGetLocalListVersionListVersion(-1);
    }
  }
  break;
  case SendLocalList:
  {
    // no need to do anything over here now, will remove this callback after confermation
    /*if (responcecode == SEND_LOCAL_LIST_MISSING_LIST_VERSION)
    {
      printf("SendLocalList request missing List Version Number\n");
    }
    else if (responcecode == SEND_LOCAL_LIST_MISSING_UPDATE_TYPE)
    {
      printf("SendLocalList request missing Missing Update Type\n");
    }
    else
    {
      RX_SendLocalList_t params;
      GetSendLocalListParams(&params);
      printf("SendLocalList Params listVersion: %d updateType: %d \n", params.listVersion, params.updateType);
      //SetSendLocalListStatus(SEND_LOCAL_LIST_NOT_SUPPORTED);
    }*/
  }
  break;
  case TriggerMessage:
  {
    RX_TriggerMessage_t params;
    GetTriggerMessageParams(&params);
    printf("TriggerMessage Params connecterId: %d requestedMessage: %d %s\n", params.connecterId, params.requestedMessageType, params.requestedMessage);

    if (params.requestedMessageType == TRIGGER_MESSAGE_TYPE_INVALID)
    {
      SetTriggerMessageStatus(TRIGGER_MESSAGE_REJECTED);
    }
    else if (params.requestedMessageType == TRIGGER_MESSAGE_TYPE_NOT_IMPLEMENTED)

    {
      SetTriggerMessageStatus(TRIGGER_MESSAGE_NOT_IMPLEMENTED);
    }
    else
    {
      SetTriggerMessageStatus(TRIGGER_MESSAGE_ACCEPTED);
    }
  }
  break;
  case CancelReservation:
  {
    if (responcecode == CANCEL_RESERVATION_MISSING_RESERVATION_ID)
    {
      printf("CancelReservation request missing Reservation Id\n");
    }
    else
    {
      uint16_t params;
      GetCancelReservationParams(&params);
      printf("CancelReservation Params reservationId: %d\n", params);
      // TODO: Check Reservations for id
      SetCancelReservationStatus(CANCEL_RESERVATION_ACCEPTED); // Set for testing
    }
  }
  break;
  case ReserveNow:
  {
    if (responcecode == RESERVE_NOW_MISSING_CONNECTOR_ID)
    {
      printf("ReserveNow request missing Connector Id\n");
    }
    else if (responcecode == RESERVE_NOW_MISSING_EXPIRY_DATE)
    {
      printf("ReserveNow request missing Expirty Date\n");
    }
    else if (responcecode == RESERVE_NOW_MISSING_ID_TAG)
    {
      printf("ReserveNow request missing Id Tag\n");
    }
    else if (responcecode == RESERVE_NOW_MISSING_RESERVATION_ID)
    {
      printf("ReserveNow request missing Reservation Id\n");
    }
    else
    {
      RX_ReserveNow_t params;
      GetReserveNowParams(&params);
      printf("ReserveNow Params connectorId: %d expiryDate: %s idTag: %s parentIdTag: %s reservationId: %d\n", params.connectorId, params.expiryDate, params.idTag, params.parentIdTag, params.reservationId);
      // TODO: Attempt to Reserve
      SetReserveNowStatus(RESERVE_NOW_ACCEPTED); // Set for testing
    }
  }
  break;
  case StatusNotification:
  {

    if (responcecode == STATUSNOTIFICATION_RESPONCE_ACCEPT_FAILED)
    {
      printf("StatusNotification Failed \n\r");
    }
  }
  break;
  case DiagnosticsStatusNotification:
  {
    if (responcecode == DIAGNOSTICSTATUSNOTIFICATION_RESPONCE_ACCEPT_FAILED)
    {
      printf("DiagnosticStatusNotification Failed \n\r");
    }
  }
  break;
  case SetChargingProfile:
  {
    RX_SetChargingProfile_t params;
    GetSetChargingProfileParams(&params);

    SetSetChargingProfileStatus(SET_CHARGING_PROFILE_ACCEPTED);
  }
  break;
  case GetCompositeSchedule:
  {
    RX_GetCompositeSchedule_t params;

    if (responcecode == RX_GET_COMPOSITE_SCHEDULE_MISSING_CONNECTOR_ID)
    {
      SetGetCompositeScheduleStatus(GET_COMPOSITE_SCHEDULE_REJECTED);
    }
    else if (responcecode == RX_GET_COMPOSITE_SCHEDULE_MISSING_DURATION)
    {
      SetGetCompositeScheduleStatus(GET_COMPOSITE_SCHEDULE_REJECTED);
    }
    else
    {
      GetGetCompositeScheduleParams(&params);
      printf("GetCompositeSchedule Params connectorId: %d duration: %d chargingRateUnit: %s \n", params.connectorId, params.duration, params.chargingRateUnit);

      SetGetCompositeScheduleStatus(SET_CHARGING_PROFILE_ACCEPTED);
    }
  }
  break;
  case ClearChargingProfile:
  {
    RX_ClearChargingProfile_t params;
    GetClearChargingProfileParams(&params);

    SetClearChargingProfileStatus(SET_CHARGING_PROFILE_ACCEPTED);
  }
  break;
  }
}

// void OCPP_CallBackFnMeterValue(SampledValue_t *sampledValue)
// {
//   switch (sampledValue->measurandENUM)
//   {
//   case METER_VALUES_ENERGY_ACTIVE_IMPORT_REGISTER:
//   {
//     double kwh = 55.64;
//     sprintf(sampledValue->value, "%0.2f", kwh);
//   }
//   break;
//   case METER_VALUES_VOLTAGE:
//   {
//     double voltage = 230;
//     sprintf(sampledValue->value, "%0.2f", voltage);
//   }
//   break;
//   }
// }

int kbhit()
{
  int byteswaiting;
  ioctl(0, FIONREAD, &byteswaiting);
  return byteswaiting > 0;
}

void main(void)
{
  char ch;

  printf("This is OCPP Stack Test\n\r");

  ocpp_init();
  timer_initialize();
  chargerconfiguration_database_init();
  locallist_database_init();
  DataTransferInit();
  MeterValueStructInit();
  SetLocalListVersionNumber(120);

  // SendBootnotificationRequest();
  // SetAuthorize_idTag("23F532C35");
  // SendAuthorizeRequest();
  //  SendDataTransferRequest();
  // SendMeterValuesRequest();

  // chargerconfiguration_database_close();
  while (1)
  {
    ch = getc(stdin);
    printf("I received %c %02x\n", ch, ch);

    if (ch == 0x0a)
    {
      printf("Received Enter\n\r");
    }
    else
    {
      switch (ch)
      {
      case 'u':
      {
        UpdateMeterValue(UPDATE_SOC, 5.0);
        UpdateMeterValue(UPDATE_POWER_ACTIVE_IMPORT, 0.2);
        UpdateMeterValue(UPDATE_CURRENT_IMPORT, 5.0);
        UpdateMeterValue(UPDATE_CURRENT_OFFERED, 1.0);
        UpdateMeterValue(UPDATE_TEMPERATURE, 27.0);
        UpdateMeterValue(UPDATE_VOLTAGE, 230.1);
        UpdateMeterValue(ENERGY_ACTIVE_IMPORT_REGISTER, 0.5);
      }
      break;
      case 'b':
        SendBootnotificationRequest();
        break;
        // case 'h':
        //   ocpp_send_HeartBeet();      //heartbeats are now automatic
        break;
      case 'a':
        SetAuthorize_idTag("23F532C35");
        SendAuthorizeRequest();
        break;
      case 'm':
        SendMeterValuesRequest();
        break;

      case 's':
      {
        UpdateStartTransactionRequestParms(1, "23F532C35", 0);
        SendStartTransactionRequest();
      }
      break;
      case 't':
      {
        UpdateStopTransactionRequestMeterStop(400);
        SendStopTransactionRequest(15.0);
      }
      break;
      case '1':
      {
        RX_ChangeConfiguration_t key_value;
        sprintf(key_value.key, "%s", "LocalAuthListEnabled");
        sprintf(key_value.value, "%s", "1");
        chargerconfiguration_update(key_value);
      }
      break;
      case '2':
      {
        char idTag[50];
        char status[100];
        char expiryDate[100];
        char parentIdTag[100];
        sprintf(idTag, "23F532C35");
        if (GetLocalListTagDataFromDB(idTag, status, expiryDate, parentIdTag) == 0)
        {
          printf("Tag: %s\n\r", idTag);
          printf("Status: %s\n\r", status);
          printf("ExpiryDate: %s\n\r", expiryDate);
          printf("parentIdTag: %s\n\r", parentIdTag);
        }
        else
        {
          printf("no data found\n\r");
        }
      }
      break;
      case '3':
      {
        {
          uint16_t timervalue;
          char key[100] = "MeterValueSampleInterval";
          char type[100];
          char value[100];
          char readonly[100];
          GetChargerConfigurationWithKey(key, type, value, readonly);
          timervalue = atoi(value);
          printf("Meter Timer Value = %d\n\r", timervalue);
        }
      }
      break;
      }
    }
  }
}