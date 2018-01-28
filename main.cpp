#include "MemoryManager.h"
#include <iostream>

MemoryManager* Mem;

#define m_dwLocalPlayer 0xA9D0DC
#define m_iTeamNum 0xF0
#define m_iCrossHairID 0xB2A4
#define m_dwEntityList 0x4A79EC4
#define m_bDromant 0xE9
#define m_dwForceAttack 0x2EBC23C


bool TriggerToggled = false;
 
void Trigger()
{
	while (true)
	{
        //Check for toggle on/off
        if (GetAsyncKeyState(VK_F1)) //You can customize the toggle key
        {
            TriggerToggled = !TriggerToggled;
            if (TriggerToggled) std::cout<<"Awesome triggerbot now ON"<<std::endl;
            else std::cout<<"Awesome triggerbot now OFF"<<std::endl;
            Sleep(200);
        }
		//If the triggerbot is not toggled, skip the iteration
		if (!TriggerToggled) continue;
		
        //Retrieve player informations
		DWORD LocalPlayer_Base = Mem->Read<DWORD>(Mem->ClientDLL_Base + m_dwLocalPlayer);
		int LocalPlayer_inCross = Mem->Read<int>(LocalPlayer_Base + m_iCrossHairID);
		int LocalPlayer_Team = Mem->Read<int>(LocalPlayer_Base + m_iTeamNum);

		//Retrieve the EntityBase, using dwEntityList
		DWORD Trigger_EntityBase = Mem->Read<DWORD>(Mem->ClientDLL_Base + m_dwEntityList + ((LocalPlayer_inCross - 1) * 0x10));
		int Trigger_EntityTeam = Mem->Read<int>(Trigger_EntityBase + m_iTeamNum);

		bool Trigger_EntityDormant = Mem->Read<bool>(Trigger_EntityBase + m_bDromant);

		if ((LocalPlayer_inCross > 0 && LocalPlayer_inCross <= 64) && (Trigger_EntityBase != NULL) && (Trigger_EntityTeam != LocalPlayer_Team) && (!Trigger_EntityDormant)) {

			// do the shooting here
			//External way, you can customize the delays
			Sleep(10); //Delay before shooting
			mouse_event(MOUSEEVENTF_LEFTDOWN,NULL,NULL,NULL,NULL);
			Sleep(10); //Delay between shots
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			Sleep(10); //Delay after shooting

			// Offset for m_dwForceAttack is 0x2EC89E8
			//Sleep(10); //Delay before shooting
			//Mem->Write<int>(Mem->ClientDLL_Base + m_dwForceAttack, 5);
			//Sleep(10); //Delay between shots
			//Mem->Write<int>(Mem->ClientDLL_Base + m_dwForceAttack, 4);
			//Sleep(10); //Delay after shooting

		}
 
	}
}

int main() {
	Mem = new MemoryManager();
	std::cout << "My awesome first triggerbot!" << std::endl;
	Trigger();
	delete Mem;
	return 0;
}
