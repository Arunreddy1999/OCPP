Gamma release of OCPP-v1.6 Linux Stack

Options: 
Core:
    A: Authorize
    B: Boot Notification
    a: Change Availability
    c: Change Configuration 
    C: Clear Cache
    D: Data Transfer
    G: GetConfiguration
    H: Heart Beat
    M: Meter Values
    s: RemoteStart Transaction
    t: RemoteStop Transaction
    R: Rest
    S: StartTransaction
    N: StatusNotification
    T: StopTransaction
    U: UnlockConnector

FirmWare:
    d: DiagnosticsStatusNotification
    F: FirmwareStatusNotification
    g: GetDiagnostics
    f: UpdateFirmWare

Local-Auth:
    L: GetLocalListVersion
    l: SendLocalList

RemotetriggerMessage:
    m: TriggerMessage

Reservation:
    r: CancelReservation
    1: ReserveNow

Smart-Charging: 
    2: ClearChargingProfile
    3: GetCompositeSchedule
    4: SetChargingProfile

Utilities:
    5: ChargerConfigurationDataBase
    6: IdTagInfo
    7: LocalStorage
    8: MyTimer



/*Core:*/
    case 'A': /*Authorize*/
    {}break;
    case 'B': /*Boot Notification*/
    {}break;
    case 'a': /*Change Availability*/
    {}break;
    case 'c': /*Change Configuration*/
    case 'C': /*Clear Cache*/
    {}break;
    case 'D': /*Data Transfer*/
    {}break;
    case 'G': /*GetConfiguration*/
    {}break;
    case 'H': /*Heart Beat*/
    {}break;
    case 'M': /*Meter Values*/
    {}break;
    case 's': /*RemoteStart Transaction*/
    {}break;
    case 't': /*RemoteStop Transaction*/
    {}break;
    case 'R': /*Rest*/
    {}break;
    case 'S': /*StartTransaction*/
    {}break;
    case 'N': /*StatusNotification*/
    {}break;
    case 'T': /*StopTransaction*/
    {}break;
    case 'U': /*UnlockConnector*/
    {}break;

/*FirmWare:*/
    case 'd': /*DiagnosticsStatusNotification*/
    {}break;
    case 'F': /*FirmwareStatusNotification*/
    {}break;
    case 'g': /*GetDiagnostics*/
    {}break;
    case 'f': /*UpdateFirmWare*/
    {}break;

/*Local-Auth:*/
    case 'L': /*GetLocalListVersion*/
    {}break;
    case 'l': /*SendLocalList*/
    {}break;

/*RemotetriggerMessage:*/
    case 'm': /*TriggerMessage*/
    {}break;

/*Reservation:*/
    case 'r': /*CancelReservation*/
    {}break;
    case '1': /*ReserveNow*/
    {}break;

/*Smart-Charging: */
    case '2': /*ClearChargingProfile*/
    {}break;
    case '3': /*GetCompositeSchedule*/
    {}break;
    case '4': /*SetChargingProfile*/
    {}break;

/*Utilities:*/
    case '5': /*ChargerConfigurationDataBase*/
    {}break;
    case '6': /*IdTagInfo*/
    {}break;
    case '7': /*LocalStorage*/
    {}break;
    case '8': /*MyTimer*/
    {}break;