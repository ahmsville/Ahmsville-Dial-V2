void Load_Configurations() {
void (*config_arr[])() = {
Altium,
Default,
Dolphin,
Notepad,
SOLIDWORKS
};
for (int i = 0; i < maxconfignum; i++) {
(*config_arr[i])();
 }
 }