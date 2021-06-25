//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_PACKETS_H
#define MEDIUS_MUIS_SERVER_PACKETS_H

#define SESSIONKEY_MAXLEN 17
#define ACCESSKEY_MAXLEN 17
#define IP_MAXLEN 17
#define RSAKEY_SIZE 64
#define MESSAGEID_MAXLEN 21
#define NET_MAX_IP_LENGTH 16
#define ACCOUNTNAME_MAXLEN 32
#define PASSWORD_MAXLEN 32

class Packets {
public:
    typedef enum {
        Modem = 0,
        Ethernet = 1,
        Wireless = 2,
        ExtraMediusConnectionType = 0xffffff
    } MediusConnectionType;

    typedef enum int32_t {
        MediusBeginSessionFailed = -1000,
        MediusAccountAlreadyExists = -999,
        MediusAccountNotFound = -998,
        MediusAccountLoggedIn = -997,
        MediusEndSessionFailed = -996,
        MediusLoginFailed = -995,
        MediusRegistrationFailed = -994,
        MediusIncorrectLoginStep = -993,
        MediusAlreadyLeaderOfClan = -992,
        MediusWMError = -991,
        MediusNotClanLeader = -990,
        MediusPlayerNotPrivileged = -989,
        MediusDBError = -988,
        MediusDMEError = -987,
        MediusExceedsMaxWorlds = -986,
        MediusRequestDenied = -985,
        MediusSetGameListFilterFailed = -984,
        MediusClearGameListFilterFailed = -983,
        MediusGetGameListFilterFailed = -982,
        MediusNumFiltersAtMax = -981,
        MediusFilterNotFound = -980,
        MediusInvalidRequestMsg = -979,
        MediusInvalidPassword = -978,
        MediusGameNotFound = -977,
        MediusChannelNotFound = -976,
        MediusGameNameExists = -975,
        MediusChannelNameExists = -974,
        MediusGameNameNotFound = -973,
        MediusPlayerBanned = -972,
        MediusClanNotFound = -971,
        MediusClanNameInUse = -970,
        MediusSessionKeyInvalid = -969,
        MediusTextStringInvalid = -968,
        MediusFilterFailed = -967,
        MediusFail = -966,
        MediusFileInternalAccessError = -965,
        MediusFileNoPermissions = -964,
        MediusFileDoesNotExist = -963,
        MediusFileAlreadyExists = -962,
        MediusFileInvalidFilename = -961,
        MediusFileQuotaExceeded = -960,
        MediusCacheFailure = -959,
        MediusDataAlreadyExists = -958,
        MediusDataDoesNotExist = -957,
        MediusMaxExceeded = -956,
        MediusKeyError = -955,
        MediusIncompatibleAppID = -954,
        MediusAccountBanned = -953,
        MediusMachineBanned = -952,
        MediusLeaderCannotLeaveClan = -951,
        MediusFeatureNotEnabled = -950,
        MediusDNASSignatureLoggedIn = -949,
        MediusWorldIsFull = -948,
        MediusNotClanMember = -947,
        MediusServerBusy = -946,
        MediusNumGameWorldsPerLobbyWorldExceeded = -945,
        MediusAccountNotUCCompliant = -944,
        MediusPasswordNotUCCompliant = -943,
        MediusGatewayError = -942,
        MediusTransactionCanceled = -941,
        MediusSessionFail = -940,
        MediusTokenAlreadyTaken = -939,
        MediusTokenDoesNotExist = -938,
        MediusSubscriptionAborted = -937,
        MediusSubscriptionInvalid = -936,
        MediusNotAMember = -935,
        MediusSuccess = 0,
        MediusNoResult = 1,
        MediusRequestAccepted = 2,
        MediusWorldCreatedSizeReduced = 3,
        MediusPass = 4,
        ExtraMediusCallbackStatus = 0xffffff
    } MediusCallbackStatus;

    typedef enum {
        MediusCharacterEncoding_NoUpdate,
        MediusCharacterEncoding_ISO8859_1,
        MediusCharacterEncoding_UTF8,
        ExtraMediusCharacterEncodingType = 0xffffff
    } MediusCharacterEncodingType;

