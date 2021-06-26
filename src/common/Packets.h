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
#define ACCOUNTSTATS_MAXLEN 256
#define WORLDNAME_MAXLEN 64
#define LOBBYNAME_MAXLEN WORLDNAME_MAXLEN
#define CLANNAME_MAXLEN 32
#define CLANSTATS_MAXLEN 256
#define LADDERSTATSWIDE_MAXLEN 100
#define CLANINVITERESPONSEMSG_MAXLEN 200
#define CLANMSG_MAXLEN 200
#define WORLDPASSWORD_MAXLEN 32
#define LOBBYPASSWORD_MAXLEN WORLDPASSWORD_MAXLEN
#define CHATMESSAGE_MAXLEN 64
#define GAMENAME_MAXLEN WORLDNAME_MAXLEN
#define WORLDSTATS_MAXLEN 256
#define GAMESTATS_MAXLEN WORLDSTATS_MAXLEN

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

    typedef enum {
        MediusTimeZone_IDLW = -1200,
        MediusTimeZone_HST = -1000,
        MediusTimeZone_AKST = -900,
        MediusTimeZone_AKDT = -800,
        MediusTimeZone_PST = -801,
        MediusTimeZone_PDT = -700,
        MediusTimeZone_MST = -701,
        MediusTimeZone_MDT = -600,
        MediusTimeZone_CST = -601,
        MediusTimeZone_CDT = -500,
        MediusTimeZone_EST = -501,
        MediusTimeZone_EDT = -400,
        MediusTimeZone_AST = -401,
        MediusTimeZone_NST = -350,
        MediusTimeZone_ADT = -300,
        MediusTimeZone_NDT = -250,
        MediusTimeZone_WAT = -100,
        MediusTimeZone_GMT = 0,
        MediusTimeZone_UTC = 1,
        MediusTimeZone_WET = 2,
        MediusTimeZone_BST = 100,
        MediusTimeZone_IRISHST = 101,
        MediusTimeZone_WEST = 102,
        MediusTimeZone_CET = 103,
        MediusTimeZone_CEST = 200,
        MediusTimeZone_SWEDISHST = 201,
        MediusTimeZone_FST = 202,
        MediusTimeZone_CAT = 203,
        MediusTimeZone_SAST = 204,
        MediusTimeZone_EET = 205,
        MediusTimeZone_ISRAELST = 206,
        MediusTimeZone_EEST = 300,
        MediusTimeZone_BT = 301,
        MediusTimeZone_MSK = 302,
        MediusTimeZone_IRANST = 350,
        MediusTimeZone_MSD = 400,
        MediusTimeZone_INDIANST = 550,
        MediusTimeZone_JT = 750,
        MediusTimeZone_HKT = 800,
        MediusTimeZone_CCT = 801,
        MediusTimeZone_AWST = 802,
        MediusTimeZone_MT = 850,
        MediusTimeZone_KST = 900,
        MediusTimeZone_JST = 901,
        MediusTimeZone_ACST = 950,
        MediusTimeZone_AEST = 1000,
        MediusTimeZone_GST = 1001,
        MediusTimeZone_ACDT = 1050,
        MediusTimeZone_AEDT = 1100,
        MediusTimeZone_SST = 1101,
        MediusTimeZone_NZST = 1200,
        MediusTimeZone_IDLE = 1201,
        MediusTimeZone_NZDT = 1300,
        MediusTimeZone_EXTRA = 0xffffff
    } MediusTimeZone;

    typedef struct {
        NetAddressType AddressType;
        char Address[NET_MAX_IP_LENGTH];
        uint16_t Port;
    } NetAddress;

    typedef struct {
        NetAddress aAddressList[2];
    } NetAddressList;

    typedef enum {
        KeepAlive,
        JoinedChatWorld,
        LeftGameWorld,
        ExtraMediusUserAction = 0xffffff
    } MediusUserAction;

    typedef enum {
        NetConnectionNone = 0,
        NetConnectionTypeClientServerTCP = 1,
        NetConnectionTypePeerToPeerUDP = 2,
        NetConnectionTypeClientServerTCPAuxUDP = 3,
        NetConnectionTypeClientListenerTCP = 4,
        ExtraNetConnectionType = 0xffffff
    } NetConnectionType;

    typedef enum {
        LESS_THAN,
        LESS_THAN_OR_EQUAL_TO,
        EQUAL_TO,
        GREATER_THAN_OR_EQUAL_TO,
        GREATER_THAN,
        NOT_EQUALS,
        ExtraMediusComparisonOperator = 0xffffff
    } MediusComparisonOperator;

    typedef enum {
        MEDIUS_FILTER_PLAYER_COUNT = 1,
        MEDIUS_FILTER_MIN_PLAYERS = 2,
        MEDIUS_FILTER_MAX_PLAYERS = 3,
        MEDIUS_FILTER_GAME_LEVEL = 4,
        MEDIUS_FILTER_PLAYER_SKILL_LEVEL = 5,
        MEDIUS_FILTER_RULES_SET = 6,
        MEDIUS_FILTER_GENERIC_FIELD_1 = 7,
        MEDIUS_FILTER_GENERIC_FIELD_2 = 8,
        MEDIUS_FILTER_GENERIC_FIELD_3 = 9,
        MEDIUS_FILTER_LOBBY_WORLDID = 10,
        MEDIUS_FILTER_GENERIC_FIELD_4 = 11,
        MEDIUS_FILTER_GENERIC_FIELD_5 = 12,
        MEDIUS_FILTER_GENERIC_FIELD_6 = 13,
        MEDIUS_FILTER_GENERIC_FIELD_7 = 14,
        MEDIUS_FILTER_GENERIC_FIELD_8 = 15,
        ExtraMediusGameListFilterField = 0xffffff
    } MediusGameListFilterField;

    typedef enum {
        MediusPlayerDisconnected = 0,
        MediusPlayerInAuthWorld,
        MediusPlayerInChatWorld,
        MediusPlayerInGameWorld,
        MediusPlayerInOtherUniverse,
        LastMediusPLayerStatus,
        ExtraMediusPlayerStatus = 0xffffff
    } MediusPlayerStatus;

    typedef enum {
        MediusWorldGenericFieldLevel0 = 0,
        MediusWorldGenericFieldLevel1 = (1<<0),
        MediusWorldGenericFieldLevel2 = (1<<1),
        MediusWorldGenericFieldLevel3 = (1<<2),
        MediusWorldGenericFieldLevel4 = (1<<3),
        MediusWorldGenericFieldLevel12 = (1<<4),
        MediusWorldGenericFieldLevel123 = (1<<5),
        MediusWorldGenericFieldLevel1234 = (1<<6),
        MediusWorldGenericFieldLevel23 = (1<<7),
        MediusWorldGenericFieldLevel234 = (1<<8),
        MediusWorldGenericFieldLevel34 = (1<<9),
        ExtraMediusWorldGenericFieldLevelType = 0xffffff
    } MediusWorldGenericFieldLevelType;

    typedef enum {
        WORLD_SECURITY_NONE = 0,
        WORLD_SECURITY_PLAYER_PASSWORD = (1 << 0),
        WORLD_SECURITY_CLOSED = (1 << 1),
        WORLD_SECURITY_SPECTATOR_PASSWORD = (1 << 2),
        WORLD_SECURITY_EXTRA = 0xFFFFFF
    } MediusWorldSecurityLevelType;

    typedef enum {
        AddSingle = 0,
        AddSymmetric = 1,
        ExtraMediusAddType = 0xffffff
    } MediusBuddyAddType;

    typedef enum {
        ClanActive,
        ClanDisbanded = -1,
        ExtraMediusClanStatus = 0xffffff
    } MediusClanStatus;

    typedef enum {
        MediusLadderTypePlayer = 0,
        MediusLadderTypeClan = 1,
        ExtraMediusLadderType = 0xffffff
    } MediusLadderType;

    typedef enum {
        ClanInvitationUndecided,
        ClanInvitationAccept,
        ClanInvitationDecline,
        ClanInvitationRevoked,
        ExtraMediusClanInvitationsResponseStatus = 0xffffff
    } MediusClanInvitationsResponseStatus;

    typedef enum {
        MEDIUS_ASCENDING,
        MEDIUS_DESCENDING,
        ExtraMediusSortOrder = 0xffffff
    } MediusSortOrder;

    typedef enum {
        MediusTextFilterPassFail = 0,
        MediusTextFilterReplace = 1,
        ExtraMediusTextFilter = 0xffffff
    } MediusTextFilterType;

    typedef enum {
        MediusGameHostClientServer = 0,
        MediusGameHostIntegratedServer = 1,
        MediusGameHostPeerToPeer = 2,
        MediusGameHostLANPlay = 3,
        MediusGameHostClientServerAuxUDP = 4,
        ExtraMediusGameHost = 0xffffff
    } MediusGameHostType;

    typedef enum {
        WorldInactive,
        WorldStaging,
        WorldActive,
        WorldClosed,
        WorldPendingCreation,
        WorldPendingConnectToGame,
        ExtraMediusWorldStatus = 0xffffff
    } MediusWorldStatus;

    typedef char MessageID[MESSAGEID_MAXLEN];
    typedef char SessionKey[SESSIONKEY_MAXLEN];
    typedef char AccessKey[ACCESSKEY_MAXLEN];
    typedef char AccountStats[ACCOUNTSTATS_MAXLEN];
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

    typedef struct {
        MessageID MsgID;
    } MediusGetServerTimeRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int GMT_time;
        MediusTimeZone Local_server_timezone;
    } MediusGetServerTimeResponse;

    typedef struct {
        SessionKey SKey;
        MediusUserAction UserAction;
    } MediusUpdateUserState;

    typedef struct {
        MessageID MsgID;
        MediusGameListFilterField FilterField;
        int Mask;
        MediusComparisonOperator ComparisonOperator;
        int BaselineValue;
    } MediusSetGameListFilterRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        unsigned int FilterID;
    } MediusSetGameListFilterResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        int AccountID;
    } MediusPlayerInfoRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        char AccountName[ACCOUNTNAME_MAXLEN];
        int ApplicationID;
        MediusPlayerStatus PlayerStatus;
        MediusConnectionType ConnectionType;
        AccountStats Stats;
    } MediusPlayerInfoResponse;

    typedef struct {
        MessageID MsgID;
        unsigned short PageID;
        unsigned short PageSize;
    } MediusChannelList_ExtraInfoRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int MediusWorldID;
        unsigned short PlayerCount;
        unsigned short MaxPlayers;
        unsigned short GameWorldCount;
        MediusWorldSecurityLevelType SecurityLevel;
        unsigned int GenericField1;
        unsigned int GenericField2;
        unsigned int GenericField3;
        unsigned int GenericField4;
        MediusWorldGenericFieldLevelType GenericFieldLevel;
        char LobbyName[LOBBYNAME_MAXLEN];
        char EndOfList;
    } MediusChannelList_ExtraInfoResponse;

    typedef struct {
        MessageID MsgID;
    } MediusGetBuddyInvitationsRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int AccountID;
        char AccountName[ACCOUNTNAME_MAXLEN];
        MediusBuddyAddType addType;
        char EndOfList;
    } MediusGetBuddyInvitationsResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        AccountStats Stats;
    } MediusAccountUpdateStatsRequest;

    typedef struct {
        MediusPlayerStatus ConnectStatus;
        int MediusLobbyWorldID;
        int MediusGameWorldID;
        char LobbyName[WORLDNAME_MAXLEN];
        char GameName[WORLDNAME_MAXLEN];
    } MediusPlayerOnlineState;

    typedef struct {
        char MessageID[MESSAGEID_MAXLEN];
        MediusCallbackStatus StatusCode;
        int AccountID;
        char AccountName[ACCOUNTNAME_MAXLEN];
        MediusPlayerOnlineState OnlineState;
        char EndOfList;
    } MediusGetBuddyList_ExtraInfoResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        int Start;
        int PageSize;
    } MediusGetMyClansRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int ClanID;
        int ApplicationID;
        char ClanName[CLANNAME_MAXLEN];
        int LeaderAccountID;
        char LeaderAccountName[ACCOUNTNAME_MAXLEN];
        char Stats[CLANSTATS_MAXLEN];
        MediusClanStatus Status;
        char EndOfList;
    } MediusGetMyClansResponse;

    typedef struct {
        MessageID MsgID;
        MediusLadderType LadderType;
        int Stats[LADDERSTATSWIDE_MAXLEN];
    } MediusUpdateLadderStatsWideRequest;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
    } MediusGetIgnoreListRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int IgnoreAccountID;
        char IgnoreAccountName[ACCOUNTNAME_MAXLEN];
        MediusPlayerStatus PlayerStatus;
        char EndOfList;
    } MediusGetIgnoreListResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        int Start;
        int PageSize;
    } MediusCheckMyClanInvitationsRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int ClanInvitationID;
        int ClanID;
        MediusClanInvitationsResponseStatus ResponseStatus;
        char Message[CLANMSG_MAXLEN];
        int LeaderAccountID;
        char LeaderAccountName[ACCOUNTNAME_MAXLEN];
        char EndOfList;
    } MediusCheckMyClanInvitationsResponse;

    typedef struct {
        MessageID MsgID;
        int ClanID;
        int LadderStatIndex;
        MediusSortOrder SortOrder;
    } MediusGetClanMemberList_ExtraInfoRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int AccountID;
        char AccountName[ACCOUNTNAME_MAXLEN];
        char Stats[ACCOUNTSTATS_MAXLEN];
        MediusPlayerOnlineState OnlineState;
        int LadderStat;
        unsigned int LadderPosition;
        unsigned int TotalRankings;
        char EndOfList;
    } MediusGetClanMemberList_ExtraInfoResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        char AccountName[ACCOUNTNAME_MAXLEN];
    } MediusAccountGetIDRequest;

    typedef struct {
        MessageID MsgID;
        int AccountID;
        MediusCallbackStatus StatusCode;
    } MediusAccountGetIDResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        int IgnoreAccountID;
    } MediusAddToIgnoreListRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
    } MediusAddToIgnoreListResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        int MediusWorldID;
        char LobbyChannelPassword[LOBBYPASSWORD_MAXLEN];
    } MediusJoinChannelRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        NetConnectionInfo ConnectInfo;
    } MediusJoinChannelResponse;

    typedef struct {
        MessageID MsgID;
        SessionKey SKey;
        MediusTextFilterType TextFilterType;
        char Text[CHATMESSAGE_MAXLEN];
    } MediusTextFilterRequest;

    typedef struct {
        MessageID MsgID;
        char Text[CHATMESSAGE_MAXLEN];
        MediusCallbackStatus StatusCode;
    } MediusTextFilterResponse;

    typedef struct {
        MessageID MsgID;
        unsigned short PageID;
        unsigned short PageSize;
    } MediusGameList_ExtraInfoRequest;

    typedef struct {
        MessageID MsgID;
        MediusCallbackStatus StatusCode;
        int MediusWorldID;
        unsigned short PlayerCount;
        unsigned short MinPlayers;
        unsigned short MaxPlayers;
        int GameLevel;
        int PlayerSkillLevel;
        int RulesSet;
        int GenericField1;
        int GenericField2;
        int GenericField3;
        int GenericField4;
        int GenericField5;
        int GenericField6;
        int GenericField7;
        int GenericField8;
        MediusWorldSecurityLevelType SecurityLevel;
        MediusWorldStatus WorldStatus;
        MediusGameHostType GameHostType;
        char GameName[GAMENAME_MAXLEN];
        char GameStats[GAMESTATS_MAXLEN];
        char EndOfList;
    } MediusGameList_ExtraInfoResponse;
};


#endif //MEDIUS_MUIS_SERVER_PACKETS_H
