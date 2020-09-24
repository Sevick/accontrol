#pragma once
#include "esphome.h"

using namespace esphome;

class EspMenu : public Component, public CustomAPIDevice {

   const static int MENU_SIZE=4;

   struct MenuItem{
        char text[10];
        const char* mask;
        esphome::globals::GlobalsComponent<int>* globalValue;
        std::string* valueOptions;
        int optionsCount;
        int posX;
        int posY;         
        esphome::display::Font* font;
        int color;
        TextAlign textAlign;
      };

   bool firstCall=true;   
   int modescolor[2]={0xC618,0xFFF0};
   struct MenuItem menu[MENU_SIZE];
 
  public:
  
     static std::string delay_text[7];
     static std::string worktime_text[4];
     static std::string temp_text[12];
     static std::string acmode_text[2]; 
   
     void setup() override {
       ESP_LOGD("EspMenu","setup");
     }
  
     void loop(esphome::display::DisplayBuffer& it){
       if (firstCall){
          init(it);
          firstCall=false;
       }
       for (int i=0; i<MENU_SIZE; i++){
         sprintf(menu[i].text, menu[i].mask, (menu[i].valueOptions[id(menu[i].globalValue)]).c_str());
         it.printf(menu[i].posX, menu[i].posY, menu[i].font, menu[i].color, menu[i].textAlign, menu[i].text);
         if (i==id(menuposition)){
           it.line(menu[i].posX-10, menu[i].posY,menu[i].posX+10, menu[i].posY, 0x07E0);
         }
       }
     }
  
     void init(esphome::display::DisplayBuffer& it) {
       ESP_LOGD("EspMenu","init");
       menu[0]=MenuItem{"    ", "%s", acdelay, delay_text, 7, it.get_width()/2-30, 130, &id(font1), modescolor[0], TextAlign::BOTTOM_CENTER};
       menu[1]=MenuItem{"    ", "%s", acworktime, worktime_text, 4, it.get_width()/2+30, 130, &id(font1), modescolor[1], TextAlign::BOTTOM_CENTER};
       menu[2]=MenuItem{"    ", "%s°", actemperature, temp_text, 12, it.get_width()/2, 177, &id(font1), 0xC618, TextAlign::BOTTOM_CENTER};
       menu[3]=MenuItem{"    ", "%s", acmode, acmode_text, 2, it.get_width()/2, 222, &id(font1), 0xC618, TextAlign::BOTTOM_CENTER}; 
       ESP_LOGD("EspMenu","init done");
     }  
  
   
     void goDown(){
       if (id(menuposition)==MENU_SIZE-1)
         id(menuposition)=0;
       else
         id(menuposition)++;
     }
  
     void goUp(){
       if (id(menuposition)==0)
         id(menuposition)=MENU_SIZE-1;
       else
         id(menuposition)--;  
     }
  
  
     void incValue(){
       if (id(menu[id(menuposition)].globalValue)==menu[id(menuposition)].optionsCount-1)
         id(menu[id(menuposition)].globalValue)=0;
       else
         id(menu[id(menuposition)].globalValue)++;
     }
   
     void decValue(){
       if (id(menu[id(menuposition)].globalValue)==0)
         id(menu[id(menuposition)].globalValue)=menu[id(menuposition)].optionsCount-1;
       else
         id(menu[id(menuposition)].globalValue)--;
     }   
};


std::string EspMenu::delay_text[] = {"3","15","20","30","40","50","60"};
std::string EspMenu::worktime_text[] = {"5","10","15","20"};
std::string EspMenu::temp_text[] = {"18","19","20","21","22","23","24","25","26","27","28","29"};
std::string EspMenu::acmode_text[] = {"cool","heat"};
