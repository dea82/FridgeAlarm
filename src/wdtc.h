typedef enum
{
    WDTC_OFF_E, WDTC_16MS_E, WDTC_8S_E,
} tWdtc_Timeout_E;

void Wdtc_SetWatchdog(const tWdtc_Timeout_E timeout_E);
