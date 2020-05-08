#include <switch.h>
#include <stdio.h>
#include <unistd.h>

#ifdef NXLINK
static int nxlink_sock = -1;
#endif

void switch_open(void)
{
#ifdef NXLINK
    socketInitializeDefault();
    nxlink_sock = nxlinkStdio();
#endif
}

void switch_close(void)
{
#ifdef NXLINK
    if (nxlink_sock != -1)
        close(nxlink_sock);
    socketExit();
#endif
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
                pcvSetClockRate(PcvModule_GPU, 921000000);
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
                clkrstSetClockRate(&session, 1785000000);
                clkrstCloseSession(&session);
                clkrstOpenSession(&session, PcvModuleId_GPU, 3);
                clkrstSetClockRate(&session, 921000000);
                clkrstCloseSession(&session);
                clkrstOpenSession(&session, PcvModuleId_EMC, 3);
                clkrstSetClockRate(&session, 1600000000);
                clkrstCloseSession(&session);
            }
        }
    }
}
