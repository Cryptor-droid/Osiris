#include "SightDown.h"
using namespace Sightdown;

SightData gundata[40] = {
	{Vector{-3.82f, 10.0f, 0.51f}, Vector{-3.72f, 10.0f, 0.56f}},     //Deagle
	{Vector{0.0f, -3.0f, 1.51f}, Vector{0.0f, -3.0f, 1.51f}},         //Dual Berettas
	{Vector{-2.79f, 10.0f, 0.87f}, Vector{-2.68f, 10.0f, 0.9f}},      //Five-Seven
	{Vector{-2.81f, 10.0f, 1.45f}, Vector{-2.75f, 10.0f, 1.48f}},     //Glock-18
	{},                                                               // There is no gun with id 5 and 6
	{},                                                               //
	{Vector{-3.82f, 10.0f, 0.51f}, Vector{-3.72f, 10.0f, 0.56f}},     //Ak-47      I forgot to set up this gun :p
	{},                                                               //AUG
	{},                                                               //AWP     Aug and Awp already have scope.
	{Vector{-6.29f, -4.78f, 1.24f}, Vector{-6.15f, -4.78f, 1.30f}}, //Famas
	{},                                                               //G3SG1 already has a scope
	{},                                                               // There is no gun with id 12
	{Vector{-5.32f, -6.52f, 1.59f}, Vector{-5.19f, -6.52f, 1.67f}}, //Galil AR
	{Vector{-7.58f, 10.0f, 1.52f}, Vector{-7.41f, 10.0f, 1.63f}},     //M249
	{},                                                               // There is no gun with id 15
	{Vector{-5.11f, -3.70f, 0.78f}, Vector{-5.06f, -3.7f, 0.8f}},   //M4A4
	{Vector{-6.15f, 10.0f, 1.98f}, Vector{-5.07f, 10.0f, 2.05f}},     //Mac10
	{},                                                               // There is no gun with id 18
	{Vector{-5.18f, 10.0f, -0.36f}, Vector{-4.99f, 10.0f, -0.25f}}, //P90
	{},                                                               //Repulsion Device
	{},                                                               // There is no gun with id 21
	{},                                                               // There is no gun with id 22
	{Vector{-3.82f, 10.0f, 0.51f}, Vector{-3.72f, 10.0f, 0.56f}},     //MP5-SD      I forgot to set up this gun :p
	{Vector{-5.11f, 10.0f, 1.45f}, Vector{-4.94f, 10.0f, 1.52f}},     //UMP-45
	{},                                                               //XM1014
	{Vector{-5.09f, 10.0f, 1.01f}, Vector{-4.9f , 10.0f, 1.13f}},     //PP-Bizon
	{},                                                               //MAG-7
	{Vector{-7.65f, 10.0f, 1.96f}, Vector{-7.47f,  5.0f,  2.1f}},     //Negev
	{},                                                               //Sawed-Off
	{Vector{-5.15f, 10.0f, 2.54f}, Vector{-5.08f, 10.0f, 2.56f}},     //Tec-9
	{},                                                               //Zeus x27
	{Vector{-2.81f, 10.0f, 1.09f}, Vector{-2.75f, 10.0f, 1.12f}},     //p2000
	{Vector{-5.31f, 10.0f, 0.96f}, Vector{-5.07f, 10.0f, 1.09f}},     //mp7
	{Vector{-4.74f, 10.0f, 0.86f}, Vector{-4.58f, 10.0f, 0.93f}},     //mp9
	{},                                                               //Nova
	{Vector{-2.78f, 10.0f, 0.94f}, Vector{-2.67f, 10.0f, 1.01f}},     //P250
	{},                                                               //Ballistic Shield
	{},                                                               //SCAR-20
	{},                                                               //SG 553
	{},                                                               //SSG 08
};

void Sightdown::init() {
	interfaces->cvar->findVar("viewmodel_offset_x")->onChangeCallbacks.size = 0;
	interfaces->cvar->findVar("viewmodel_offset_y")->onChangeCallbacks.size = 0;
	interfaces->cvar->findVar("viewmodel_offset_z")->onChangeCallbacks.size = 0;
	if (!hasInitted) {
		Vector initval;
		initval.x = interfaces->cvar->findVar("viewmodel_offset_x")->getFloat();
		initval.y = interfaces->cvar->findVar("viewmodel_offset_y")->getFloat();
		initval.z = interfaces->cvar->findVar("viewmodel_offset_z")->getFloat();
		initialOffset = initval;
		hasInitted = true;
	}
}

void Sightdown::setSight(UserCmd* cmd) {
	static float lerpamount = 0.0f;
	static bool switcher = false;
	if (localPlayer && localPlayer->isAlive()) {
		const auto activeWeapon = localPlayer->getActiveWeapon();
		auto weaponIndex = activeWeapon->itemDefinitionIndex() - 1;
		if (weaponIndex < 0 && weaponIndex > 39) weaponIndex = 39;
		SightData weapon = gundata[weaponIndex];
		Vector setval;
		if (!weapon.isEqualToInitialOffset)
		{
			cmd->buttons |= UserCmd::IN_ATTACK2;
			if (GetAsyncKeyState(VK_RBUTTON)) {//cmd->buttons & UserCmd::IN_ATTACK2) {
				lerpamount += 1.0f;
				cmd->viewangles -= localPlayer->aimPunchAngle() * 2.0f;
			}
			else {
				lerpamount -= 1.0f;
			}
			//if (cmd->buttons & UserCmd::IN_ATTACK) cmd->buttons |= UserCmd::IN_ATTACK2;

			if (cmd->buttons & UserCmd::IN_DUCK) {
				setval = weapon.cdata;
			}
			else {
				setval = weapon.data;
			}
		}
		else {
			setval = initialOffset;
		}
		lerpamount = std::clamp<float>(lerpamount, 0.0f, 8.f);
		interfaces->cvar->findVar("viewmodel_offset_x")->setValue(std::lerp(initialOffset.x,setval.x,lerpamount / 8.0f));
		interfaces->cvar->findVar("viewmodel_offset_y")->setValue(std::lerp(initialOffset.y, setval.y, lerpamount / 8.0f));
		interfaces->cvar->findVar("viewmodel_offset_z")->setValue(std::lerp(initialOffset.z, setval.z, lerpamount / 8.0f));
		config->visuals.fov = std::lerp(0.f, -35.f, lerpamount / 8.f);
	}
}