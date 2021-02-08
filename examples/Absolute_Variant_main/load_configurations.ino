void Load_Configurations() {
void (*config_arr[])() = {
Default,
SOLIDWORKS
};
for (int i = 0; i < maxconfignum; i++) {
(*config_arr[i])();
 }
 }