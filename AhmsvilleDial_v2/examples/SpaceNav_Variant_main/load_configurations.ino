void Load_Configurations() {
void (*config_arr[])() = {
Blender,
Default,
SOLIDWORKS,
Fusion_360
};
for (int i = 0; i < maxconfignum; i++) {
(*config_arr[i])();
 }
 }