void Load_Configurations() {
void (*config_arr[])() = {
AltiumSpecial,
Altium,
Blender,
Default,
Fusion_360,
Google_Docs,
PCB_Editor,
Premiere_Pro,
SOLIDWORKS,
Horizon
};
for (int i = 0; i < maxconfignum; i++) {
(*config_arr[i])();
 }
 }