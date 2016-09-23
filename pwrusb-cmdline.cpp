#include <stdio.h>
#include <unistd.h>
#include "_PwrUSBCmd.cpp"

// The purpose of this file is to a quick test case of command line interface

extern int get_number_of_strips_attached()
{
    int i, MaxUnits, connected;
    int ioStates[7];
	char firmware[32];

	memset(ioStates, 0, sizeof(int)*7);					// triggger state of 3 outputs and 4 inputs
    strcpy(firmware, "");
    loadSharedLibrary();
    if ((MaxUnits=(*InitPowerUSB_Address)(&i, firmware)) > 0)		// Initialize the PowerUSB
        connected = (*CheckStatusPowerUSB_Address)();
    return MaxUnits;
}

extern int get_total_current_milliamps(int bank)
{
    int r1, current, i, MaxUnits, connected;
    int ioStates[7];
	char firmware[32];

	memset(ioStates, 0, sizeof(int)*7);					// triggger state of 3 outputs and 4 inputs
    strcpy(firmware, "");
    loadSharedLibrary();
    if ((MaxUnits=(*InitPowerUSB_Address)(&i, firmware)) > 0)		// Initialize the PowerUSB
        connected = (*CheckStatusPowerUSB_Address)();
    // TODO: sometime when I have multiple pwrusb strips attached, test `bank`
    (*SetCurrentPowerUSB_Address)(bank);
    r1 = (*ReadCurrentPowerUSB_Address)(&current);						// Present current consumption reading in milli amps
    return current;
}

extern void get_outlet_states(int bank, int *outlet1, int *outlet2, int *outlet3)
{
	int connected=0;
    int in1, in2, in3, i, MaxUnits;
    int ioStates[7];
	char firmware[32];

	memset(ioStates, 0, sizeof(int)*7);					// triggger state of 3 outputs and 4 inputs
    strcpy(firmware, "");
    loadSharedLibrary();

    if ((MaxUnits=(*InitPowerUSB_Address)(&i, firmware)) > 0)		// Initialize the PowerUSB
        connected = (*CheckStatusPowerUSB_Address)();

    // TODO: sometime when I have multiple pwrusb strips attached, test `bank`
    (*SetCurrentPowerUSB_Address)(bank);
    
//     See:
//     http://smile.amazon.com/forum/-/Tx3S42X36ZWYTGH/ref=ask_dp_dpmw_al_hza?asin=B008AZW9P6
// 
//         I am using my PwrUSB connected to a MacMini. They don't support Mac, but
//         they were nice enough to send me the C src code for the programming library.
//         I discovered a bug related ReadPortStatePowerUSB().
// 
//         The read function implementation does not match the documentation. According
//         to the code, the 3 return parameters must go in with values greater than 0
//         otherwise the ports are not actually read. And the returned status is
//         actually the on/off state of the last one read.
// HR: so, therefore, initialize in1,in2,in3 before requesting.  I still haven't figured out
//     what the last comment about 'last one read' means.

    in1 = 999;
    in2 = 999;
    in3 = 999;

    (*ReadPortStatePowerUSB_Address)(&in1, &in2, &in3);

    *outlet1 = in1;
    *outlet2 = in2;
    *outlet3 = in3;
    return;
}

extern void set_outlet_states(int bank, int outlet1, int outlet2, int outlet3)
{
	int connected=0;
    int i, MaxUnits;
    int ioStates[7];
	char firmware[32];

	memset(ioStates, 0, sizeof(int)*7);					// triggger state of 3 outputs and 4 inputs
    strcpy(firmware, "");
    loadSharedLibrary();

    if ((MaxUnits=(*InitPowerUSB_Address)(&i, firmware)) > 0)		// Initialize the PowerUSB
        connected = (*CheckStatusPowerUSB_Address)();
    // TODO: sometime when I have multiple pwrusb strips attached, test `bank`
    (*SetCurrentPowerUSB_Address)(bank);
    (*SetPortPowerUSB_Address)(outlet1, outlet2, outlet3);
    return;
}

extern void set_single_outlet_state(int bank, int outlet_number, int state)
{
    int outlet1, outlet2, outlet3, new_state;
    if (state)
        new_state = 1;
    else
        new_state = 0;
    get_outlet_states(bank, &outlet1, &outlet2, &outlet3);
    if (outlet_number == 1){outlet1 = new_state;}
    else if (outlet_number == 2){outlet2 = new_state;}
    else if (outlet_number == 3){outlet3 = new_state;}
    set_outlet_states(bank, outlet1, outlet2, outlet3);
    return;
}

