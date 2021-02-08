void Load_Configurations() {
void (*config_arr[])() = {
Default,
Premiere,
SOLIDWORKS,
Altium
};
for (int i = 0; i < maxconfignum; i++) {
(*config_arr[i])();
 }
 }