void Load_Configurations() {
void (*config_arr[])() = {
Default,
SOLIDWORKS,
Fusion_360,
Premiere_Pro,
Audition,
Camera,
Altium
};
for (int i = 0; i < maxconfignum; i++) {
(*config_arr[i])();
 }
 }