void demo()
{
    int outlet1, outlet2, outlet3;
    printf("Yippee yi yo.\n");
    printf("Number of strips attached: %d\n", get_number_of_strips_attached());
    printf("Total current milliamps on bank 0: %d\n", get_total_current_milliamps(0));
    get_outlet_states(0, &outlet1, &outlet2, &outlet3);
    printf("Outlet states on bank 0:  %d  %d  %d\n", outlet1, outlet2, outlet3);
    sleep(1);
    outlet1 = 0;
    outlet2 = 1; 
    outlet3 = 0;
    printf("Setting outlets on bank 0 to:  %d  %d  %d\n", outlet1, outlet2, outlet3);
    set_outlet_states(0, outlet1, outlet2, outlet3);
    sleep(1);
    printf("Outlet states on bank 0:  %d  %d  %d\n", outlet1, outlet2, outlet3);
    sleep(1);
    outlet1 = 1;
    outlet2 = 0; 
    outlet3 = 1;
    printf("Setting outlets on bank 0 to:  %d  %d  %d\n", outlet1, outlet2, outlet3);
    set_outlet_states(0, outlet1, outlet2, outlet3);
    sleep(1);
    printf("Outlet states on bank 0:  %d  %d  %d\n", outlet1, outlet2, outlet3);
}

void status()
{
    int outlet1, outlet2, outlet3;
    int bank=0;
    get_outlet_states(bank, &outlet1, &outlet2, &outlet3);
    printf("Outlet states on bank %d:\n\t%d  %d  %d\n", bank, outlet1, outlet2, outlet3);
}

void pwrusb_info()
{
    printf("pwrusb usage examples:\n");
    printf("  pwrusb status\n");
    printf("      returns the current outlet states, in format:\n");
    printf("          Outlet states on bank BANK:\n                OUTLET1  OUTLET2  OULTET3\n");
    printf("      where OUTLET state is 0=off, 1=on\n");
    printf("  pwrusb set OUTLET1STATE OUTLET2STATE OUTLET3STATE\n");
    printf("      sets all 3 outlets to individually specified states\n");
    printf("      where OUTLET[1-3]STATE is 0=off, 1=on  (anything except 0 is on)\n");
    printf("  pwrusb setall STATE\n");
    printf("      sets all 3 outlets\n");
    printf("      where STATE is 0=off, 1=on  (anything except 0 is on)\n");
    printf("  pwrusb setone OUTLETNUM STATE\n");
    printf("      where OUTLETNUM is 1, 2, or 3\n");
    printf("      where STATE is 0=off, 1=on  (anything except 0 is on)\n");
    printf("  pwrusb current\n");
    printf("      returns the total current being drawn (milliamp)\n");
    printf("NOTES: \n");
    printf("    - current is not 0, even when all outlets off, or power strip switch is off\n");
    printf("    - outlets respond & are controllable even if overall powerstrip red power switch is off\n");
}

int main(int argc, char* argv[])
{
    int outlet1, outlet2, outlet3, outletnum;
    int bank=0;
    
    if (get_number_of_strips_attached() <= 0)
    {
        pwrusb_info();
        printf("--- No pwrusb strips were detected on USB ----\n");
    }
    else
    if (argc == 2 && strcmp(argv[1], "status") == 0)
        status();
    else 
    if (argc == 5 && strcmp(argv[1], "set") == 0)
    {
        if (strcmp(argv[2], "0") == 0)
            outlet1 = 0;
        else
            outlet1 = 1;
        if (strcmp(argv[3], "0") == 0)
            outlet2 = 0;
        else
            outlet2 = 1;
        if (strcmp(argv[4], "0") == 0)
            outlet3 = 0;
        else
            outlet3 = 1;
        set_outlet_states(bank, outlet1, outlet2, outlet3);
    }
    else
    if (argc == 3 && strcmp(argv[1], "setall") == 0)
    {
        if (strcmp(argv[2], "0") == 0)
            outlet1 = 0;
        else
            outlet1 = 1;
        outlet2 = outlet1;
        outlet3 = outlet1;
        set_outlet_states(bank, outlet1, outlet2, outlet3);
    }
    else    
    if (argc == 4 && strcmp(argv[1], "setone") == 0)
    {
        if ((strcmp(argv[2], "1") != 0) && (strcmp(argv[2], "2") != 0) && (strcmp(argv[2], "3") != 0))
        {
            printf("ERROR: pwrusb setone: unrecognized outlet number, must be 1, 2, or 3\n");
            return -1;
        }
        else
        {
            outletnum = atoi(argv[2]);
            if (strcmp(argv[3], "0") == 0)
                outlet1 = 0;
            else
                outlet1 = 1;
            set_single_outlet_state(bank, outletnum, outlet1);
        }
    }
    else
    if (argc == 2 && strcmp(argv[1], "current") == 0)
    {
        printf("Total current (milliamp) on bank %d: %d\n", bank, get_total_current_milliamps(bank));
    }
    else
        pwrusb_info();
//     demo();    
    return 0;
}