    typedef enum {
        MediusLanguage_NoUpdate,
        MediusLanguage_USEnglish,
        MediusLanguage_UKEnglish,
        MediusLanguage_Japanese,
        MediusLanguage_Korean,
        MediusLanguage_Italian,
        MediusLanguage_Spanish,
        MediusLanguage_German,
        MediusLanguage_French,
        MediusLanguage_Dutch,
        MediusLanguage_Portuguese,
        MediusLanguage_Chinese,
        MediusLanguage_Taiwanese,
        MediusLanguage_Finnish,
        MediusLanguage_Norwegian,
        ExtraMediusLanguageType = 0xffffff
    } MediusLanguageType;

    typedef enum {
        MediusChildAccount,
        MediusMasterAccount,
        ExtraMediusAccountType = 0xffffff
    } MediusAccountType;

    typedef enum {
        NetAddressNone = 0,
        NetAddressTypeExternal = 1,
        NetAddressTypeInternal = 2,
        NetAddressTypeNATService = 3,
        NetAddressTypeBinaryExternal = 4,
        NetAddressTypeBinaryInternal = 5,
        NetAddressTypeBinaryExternalVport = 6,
        NetAddressTypeBinaryInternalVport = 7,
        NetAddressTypeBinaryNATServices = 8,
        ExtraNetAddressType = 0xFFFFFFFF
    } NetAddressType;

    typedef struct {
        NetAddressType AddressType;
        char Address[NET_MAX_IP_LENGTH];
        uint16_t Port;
    } NetAddress;

    typedef struct {
        NetAddress aAddressList[2];
    } NetAddressList;

    typedef enum {
        NetConnectionNone = 0,
        NetConnectionTypeClientServerTCP = 1,
        NetConnectionTypePeerToPeerUDP = 2,
        NetConnectionTypeClientServerTCPAuxUDP = 3,
        NetConnectionTypeClientListenerTCP = 4,
        ExtraNetConnectionType = 0xffffff
    } NetConnectionType;

    typedef char MessageID[MESSAGEID_MAXLEN];
    typedef char SessionKey[SESSIONKEY_MAXLEN];
    typedef char AccessKey[ACCESSKEY_MAXLEN];
    typedef char IP[IP_MAXLEN];
    typedef uint8_t RSAKey[RSAKEY_SIZE];

    typedef struct {
        NetConnectionType Type;
        NetAddressList AddressList;
        int WorldID;
        RSAKey ServerKey;
        SessionKey SKey;
        AccessKey AKey;
    } NetConnectionInfo;

    typedef struct {
        uint8_t serverVersion;
        uint8_t unk01;
        uint8_t encrypt;
        uint8_t unk02;
    } RT_MSG_SERVER_HELLO;

    typedef struct {
        uint32_t TargetWorldId;
        int32_t AppId;
        RSAKey RKey;
    } RT_MSG_CLIENT_CONNECT_TCP;

    typedef struct {
        uint32_t TargetWorldId;
        int32_t AppId;
        RSAKey RKey;
        SessionKey SKey;
        AccessKey AKey;
    } RT_MSG_CLIENT_CONNECT_TCP_EXTENDED;

    typedef struct {
        uint16_t WorldID;
        int32_t ClientID;
        uint16_t CurClients;
        IP ClientIP;
    } RT_MSG_SERVER_CONNECT_ACCEPT_TCP;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
    } MediusVersionServerRequest;

    typedef struct {
        MessageID MsgID;
        MediusConnectionType ConnectionClass;
    } MediusSessionBeginRequest;

    typedef struct {
        MediusSessionBeginRequest SessionBeginRequest;
        int ClientMajor;
        int ClientMinor;
        int ClientBuild;
    } MediusExtendedSessionBeginRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        SessionKey SKey;
    } MediusSessionBeginResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        MediusCharacterEncodingType CharacterEncoding;
        MediusLanguageType Language;
    } MediusSetLocalizationParamsRequest;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        char AccountName[ACCOUNTNAME_MAXLEN];
        char Password[PASSWORD_MAXLEN];
    } MediusAccountLoginRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int AccountID;
        MediusAccountType AccountType;
        int MediusWorldID;
        NetConnectionInfo ConnectInfo;
    } MediusAccountLoginResponse;
};


#endif //MEDIUS_MUIS_SERVER_PACKETS_H
