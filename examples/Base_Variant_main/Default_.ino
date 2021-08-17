void Default() {
int index=0;
appconfig[index].appname="Default";
appconfig[index].ID=1;//default
appconfig[index].appcolor=CRGB::ALLColors;
appconfig[index].appanimation=solid;

appconfig[index].knob1_res=0;
appconfig[index].useapp_knob1_CW=true;
// inbuiltfunc_knob1_CW =true
// file_knob1_CW =false
// functionscombobox_knob1_CW =volume_up
appconfig[index].knob1_CW[0]=128;
appconfig[index].knob1_CW[1]=128;
appconfig[index].knob1_CW[2]=128;
appconfig[index].useapp_knob1_CCW=true;
// inbuiltfunc_knob1_CCW =true
// file_knob1_CCW =false
// functionscombobox_knob1_CCW =volume_down
appconfig[index].knob1_CCW[0]=128;
appconfig[index].knob1_CCW[1]=128;
appconfig[index].knob1_CCW[2]=128;
/*
appconfig[index].knob2_res=0;
appconfig[index].useapp_knob2_CW=false;
// inbuiltfunc_knob2_CW =false
// file_knob2_CW =false
// functionscombobox_knob2_CW =
appconfig[index].knob2_CW[0]=128;
appconfig[index].knob2_CW[1]=128;
appconfig[index].knob2_CW[2]=128;
appconfig[index].useapp_knob2_CCW=false;
// inbuiltfunc_knob2_CCW =false
// file_knob2_CCW =false
// functionscombobox_knob2_CCW =
appconfig[index].knob2_CCW[0]=128;
appconfig[index].knob2_CCW[1]=128;
appconfig[index].knob2_CCW[2]=128;
*/
appconfig[index].captouch_dualtapfunc[0]=false;
appconfig[index].captouch_dualtapfunc[1]=false;
appconfig[index].useapp_captouch_singletap=true;
// inbuiltfunc_captouch_singletap =true
// file_captouch_singletap =false
// functionscombobox_captouch_singletap =media_PlayPause
appconfig[index].singletap[0]=128;
appconfig[index].singletap[1]=128;
appconfig[index].singletap[2]=128;
appconfig[index].useapp_captouch_singletap2=false;
// inbuiltfunc_captouch_singletap2 =false
// file_captouch_singletap2 =false
// functionscombobox_captouch_singletap2 =
appconfig[index].singletap2[0]=128;
appconfig[index].singletap2[1]=128;
appconfig[index].singletap2[2]=128;
appconfig[index].useapp_captouch_doubletap=false;
// inbuiltfunc_captouch_doubletap =false
// file_captouch_doubletap =false
// functionscombobox_captouch_doubletap =
appconfig[index].doubletap[0]=128;
appconfig[index].doubletap[1]=128;
appconfig[index].doubletap[2]=128;
appconfig[index].useapp_captouch_doubletap2=false;
// inbuiltfunc_captouch_doubletap2 =false
// file_captouch_doubletap2 =false
// functionscombobox_captouch_doubletap2 =
appconfig[index].doubletap2[0]=128;
appconfig[index].doubletap2[1]=128;
appconfig[index].doubletap2[2]=128;
appconfig[index].useapp_captouch_shortpress=false;
// inbuiltfunc_captouch_shortpress =false
// file_captouch_shortpress =false
// functionscombobox_captouch_shortpress =
appconfig[index].shortpress[0]=128;
appconfig[index].shortpress[1]=128;
appconfig[index].shortpress[2]=128;
appconfig[index].useapp_captouch_longpress=false;
// inbuiltfunc_captouch_longpress =false
// file_captouch_longpress =false
// functionscombobox_captouch_longpress =
appconfig[index].longpress[0]=128;
appconfig[index].longpress[1]=128;
appconfig[index].longpress[2]=128;
/*
appconfig[index].MK_dualtapfunc[0]=false;
appconfig[index].MK_dualtapfunc[1]=false;
appconfig[index].MK_dualtapfunc[2]=false;
appconfig[index].MK_dualtapfunc[3]=false;
appconfig[index].MK_dualtapfunc[4]=false;
appconfig[index].MK_tapfunc[0]=true;
appconfig[index].MK_tapfunc[1]=true;
appconfig[index].MK_tapfunc[2]=true;
appconfig[index].MK_tapfunc[3]=true;
appconfig[index].MK_tapfunc[4]=true;
appconfig[index].MK_holdfunc[0]=true;
appconfig[index].MK_holdfunc[1]=true;
appconfig[index].MK_holdfunc[2]=true;
appconfig[index].MK_holdfunc[3]=true;
appconfig[index].MK_holdfunc[4]=true;

appconfig[index].MK_colors[0]=CRGB::Snow;
appconfig[index].MK_colors[1]=CRGB::Snow;
appconfig[index].MK_colors[2]=CRGB::Snow;
appconfig[index].MK_colors[3]=CRGB::Snow;
appconfig[index].MK_colors[4]=CRGB::Snow;
appconfig[index].MK_animation[0]=solid;
appconfig[index].MK_animation[1]=solid;
appconfig[index].MK_animation[2]=solid;
appconfig[index].MK_animation[3]=solid;
appconfig[index].MK_animation[4]=solid;

appconfig[index].useapp_MK1_tap=false;
// inbuiltfunc_MK1tap_SHORTCUT1 =false
// file_MK1tap_SHORTCUT1 =false
// functionscombobox_MK1tap_SHORTCUT1 =
appconfig[index].MK1_tap[0]=128;
appconfig[index].MK1_tap[1]=128;
appconfig[index].MK1_tap[2]=128;
appconfig[index].useapp_MK1_tap2=false;
// inbuiltfunc_MK1tap_SHORTCUT2 =false
// file_MK1tap_SHORTCUT2 =false
// functionscombobox_MK1tap_SHORTCUT2 =
appconfig[index].MK1_tap2[0]=128;
appconfig[index].MK1_tap2[1]=128;
appconfig[index].MK1_tap2[2]=128;
appconfig[index].MK1_taptext="128";
appconfig[index].useapp_MK1_hold=false;
// inbuiltfunc_MK1_hold =false
// file_MK1_hold =false
// functionscombobox_MK1hold =
appconfig[index].MK1_hold[0]=128;
appconfig[index].MK1_hold[1]=128;
appconfig[index].MK1_hold[2]=128;
appconfig[index].MK1_holdtext="128";

appconfig[index].useapp_MK2_tap=false;
// inbuiltfunc_MK2tap_SHORTCUT1 =false
// file_MK2tap_SHORTCUT1 =false
// functionscombobox_MK2tap_SHORTCUT1 =
appconfig[index].MK2_tap[0]=128;
appconfig[index].MK2_tap[1]=128;
appconfig[index].MK2_tap[2]=128;
appconfig[index].useapp_MK2_tap2=false;
// inbuiltfunc_MK2tap_SHORTCUT2 =false
// file_MK2tap_SHORTCUT2 =false
// functionscombobox_MK2tap_SHORTCUT2 =
appconfig[index].MK2_tap2[0]=128;
appconfig[index].MK2_tap2[1]=128;
appconfig[index].MK2_tap2[2]=128;
appconfig[index].MK2_taptext="128";
appconfig[index].useapp_MK2_hold=false;
// inbuiltfunc_MK2_hold =false
// file_MK2_hold =false
// functionscombobox_MK2hold =
appconfig[index].MK2_hold[0]=128;
appconfig[index].MK2_hold[1]=128;
appconfig[index].MK2_hold[2]=128;
appconfig[index].MK2_holdtext="128";

appconfig[index].useapp_MK3_tap=false;
// inbuiltfunc_MK3tap_SHORTCUT1 =false
// file_MK3tap_SHORTCUT1 =false
// functionscombobox_MK3tap_SHORTCUT1 =
appconfig[index].MK3_tap[0]=128;
appconfig[index].MK3_tap[1]=128;
appconfig[index].MK3_tap[2]=128;
appconfig[index].useapp_MK3_tap2=false;
// inbuiltfunc_MK3tap_SHORTCUT2 =false
// file_MK3tap_SHORTCUT2 =false
// functionscombobox_MK3tap_SHORTCUT2 =
appconfig[index].MK3_tap2[0]=128;
appconfig[index].MK3_tap2[1]=128;
appconfig[index].MK3_tap2[2]=128;
appconfig[index].MK3_taptext="128";
appconfig[index].useapp_MK3_hold=false;
// inbuiltfunc_MK3_hold =false
// file_MK3_hold =false
// functionscombobox_MK3hold =
appconfig[index].MK3_hold[0]=128;
appconfig[index].MK3_hold[1]=128;
appconfig[index].MK3_hold[2]=128;
appconfig[index].MK3_holdtext="128";

appconfig[index].useapp_MK4_tap=false;
// inbuiltfunc_MK4tap_SHORTCUT1 =false
// file_MK4tap_SHORTCUT1 =false
// functionscombobox_MK4tap_SHORTCUT1 =
appconfig[index].MK4_tap[0]=128;
appconfig[index].MK4_tap[1]=128;
appconfig[index].MK4_tap[2]=128;
appconfig[index].useapp_MK4_tap2=false;
// inbuiltfunc_MK4tap_SHORTCUT2 =false
// file_MK4tap_SHORTCUT2 =false
// functionscombobox_MK4tap_SHORTCUT2 =
appconfig[index].MK4_tap2[0]=128;
appconfig[index].MK4_tap2[1]=128;
appconfig[index].MK4_tap2[2]=128;
appconfig[index].MK4_taptext="128";
appconfig[index].useapp_MK4_hold=false;
// inbuiltfunc_MK4_hold =false
// file_MK4_hold =false
// functionscombobox_MK4hold =
appconfig[index].MK4_hold[0]=128;
appconfig[index].MK4_hold[1]=128;
appconfig[index].MK4_hold[2]=128;
appconfig[index].MK4_holdtext="128";

appconfig[index].useapp_MK5_tap=false;
// inbuiltfunc_MK5tap_SHORTCUT1 =false
// file_MK5tap_SHORTCUT1 =false
// functionscombobox_MK5tap_SHORTCUT1 =
appconfig[index].MK5_tap[0]=128;
appconfig[index].MK5_tap[1]=128;
appconfig[index].MK5_tap[2]=128;
appconfig[index].useapp_MK5_tap2=false;
// inbuiltfunc_MK5tap_SHORTCUT2 =false
// file_MK5tap_SHORTCUT2 =false
// functionscombobox_MK5tap_SHORTCUT2 =
appconfig[index].MK5_tap2[0]=128;
appconfig[index].MK5_tap2[1]=128;
appconfig[index].MK5_tap2[2]=128;
appconfig[index].MK5_taptext="128";
appconfig[index].useapp_MK5_hold=false;
// inbuiltfunc_MK5_hold =false
// file_MK5_hold =false
// functionscombobox_MK5hold =
appconfig[index].MK5_hold[0]=128;
appconfig[index].MK5_hold[1]=128;
appconfig[index].MK5_hold[2]=128;
appconfig[index].MK5_holdtext="128";
*/
/*
appconfig[index].spacenav_func=1;
appconfig[index].spacenav_continuousmode[0]=false;
appconfig[index].spacenav_continuousmode[1]=false;
appconfig[index].useapp_spacenav_tiltup=false;
// inbuiltfunc_Spacenav_tiltup =false
// file_Spacenav_tiltup =false
// functionscombobox_Spacenav_tiltup =
appconfig[index].spacenav_tiltup[0]=128;
appconfig[index].spacenav_tiltup[1]=128;
appconfig[index].spacenav_tiltup[2]=128;
appconfig[index].useapp_spacenav_tiltdown=false;
// inbuiltfunc_Spacenav_tiltdown =false
// file_Spacenav_tiltdown =false
// functionscombobox_Spacenav_tiltdown =
appconfig[index].spacenav_tiltdown[0]=128;
appconfig[index].spacenav_tiltdown[1]=128;
appconfig[index].spacenav_tiltdown[2]=128;
appconfig[index].useapp_spacenav_tiltright=false;
// inbuiltfunc_Spacenav_tiltright =false
// file_Spacenav_tiltright =false
// functionscombobox_Spacenav_tiltright =
appconfig[index].spacenav_tiltright[0]=128;
appconfig[index].spacenav_tiltright[1]=128;
appconfig[index].spacenav_tiltright[2]=128;
appconfig[index].useapp_spacenav_tiltleft=false;
// inbuiltfunc_Spacenav_tiltleft =false
// file_Spacenav_tiltleft =false
// functionscombobox_Spacenav_tiltleft =
appconfig[index].spacenav_tiltleft[0]=128;
appconfig[index].spacenav_tiltleft[1]=128;
appconfig[index].spacenav_tiltleft[2]=128;

appconfig[index].useapp_spacenav_slideup=false;
// inbuiltfunc_Spacenav_slideup =false
// file_Spacenav_slideup =false
// functionscombobox_Spacenav_slideup =
appconfig[index].spacenav_slideup[0]=128;
appconfig[index].spacenav_slideup[1]=128;
appconfig[index].spacenav_slideup[2]=128;
appconfig[index].useapp_spacenav_slidedown=false;
// inbuiltfunc_Spacenav_slidedown =false
// file_Spacenav_slidedown =false
// functionscombobox_Spacenav_slidedown =
appconfig[index].spacenav_slidedown[0]=128;
appconfig[index].spacenav_slidedown[1]=128;
appconfig[index].spacenav_slidedown[2]=128;
appconfig[index].useapp_spacenav_slideright=false;
// inbuiltfunc_Spacenav_slideright =false
// file_Spacenav_slideright =false
// functionscombobox_Spacenav_slideright =
appconfig[index].spacenav_slideright[0]=128;
appconfig[index].spacenav_slideright[1]=128;
appconfig[index].spacenav_slideright[2]=128;
appconfig[index].useapp_spacenav_slideleft=false;
// inbuiltfunc_Spacenav_slideleft =false
// file_Spacenav_slideleft =false
// functionscombobox_Spacenav_slideleft =
appconfig[index].spacenav_slideleft[0]=128;
appconfig[index].spacenav_slideleft[1]=128;
appconfig[index].spacenav_slideleft[2]=128;
*/
}
