#include <switch.h>
#include <stdio.h>
#include <unistd.h>

static int nxlink_sock = -1;

void switch_open(void)
{
    socketInitializeDefault();
    nxlink_sock = nxlinkStdio();
}

void switch_close(void)
{
    if (nxlink_sock != -1)
        close(nxlink_sock);
    socketExit();
}

void switch_enable_oc(int enable)
{
    if(enable > 0)
    {
        if (hosversionBefore(8, 0, 0))
        {
            if (R_SUCCEEDED(pcvInitialize()))
            {
                printf("SWITCH: overclock enabled (version < 8.0.0)\n");
                pcvSetClockRate(PcvModule_CpuBus, 1785000000);
                pcvSetClockRate(PcvModule_GPU, 768000000);
                pcvSetClockRate(PcvModule_EMC, 1600000000);
            }
        }
        else
        {
            if (R_SUCCEEDED(clkrstInitialize()))
            {
                printf("SWITCH: overclock enabled (version >= 8.0.0)\n");
                ClkrstSession session;
                clkrstOpenSession(&session, PcvModuleId_CpuBus, 3);
                if(!R_SUCCEEDED(clkrstSetClockRate(&session, 1785000000)))
                    printf("SWITCH: could not change cpu speed\n");
                clkrstCloseSession(&session);
                clkrstOpenSession(&session, PcvModuleId_GPU, 3);
                if(!R_SUCCEEDED(clkrstSetClockRate(&session, 768000000)))
                    printf("SWITCH: could not change cpu speed\n");
                clkrstCloseSession(&session);
                clkrstOpenSession(&session, PcvModuleId_EMC, 3);
                if(!R_SUCCEEDED(clkrstSetClockRate(&session, 1600000000)))
                    printf("SWITCH: could not change cpu speed\n");
                clkrstCloseSession(&session);
            }
        }
    }
}
